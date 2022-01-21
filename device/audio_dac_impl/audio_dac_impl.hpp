/**
 * @file        audio_dac_impl.hpp
 * @brief       Defines AudioDac implementation.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef AUDIO_DAC_IMPL_HPP
#define AUDIO_DAC_IMPL_HPP

#include "audio_dac.hpp"
#include "sampling_trigger_timer.hpp"

#include <array>
#include <cstdint>
#include <memory>

#include "jungles/singleton.hpp"
#include "jungles_os_helpers/freertos/thread.hpp"

#include "FreeRTOS.h"
#include "event_groups.h"

static inline constexpr unsigned AudioDacBufferSize{64};

using AudioDacBuffer = std::array<float, AudioDacBufferSize>;
using AudioDacBatchOfSamples = std::unique_ptr<AudioDacBuffer>;
using AudioDacInterface = AudioDac<AudioDacBatchOfSamples>;

extern "C" void DMA1_Channel3_IRQHandler();

class AudioDacImpl : public AudioDacInterface, jungles::Singleton<AudioDacInterface, AudioDacInterface::Error>
{
  public:
    explicit AudioDacImpl(SamplingTriggerTimer&);
    ~AudioDacImpl() override;

    void start() override;
    void stop() override;
    void set_on_stream_update_handler(Handler) override;

  private:
    void configure_dma();
    static void enable_dac();
    static void disable_dma();
    static void disable_dac();

    void thread_code();

    AudioDacBatchOfSamples get_new_samples();
    static uint16_t convert_sample(float);

    static inline AudioDacImpl* singleton_pointer{nullptr};

    static inline constexpr unsigned RawBufferSize{AudioDacBufferSize * 2};
    using RawSampleBuffer = std::array<uint16_t, RawBufferSize>;

    RawSampleBuffer raw_sample_buffer = {};

    Handler stream_update_handler;
    SamplingTriggerTimer& sampling_trigger_timer;

    EventGroupHandle_t event_group_handle;

    using Thread = jungles::freertos::thread;
    Thread worker;

    friend void DMA1_Channel3_IRQHandler();
};

#endif /* AUDIO_DAC_IMPL_HPP */
