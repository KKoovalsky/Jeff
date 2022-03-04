/**
 * @file        threaded_audio_sampler.hpp
 * @brief       Implements ThreadedAudioSampler with STM32 LL library and using a thread.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef THREADED_AUDIO_SAMPLER_HPP
#define THREADED_AUDIO_SAMPLER_HPP

#include <array>
#include <cstdint>
#include <string>

#include "audio_chain_config.hpp"
#include "audio_sampler.hpp"
#include "event_tracer.hpp"
#include "sampling_trigger_timer.hpp"

#include "jungles_os_helpers/freertos/thread.hpp"

#include "FreeRTOS.h"
#include "event_groups.h"

extern "C" void DMA1_Channel1_IRQHandler();

using AudioSamplerInterface = AudioSampler<AudioChainConfig::BatchOfSamples>;

class ThreadedAudioSampler : public AudioSamplerInterface
{
  public:
    explicit ThreadedAudioSampler(SamplingTriggerTimer&, EventTracer&);
    ~ThreadedAudioSampler() override;

    using BatchOfSamples = AudioChainConfig::BatchOfSamples;

    BatchOfSamples await_samples() override;

    class Error : public AudioSamplerInterface::Error
    {
      public:
        explicit Error(const char* message);

        [[nodiscard]] const char* what() const noexcept override;

      private:
        std::string exception_message;
    };

  private:
    void start();
    void stop();
    void configure_dma();
    static void disable_dma();
    static void calibrate_adc();
    static void enable_adc();
    static void disable_adc();

    static float convert_sample(uint16_t raw_sample);

    static inline ThreadedAudioSampler* singleton_pointer{nullptr};

    SamplingTriggerTimer& sampling_trigger_timer;
    EventTracer& event_tracer;

    static constexpr inline unsigned RawSampleBufferSize{AudioChainConfig::WindowSize * 2};
    using RawSampleBuffer = std::array<uint16_t, RawSampleBufferSize>;

    RawSampleBuffer raw_sample_buffer = {};

    using RawSampleIterator = RawSampleBuffer::const_iterator;
    const RawSampleIterator raw_sample_buffer_half_indicator{
        std::next(std::begin(raw_sample_buffer), RawSampleBufferSize / 2)};

    bool is_started{false};

    EventGroupHandle_t audio_sampler_events;

    friend void DMA1_Channel1_IRQHandler();
};

#endif /* THREADED_AUDIO_SAMPLER_HPP */
