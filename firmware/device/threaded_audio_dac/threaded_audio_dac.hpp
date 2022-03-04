/**
 * @file        threaded_audio_dac.hpp
 * @brief       Defines AudioDac implementation.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef AUDIO_DAC_IMPL_HPP
#define AUDIO_DAC_IMPL_HPP

#include "audio_chain_config.hpp"
#include "audio_dac.hpp"
#include "event_tracer.hpp"
#include "sampling_trigger_timer.hpp"

#include <array>
#include <cstdint>

#include "jungles/singleton.hpp"
#include "jungles_os_helpers/freertos/thread.hpp"

#include "FreeRTOS.h"
#include "event_groups.h"

using AudioDacInterface = AudioDac<AudioChainConfig::BatchOfSamples>;

extern "C" void DMA1_Channel3_IRQHandler();

class ThreadedAudioDac : public AudioDacInterface, jungles::Singleton<AudioDacInterface, AudioDacInterface::Error>
{
  public:
    explicit ThreadedAudioDac(SamplingTriggerTimer&, EventTracer&);
    ~ThreadedAudioDac() override;

    using BatchOfSamples = AudioChainConfig::BatchOfSamples;
    using Handler = std::function<BatchOfSamples(void)>;

    void start();
    void stop();
    void set_on_stream_update_handler(Handler);

    void await_stream_update(BatchOfSamples) override;

  private:
    void configure_dma();
    static void enable_dac();
    static void disable_dma();
    static void disable_dac();

    void thread_code();

    AudioChainConfig::BatchOfSamples get_new_samples();
    static uint16_t convert_sample(float);

    static inline ThreadedAudioDac* singleton_pointer{nullptr};

    static inline constexpr unsigned RawBufferSize{AudioChainConfig::WindowSize * 2};
    using RawSampleBuffer = std::array<uint16_t, RawBufferSize>;

    RawSampleBuffer raw_sample_buffer = {};

    Handler stream_update_handler;
    SamplingTriggerTimer& sampling_trigger_timer;

    EventTracer& event_tracer;

    EventGroupHandle_t event_group_handle;

    using Thread = jungles::freertos::thread;
    Thread worker;

    friend void DMA1_Channel3_IRQHandler();
};

#endif /* AUDIO_DAC_IMPL_HPP */
