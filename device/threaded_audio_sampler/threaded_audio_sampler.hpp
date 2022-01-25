/**
 * @file        threaded_audio_sampler.hpp
 * @brief       Implements ThreadedAudioSampler with STM32 LL library and using a thread.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef THREADED_AUDIO_SAMPLER_HPP
#define THREADED_AUDIO_SAMPLER_HPP

#include <array>
#include <cstdint>
#include <memory>
#include <string>

#include "audio_sampler.hpp"

#include "jungles_os_helpers/freertos/thread.hpp"

#include "FreeRTOS.h"
#include "event_groups.h"

static constexpr inline unsigned AudioSamplerBufferSize{64};
using AudioSamplerBuffer = std::array<float, AudioSamplerBufferSize>;
using AudioSamplerBatchOfSamples = std::unique_ptr<AudioSamplerBuffer>;
using AudioSamplerInterface = AudioSampler<AudioSamplerBatchOfSamples>;

extern "C" void DMA1_Channel1_IRQHandler();

class ThreadedAudioSampler : public AudioSamplerInterface
{
  public:
    explicit ThreadedAudioSampler();
    ~ThreadedAudioSampler() override;

    void set_on_batch_of_samples_received_handler(Handler) override;
    void start() override;
    void stop() override;

    class Error : public AudioSamplerInterface::Error
    {
      public:
        explicit Error(const char* message);

        [[nodiscard]] const char* what() const noexcept override;

      private:
        std::string exception_message;
    };

  private:
    void configure_dma();
    static void disable_dma();
    static void calibrate_adc();
    static void enable_adc();
    static void disable_adc();

    static float convert_sample(uint16_t raw_sample);
    void thread_code();

    static inline ThreadedAudioSampler* singleton_pointer{nullptr};

    static constexpr inline unsigned RawSampleBufferSize{AudioSamplerBufferSize * 2};
    using RawSampleBuffer = std::array<uint16_t, RawSampleBufferSize>;

    RawSampleBuffer raw_sample_buffer = {};

    using RawSampleIterator = RawSampleBuffer::const_iterator;
    const RawSampleIterator raw_sample_buffer_half_indicator{
        std::next(std::begin(raw_sample_buffer), RawSampleBufferSize / 2)};

    Handler on_samples_received_handler;
    bool is_started{false};

    EventGroupHandle_t audio_sampler_events;

    using Thread = jungles::freertos::thread;
    Thread worker;

    friend void DMA1_Channel1_IRQHandler();
};

#endif /* THREADED_AUDIO_SAMPLER_HPP */
