/**
 * @file    threaded_audio_dac.cpp
 * @brief   Implements the ThreadedAudioDac.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "threaded_audio_dac.hpp"
#include "cmsis_os2.h"
#include "dac.h"
#include "os_waiters.hpp"

#include <algorithm>
#include <cmath>

enum class AudioDacEvents : EventBits_t
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

ThreadedAudioDac::ThreadedAudioDac(SamplingTriggerTimer& t, EventTracer& et) :
    sampling_trigger_timer{t}, event_tracer{et}, event_group_handle{xEventGroupCreate()}
{
    singleton_pointer = this;
    MX_DAC1_Init();

    start();
}

ThreadedAudioDac::~ThreadedAudioDac()
{
    stop();

    LL_DAC_DeInit(DAC1);

    vEventGroupDelete(event_group_handle);
    singleton_pointer = nullptr;
}

void ThreadedAudioDac::start()
{
    configure_dma();
    enable_dac();
    sampling_trigger_timer.start();
}

void ThreadedAudioDac::stop()
{
    disable_dac();
    disable_dma();
    sampling_trigger_timer.stop();
}

void ThreadedAudioDac::configure_dma()
{
    LL_DMA_ConfigAddresses(DMA1,
                           LL_DMA_CHANNEL_3,
                           reinterpret_cast<uint32_t>(raw_sample_buffer.data()),
                           LL_DAC_DMA_GetRegAddr(DAC1, LL_DAC_CHANNEL_1, LL_DAC_DMA_REG_DATA_12BITS_RIGHT_ALIGNED),
                           LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_3, raw_sample_buffer.size());

    auto enable_dma_interrupt_on_half_transfer{[]() {
        LL_DMA_EnableIT_HT(DMA1, LL_DMA_CHANNEL_3);
    }};

    auto enable_dma_interrupt_on_full_transfer{[]() {
        LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_3);
    }};

    enable_dma_interrupt_on_half_transfer();
    enable_dma_interrupt_on_full_transfer();

    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_3);
}

void ThreadedAudioDac::enable_dac()
{
    auto wait_for_dac_to_settle{[]() {
        os::wait_milliseconds(1);
    }};

    LL_DAC_Enable(DAC1, LL_DAC_CHANNEL_1);
    wait_for_dac_to_settle();
    LL_DAC_EnableTrigger(DAC1, LL_DAC_CHANNEL_1);
    LL_DAC_EnableDMAReq(DAC1, LL_DAC_CHANNEL_1);
}

void ThreadedAudioDac::disable_dma()
{
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_3);
    LL_DMA_DisableIT_HT(DMA1, LL_DMA_CHANNEL_3);
    LL_DMA_DisableIT_TC(DMA1, LL_DMA_CHANNEL_3);
    LL_DMA_ClearFlag_HT3(DMA1);
    LL_DMA_ClearFlag_TC3(DMA1);
}

void ThreadedAudioDac::disable_dac()
{
    LL_DAC_DisableTrigger(DAC1, LL_DAC_CHANNEL_1);
    LL_DAC_Disable(DAC1, LL_DAC_CHANNEL_1);
}

uint16_t ThreadedAudioDac::convert_sample(float sample)
{
    static constexpr unsigned max_dac_value{__LL_DAC_DIGITAL_SCALE(LL_DAC_RESOLUTION_12B)};

    float sample_rescaled_from_plus_minus_one_to_zero_one_range{(sample + 1.0f) / 2.0f};
    float final_converted_sample{sample_rescaled_from_plus_minus_one_to_zero_one_range * max_dac_value};
    auto final_converted_sample_rounded{static_cast<uint16_t>(std::lround(final_converted_sample))};
    return final_converted_sample_rounded;
}

extern "C" void DMA1_Channel3_IRQHandler(void)
{
    auto higher_priority_task_woken_during_half_transfer{pdFALSE};
    auto higher_priority_task_woken_during_full_transfer{pdFALSE};

    if (LL_DMA_IsActiveFlag_HT3(DMA1) == 1)
    {
        LL_DMA_ClearFlag_HT3(DMA1);

        auto event_group_handle{ThreadedAudioDac::singleton_pointer->event_group_handle};
        xEventGroupSetBitsFromISR(event_group_handle,
                                  to_underlying(AudioDacEvents::half_transfer),
                                  &higher_priority_task_woken_during_half_transfer);
    }

    if (LL_DMA_IsActiveFlag_TC3(DMA1) == 1)
    {
        LL_DMA_ClearFlag_TC3(DMA1);

        auto event_group_handle{ThreadedAudioDac::singleton_pointer->event_group_handle};
        xEventGroupSetBitsFromISR(event_group_handle,
                                  to_underlying(AudioDacEvents::full_transfer),
                                  &higher_priority_task_woken_during_full_transfer);
    }

    portYIELD_FROM_ISR(higher_priority_task_woken_during_half_transfer
                       or higher_priority_task_woken_during_full_transfer);
}

void ThreadedAudioDac::await_stream_update(ThreadedAudioDac::BatchOfSamples samples)
{
    auto wait_for_any_event{[this]() {
        auto do_clear_event_bits_on_exit{pdTRUE};
        auto do_wait_for_any_bit_set{pdFALSE};
        auto do_wait_indefinitely{portMAX_DELAY};
        return xEventGroupWaitBits(this->event_group_handle,
                                   to_underlying(AudioDacEvents::any),
                                   do_clear_event_bits_on_exit,
                                   do_wait_for_any_bit_set,
                                   do_wait_indefinitely);
    }};

    auto is_event{[](auto event_bits, AudioDacEvents event) {
        return (event_bits & to_underlying(event)) != 0;
    }};

    auto stream_to_first_half_of_dma_buffer{[&](BatchOfSamples&& samples) {
        std::ranges::transform(std::begin(samples), std::end(samples), std::begin(raw_sample_buffer), convert_sample);
    }};

    auto stream_to_second_half_of_dma_buffer{[&](BatchOfSamples&& samples) {
        auto raw_sample_buffer_half_indicator{std::next(std::begin(raw_sample_buffer), RawBufferSize / 2)};
        std::ranges::transform(
            std::begin(samples), std::end(samples), raw_sample_buffer_half_indicator, convert_sample);
    }};

    auto event{wait_for_any_event()};
    event_tracer.capture("ThreadedAudioDac: begin");

    if (is_event(event, AudioDacEvents::half_transfer))
        stream_to_first_half_of_dma_buffer(std::move(samples));
    else if (is_event(event, AudioDacEvents::full_transfer))
        stream_to_second_half_of_dma_buffer(std::move(samples));
    else
        throw Error{"AudioDac: Unknown event"};
}
