/**
 * @file        threaded_audio_sampler.hpp
 * @brief       Implements ThreadedAudioSampler with STM32 LL library and using a thread.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef THREADED_AUDIO_SAMPLER_HPP
#define THREADED_AUDIO_SAMPLER_HPP

#include <cstdint>
#include <string>

#include "audio_sampler.hpp"

#include "jungles_os_helpers/freertos/queue_sending_from_isr.hpp"
#include "jungles_os_helpers/freertos/thread.hpp"
#include "jungles_os_helpers/generic_implementations/active.hpp"

constexpr inline unsigned SampleBufferSize{64};
using SampleBuffer = std::array<int, SampleBufferSize>;
using SampleIterator = typename SampleBuffer::const_iterator;
using AudioSamplerInterface = AudioSampler<SampleIterator, SampleIterator>;

// TODO: Singleton
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
    void calibrate() const;
    void enable();

    void handle_new_sample(uint16_t);

    SampleBuffer sample_buffer = {};
    Handler on_sample_received_handler;
    bool is_started{false};

    static constexpr std::size_t message_pump_size{8};

    template<typename T>
    using MessagePumpTemplate = jungles::freertos::queue_sending_from_isr<T, message_pump_size>;

    using Thread = jungles::freertos::thread;

    using Active = jungles::generic::active<uint16_t, MessagePumpTemplate, Thread>;
    using MessagePump = typename Active::MessagePump;

    MessagePump message_pump;
    Thread worker;
    Active active;
};

#endif /* THREADED_AUDIO_SAMPLER_HPP */
