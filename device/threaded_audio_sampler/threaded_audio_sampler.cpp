/**
 * @file    threaded_audio_sampler.cpp
 * @brief   Implements the ThreadedAudioSampler.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "threaded_audio_sampler.hpp"

ThreadedAudioSampler::ThreadedAudioSampler()
{
}

ThreadedAudioSampler::~ThreadedAudioSampler()
{
}

void ThreadedAudioSampler::set_on_sample_received_handler(Handler handler)
{
    on_sample_received_handler = std::move(handler);
}

void ThreadedAudioSampler::start()
{
}

void ThreadedAudioSampler::stop()
{
}
