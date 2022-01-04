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
#include <vector>

#include "audio_sampler.hpp"

#include "jungles_os_helpers/freertos/thread.hpp"

#include "FreeRTOS.h"
#include "event_groups.h"
#include "semphr.h"

constexpr inline unsigned RawSampleBufferSize{128};

using RawSampleBuffer = std::array<uint16_t, RawSampleBufferSize>;

using SampleBuffer = std::vector<float>;
using AudioSamplerInterface = AudioSampler<SampleBuffer>;

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

    RawSampleBuffer raw_sample_buffer = {};

    using RawSampleIterator = RawSampleBuffer::const_iterator;
    const RawSampleIterator raw_sample_buffer_half_indicator{
        std::next(std::begin(raw_sample_buffer), RawSampleBufferSize / 2)};

    Handler on_samples_received_handler;
    bool is_started{false};

    EventGroupHandle_t audio_sampler_events;
    SemaphoreHandle_t thread_finished_semaphore;

    using Thread = jungles::freertos::thread;
    Thread worker;

    friend void DMA1_Channel1_IRQHandler();
};

#endif /* THREADED_AUDIO_SAMPLER_HPP */
