/**
 * @file    threaded_audio_sampler.cpp
 * @brief   Implements the ThreadedAudioSampler.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include <cstring>
#include <string>
#include <string_view>

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

extern "C" void ADC1_IRQHandler(void)
{
}

ThreadedAudioSampler::Error::Error(const char* message)
{
    static constexpr std::string_view prefix{"ThreadedAudioSampler Error: "};

    exception_message.reserve(prefix.length() + std::strlen(message));
    exception_message.append(prefix);
    exception_message.append(message);
}

const char* ThreadedAudioSampler::Error::what() const noexcept
{
    return exception_message.c_str();
}
