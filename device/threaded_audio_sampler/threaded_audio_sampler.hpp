/**
 * @file        threaded_audio_sampler.hpp
 * @brief       Implements ThreadedAudioSampler with STM32 LL library and using a thread.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef THREADED_AUDIO_SAMPLER_HPP
#define THREADED_AUDIO_SAMPLER_HPP

#include "audio_sampler.hpp"

class ThreadedAudioSampler : public AudioSampler<int>
{
  public:
    void set_on_sample_received_handler(Handler) override;
};

#endif /* THREADED_AUDIO_SAMPLER_HPP */
