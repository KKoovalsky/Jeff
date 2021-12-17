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

#include "audio_sampler.hpp"

#include "jungles_os_helpers/freertos/queue_sending_from_isr.hpp"
#include "jungles_os_helpers/freertos/thread.hpp"
#include "jungles_os_helpers/generic_implementations/active.hpp"

constexpr inline unsigned SampleBufferSize{64};

// TODO: better use std::vector which is easily movable?
using RawSampleBuffer = std::array<uint16_t, SampleBufferSize>;

using SampleBuffer = std::array<int, SampleBufferSize>;
using SampleIterator = typename SampleBuffer::const_iterator;
using AudioSamplerInterface = AudioSampler<SampleIterator, SampleIterator>;

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

    void send_new_batch_of_samples_to_active();
    static int convert_sample(uint16_t raw_sample);
    void handle_new_batch_of_samples(const RawSampleBuffer&);

    static inline ThreadedAudioSampler* singleton_pointer{nullptr};

    RawSampleBuffer raw_sample_buffer = {};
    Handler on_samples_received_handler;
    bool is_started{false};

    static constexpr std::size_t message_pump_size{8};

    template<typename T>
    using MessagePumpTemplate = jungles::freertos::queue_sending_from_isr<T, message_pump_size>;

    using Thread = jungles::freertos::thread;

    using Active = jungles::generic::active<RawSampleBuffer, MessagePumpTemplate, Thread>;
    using MessagePump = typename Active::MessagePump;

    MessagePump message_pump;
    Thread worker;
    Active active;

    friend void DMA1_Channel1_IRQHandler();
};

#endif /* THREADED_AUDIO_SAMPLER_HPP */
