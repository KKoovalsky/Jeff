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
#include <string>
#include <string_view>

#include "os_waiters.hpp"
#include "threaded_audio_sampler.hpp"

#include "adc.h"

#include "cmsis_os2.h"
#include "jungles_os_helpers/freertos/poller.hpp"

ThreadedAudioSampler::ThreadedAudioSampler() :
    worker{"AudioSampler", 1024, osPriorityNormal},
    active{[this](auto&& v) { this->handle_new_batch_of_samples(v); }, message_pump, worker}
{
    if (singleton_pointer != nullptr)
        throw Error{"Only one instance can be run at a time!"};

    singleton_pointer = this;

    MX_ADC1_Init();
}

ThreadedAudioSampler::~ThreadedAudioSampler()
{
    LL_ADC_DeInit(ADC1);

    singleton_pointer = nullptr;
}

void ThreadedAudioSampler::set_on_batch_of_samples_received_handler(Handler handler)
{
    on_samples_received_handler = std::move(handler);
}

void ThreadedAudioSampler::start()
{
    if (is_started)
        return;

    configure_dma();
    calibrate();
    enable();

    is_started = true;
}

void ThreadedAudioSampler::stop()
{
    if (!is_started)
        return;

    // TODO: implement
    // TODO: synchronize ending or prevent from calling the IT handler, by disabling IT and clearing the IT flag.

    is_started = false;
}

void ThreadedAudioSampler::calibrate()
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

void ThreadedAudioSampler::enable()
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

int ThreadedAudioSampler::convert_sample(uint16_t raw_sample)
{
    static constexpr unsigned reference_voltage_in_millivolts{3300};
    unsigned sample_in_millivolts{
        __LL_ADC_CALC_DATA_TO_VOLTAGE(reference_voltage_in_millivolts, raw_sample, LL_ADC_RESOLUTION_12B)};

    static constexpr unsigned dc_offset_in_millivolts{1650};

    static constexpr unsigned maximum_positive_amplitude{reference_voltage_in_millivolts - dc_offset_in_millivolts};
    static constexpr unsigned maximum_negative_amplitude{dc_offset_in_millivolts - 0};
    static constexpr unsigned maximum_amplitude{std::max(maximum_positive_amplitude, maximum_negative_amplitude)};

    static constexpr int normalizing_factor_to_int_max{std::numeric_limits<int>::max() / maximum_amplitude};

    auto sample_in_millivolts_without_dc_offset{static_cast<int>(sample_in_millivolts - dc_offset_in_millivolts)};
    int sample{sample_in_millivolts_without_dc_offset * normalizing_factor_to_int_max};
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

    LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);

    //    /* Enable DMA transfer interruption: half transfer */
    //    LL_DMA_EnableIT_HT(DMA1, LL_DMA_CHANNEL_1);
    //    /* Enable DMA transfer interruption: transfer error */
    //    LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_1);

    /* Enable the DMA transfer */
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
}

extern "C" void DMA1_Channel1_IRQHandler(void)
{
    if (LL_DMA_IsActiveFlag_TC1(DMA1) == 1)
    {
        LL_DMA_ClearFlag_TC1(DMA1);

        // DMA puts the data to the buffer specified with LL_DMA_ConfigAddresses().
        ThreadedAudioSampler::singleton_pointer->send_new_batch_of_samples_to_active();
    }
}

void ThreadedAudioSampler::send_new_batch_of_samples_to_active()
{
    auto raw_samples{raw_sample_buffer};
    active.send(std::move(raw_samples));
}

void ThreadedAudioSampler::handle_new_batch_of_samples(const RawSampleBuffer& raw_samples)
{
    SampleBuffer samples;
    std::ranges::transform(std::begin(raw_samples), std::end(raw_samples), std::begin(samples), convert_sample);
    if (on_samples_received_handler)
        on_samples_received_handler(std::begin(samples), std::end(samples));
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
