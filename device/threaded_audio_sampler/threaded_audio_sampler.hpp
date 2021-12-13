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

// TODO: Singleton
class ThreadedAudioSampler : public AudioSampler<int>
{
  public:
    explicit ThreadedAudioSampler();
    ~ThreadedAudioSampler() override;

    void set_on_sample_received_handler(Handler) override;
    void start() override;
    void stop() override;

    class Error : public AudioSampler<int>::Error
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

    Handler on_sample_received_handler;
    bool is_started{false};

};

#endif /* THREADED_AUDIO_SAMPLER_HPP */
