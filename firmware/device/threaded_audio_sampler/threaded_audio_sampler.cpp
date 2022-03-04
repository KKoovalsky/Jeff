/**
 * @file    threaded_audio_sampler.cpp
 * @brief   Implements the ThreadedAudioSampler.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include <algorithm>
#include <chrono>
#include <cstring>
#include <limits>
#include <ranges>
#include <string_view>
#include <utility>

#include "os_waiters.hpp"
#include "threaded_audio_sampler.hpp"

#include "adc.h"

#include "cmsis_os2.h"
#include "jungles_os_helpers/freertos/poller.hpp"

enum class ThreadedAudioSamplerEvent : EventBits_t
{
    half_transfer = 0b001,
    full_transfer = 0b010,
    any = 0b011
};

template<typename Enum>
constexpr std::underlying_type_t<Enum> to_underlying(Enum e) noexcept
{
    return static_cast<std::underlying_type_t<Enum>>(e);
}

ThreadedAudioSampler::ThreadedAudioSampler(SamplingTriggerTimer& sampling_trigger_timer, EventTracer& event_tracer) :
    sampling_trigger_timer(sampling_trigger_timer),
    event_tracer{event_tracer},
    audio_sampler_events{xEventGroupCreate()}
{
    if (singleton_pointer != nullptr)
        throw Error{"Only one instance can be run at a time!"};

    singleton_pointer = this;

    MX_ADC1_Init();

    start();
}

ThreadedAudioSampler::~ThreadedAudioSampler()
{
    stop();

    LL_ADC_DeInit(ADC1);

    vEventGroupDelete(audio_sampler_events);

    singleton_pointer = nullptr;
}

void ThreadedAudioSampler::start()
{
    if (is_started)
        return;

    configure_dma();
    calibrate_adc();
    enable_adc();

    sampling_trigger_timer.start();

    is_started = true;
}

void ThreadedAudioSampler::stop()
{
    if (!is_started)
        return;

    sampling_trigger_timer.stop();

    disable_adc();
    disable_dma();

    is_started = false;
}

void ThreadedAudioSampler::calibrate_adc()
{
    auto wait_for_calibration_to_end_or_throw_if_failure{[]() {
        auto poller{jungles::freertos::make_poller<1, 5>()};
        auto calibration_finished{poller.poll([]() { return !LL_ADC_IsCalibrationOnGoing(ADC1); })};
        if (!calibration_finished)
            throw Error{"Calibration error!"};
    }};

    auto delay_before_enabling{[]() {
        os::wait(std::chrono::milliseconds{200});
    }};

    LL_ADC_StartCalibration(ADC1, LL_ADC_SINGLE_ENDED);
    wait_for_calibration_to_end_or_throw_if_failure();
    delay_before_enabling();
}

void ThreadedAudioSampler::enable_adc()
{
    auto wait_for_adc_to_start_or_throw_if_failure{[]() {
        auto poller{jungles::freertos::make_poller<1, 5>()};
        auto calibration_finished{poller.poll([]() { return LL_ADC_IsActiveFlag_ADRDY(ADC1) != 0; })};
        if (!calibration_finished)
            throw Error{"Failed to start!"};
    }};

    LL_ADC_Enable(ADC1);
    wait_for_adc_to_start_or_throw_if_failure();
    LL_ADC_REG_StartConversion(ADC1);
    // TODO: handle errors from LL_ADC_IsActiveFlag_OVR(ADC1) (overrun detection).
}

float ThreadedAudioSampler::convert_sample(uint16_t raw_sample)
{
    static constexpr unsigned reference_voltage_in_millivolts{3300};
    unsigned sample_in_millivolts{
        __LL_ADC_CALC_DATA_TO_VOLTAGE(reference_voltage_in_millivolts, raw_sample, LL_ADC_RESOLUTION_12B)};

    static constexpr unsigned dc_offset_in_millivolts{1650};

    static constexpr unsigned maximum_positive_amplitude{reference_voltage_in_millivolts - dc_offset_in_millivolts};
    static constexpr unsigned maximum_negative_amplitude{dc_offset_in_millivolts - 0};
    static constexpr unsigned maximum_amplitude{std::max(maximum_positive_amplitude, maximum_negative_amplitude)};

    auto sample_in_millivolts_without_dc_offset{static_cast<int>(sample_in_millivolts - dc_offset_in_millivolts)};
    auto sample{static_cast<float>(sample_in_millivolts_without_dc_offset) / maximum_amplitude};
    return sample;
}

void ThreadedAudioSampler::configure_dma()
{
    LL_DMA_ConfigAddresses(DMA1,
                           LL_DMA_CHANNEL_1,
                           LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA),
                           reinterpret_cast<uint32_t>(raw_sample_buffer.data()),
                           LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, raw_sample_buffer.size());

    auto enable_dma_interrupt_on_half_transfer{[]() {
        LL_DMA_EnableIT_HT(DMA1, LL_DMA_CHANNEL_1);
    }};

    auto enable_dma_interrupt_on_full_transfer{[]() {
        LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);
    }};

    enable_dma_interrupt_on_half_transfer();
    enable_dma_interrupt_on_full_transfer();

    //    /* Enable DMA transfer interruption: transfer error */
    //    LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_1);

    /* Enable the DMA transfer */
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
}

void ThreadedAudioSampler::disable_dma()
{
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);
    LL_DMA_DisableIT_HT(DMA1, LL_DMA_CHANNEL_1);
    LL_DMA_DisableIT_TC(DMA1, LL_DMA_CHANNEL_1);
    LL_DMA_ClearFlag_HT1(DMA1);
    LL_DMA_ClearFlag_TC1(DMA1);
}

void ThreadedAudioSampler::disable_adc()
{
    auto delay_before_disabling{[]() {
        vTaskDelay(pdMS_TO_TICKS(100));
    }};

    LL_ADC_REG_StopConversion(ADC1);
    delay_before_disabling();
    LL_ADC_Disable(ADC1);
}

extern "C" void DMA1_Channel1_IRQHandler(void)
{
    // DMA puts the data to the buffer specified with LL_DMA_ConfigAddresses().

    auto higher_priority_task_woken_during_half_transfer{pdFALSE};
    auto higher_priority_task_woken_during_full_transfer{pdFALSE};

    if (LL_DMA_IsActiveFlag_HT1(DMA1) == 1)
    {
        LL_DMA_ClearFlag_HT1(DMA1);

        auto threaded_audio_sampler_events{ThreadedAudioSampler::singleton_pointer->audio_sampler_events};
        xEventGroupSetBitsFromISR(threaded_audio_sampler_events,
                                  to_underlying(ThreadedAudioSamplerEvent::half_transfer),
                                  &higher_priority_task_woken_during_half_transfer);
    }

    if (LL_DMA_IsActiveFlag_TC1(DMA1) == 1)
    {
        LL_DMA_ClearFlag_TC1(DMA1);

        auto threaded_audio_sampler_events{ThreadedAudioSampler::singleton_pointer->audio_sampler_events};
        xEventGroupSetBitsFromISR(threaded_audio_sampler_events,
                                  to_underlying(ThreadedAudioSamplerEvent::full_transfer),
                                  &higher_priority_task_woken_during_full_transfer);
    }

    portYIELD_FROM_ISR(higher_priority_task_woken_during_half_transfer
                       or higher_priority_task_woken_during_full_transfer);
}

ThreadedAudioSampler::BatchOfSamples ThreadedAudioSampler::await_samples()
{
    auto wait_for_any_event{[this]() {
        auto do_clear_event_bits_on_exit{pdTRUE};
        auto do_wait_for_any_bit_set{pdFALSE};
        auto do_wait_indefinitely{portMAX_DELAY};
        return xEventGroupWaitBits(this->audio_sampler_events,
                                   to_underlying(ThreadedAudioSamplerEvent::any),
                                   do_clear_event_bits_on_exit,
                                   do_wait_for_any_bit_set,
                                   do_wait_indefinitely);
    }};

    auto is_event{[](auto event_bits, ThreadedAudioSamplerEvent event) {
        return (event_bits & to_underlying(event)) != 0;
    }};

    auto convert_samples{[](auto raw_samples_begin, auto raw_samples_end) {
        // TODO: We might copy the raw_samples_* range before handling, to prevent from races.
        AudioChainConfig::BatchOfSamples samples;
        std::ranges::transform(raw_samples_begin, raw_samples_end, std::begin(samples), convert_sample);
        return samples;
    }};

    auto event{wait_for_any_event()};
    event_tracer.capture("ThreadedAudioSampler: begin");
    if (is_event(event, ThreadedAudioSamplerEvent::half_transfer))
    {
        auto begin{std::cbegin(raw_sample_buffer)};
        auto end{raw_sample_buffer_half_indicator};
        return convert_samples(begin, end);
    } else if (is_event(event, ThreadedAudioSamplerEvent::full_transfer))
    {
        auto begin{raw_sample_buffer_half_indicator};
        auto end{std::cend(raw_sample_buffer)};
        return convert_samples(begin, end);
    } else
    {
        throw Error{"Unknown event while awaiting samples."};
    }
}

ThreadedAudioSampler::Error::Error(const char* message)
{
    static constexpr std::string_view prefix{"ThreadedAudioSampler Error: "};

    exception_message.reserve(prefix.length() + std::strlen(message));
    exception_message.append(prefix);
    exception_message.append(message);
}

const char* ThreadedAudioSampler::Error::what() const noexcept
{
    return exception_message.c_str();
}