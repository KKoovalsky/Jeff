/**
 * @file    audio_dac_impl.cpp
 * @brief   Implements the AudioDacImpl.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "audio_dac_impl.hpp"
#include "cmsis_os2.h"
#include "dac.h"
#include "os_waiters.hpp"

#include <algorithm>

enum class AudioDacEvents : EventBits_t
{
    half_transfer = 0b001,
    full_transfer = 0b010,
    quit = 0b100,
    any = 0b111
};

template<typename Enum>
constexpr std::underlying_type_t<Enum> to_underlying(Enum e) noexcept
{
    return static_cast<std::underlying_type_t<Enum>>(e);
}

AudioDacImpl::AudioDacImpl(SamplingTriggerTimer& t) :
    sampling_trigger_timer{t}, event_group_handle{xEventGroupCreate()}, worker("AudioDac", 1536, osPriorityNormal)
{
    singleton_pointer = this;
    MX_DAC1_Init();
}

AudioDacImpl::~AudioDacImpl()
{
    LL_DAC_DeInit(DAC1);

    vEventGroupDelete(event_group_handle);
    singleton_pointer = nullptr;
}

void AudioDacImpl::start()
{
    worker.start([this]() { this->thread_code(); });

    configure_dma();
    enable_dac();
    sampling_trigger_timer.start();
}

void AudioDacImpl::stop()
{
    auto quit_running_thread{[this]() {
        xEventGroupSetBits(this->event_group_handle, to_underlying(AudioDacEvents::quit));
    }};

    disable_dac();
    disable_dma();
    sampling_trigger_timer.stop();

    quit_running_thread();
    worker.join();
}

void AudioDacImpl::configure_dma()
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

void AudioDacImpl::enable_dac()
{
    auto wait_for_dac_to_settle{[]() {
        os::wait_milliseconds(1);
    }};

    LL_DAC_Enable(DAC1, LL_DAC_CHANNEL_1);
    wait_for_dac_to_settle();
    LL_DAC_EnableTrigger(DAC1, LL_DAC_CHANNEL_1);
    LL_DAC_EnableDMAReq(DAC1, LL_DAC_CHANNEL_1);
}

void AudioDacImpl::disable_dma()
{
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_3);
    LL_DMA_DisableIT_HT(DMA1, LL_DMA_CHANNEL_3);
    LL_DMA_DisableIT_TC(DMA1, LL_DMA_CHANNEL_3);
    LL_DMA_ClearFlag_HT3(DMA1);
    LL_DMA_ClearFlag_TC3(DMA1);
}

void AudioDacImpl::disable_dac()
{
    LL_DAC_DisableTrigger(DAC1, LL_DAC_CHANNEL_1);
    LL_DAC_Disable(DAC1, LL_DAC_CHANNEL_1);
}

void AudioDacImpl::set_on_stream_update_handler(AudioDac::Handler handler)
{
    stream_update_handler = std::move(handler);
}

AudioDacBatchOfSamples AudioDacImpl::get_new_samples()
{
    if (stream_update_handler)
        return stream_update_handler();
    else
        return std::make_unique<AudioDacBuffer>();
}

uint16_t AudioDacImpl::convert_sample(float)
{
    return 0;
}

extern "C" void DMA1_Channel3_IRQHandler(void)
{
    auto higher_priority_task_woken_during_half_transfer{pdFALSE};
    auto higher_priority_task_woken_during_full_transfer{pdFALSE};

    if (LL_DMA_IsActiveFlag_HT3(DMA1) == 1)
    {
        LL_DMA_ClearFlag_HT3(DMA1);

        auto event_group_handle{AudioDacImpl::singleton_pointer->event_group_handle};
        xEventGroupSetBitsFromISR(event_group_handle,
                                  to_underlying(AudioDacEvents::half_transfer),
                                  &higher_priority_task_woken_during_half_transfer);
    }

    if (LL_DMA_IsActiveFlag_TC3(DMA1) == 1)
    {
        LL_DMA_ClearFlag_TC3(DMA1);

        auto event_group_handle{AudioDacImpl::singleton_pointer->event_group_handle};
        xEventGroupSetBitsFromISR(event_group_handle,
                                  to_underlying(AudioDacEvents::full_transfer),
                                  &higher_priority_task_woken_during_full_transfer);
    }

    portYIELD_FROM_ISR(higher_priority_task_woken_during_half_transfer
                       or higher_priority_task_woken_during_full_transfer);
}

void AudioDacImpl::thread_code()
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

    auto copy_converted_samples{[this](auto destination_begin_it) {
        auto samples{this->get_new_samples()};
        std::ranges::transform(samples->begin(), samples->end(), destination_begin_it, convert_sample);
    }};

    auto copy_converted_samples_to_first_half_of_raw_buffer{[&]() {
        copy_converted_samples(std::begin(raw_sample_buffer));
    }};

    auto copy_converted_samples_to_second_half_of_raw_buffer{[&]() {
        auto raw_sample_buffer_half_indicator{std::next(std::begin(raw_sample_buffer), RawBufferSize / 2)};
        copy_converted_samples(raw_sample_buffer_half_indicator);
    }};

    while (true)
    {
        auto event{wait_for_any_event()};
        if (is_event(event, AudioDacEvents::half_transfer))
            copy_converted_samples_to_first_half_of_raw_buffer();

        if (is_event(event, AudioDacEvents::full_transfer))
            copy_converted_samples_to_second_half_of_raw_buffer();

        if (is_event(event, AudioDacEvents::quit))
            break;
    }
}
