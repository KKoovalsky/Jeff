/**
 * @file    audio_sampler.hpp
 * @brief   Interface for an audio sampler.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef AUDIO_SAMPLER_HPP
#define AUDIO_SAMPLER_HPP

#include <exception>
#include <functional>

template<typename BatchOfSamples>
struct AudioSampler
{
    virtual void start() = 0;
    virtual BatchOfSamples await_samples() = 0;
    virtual void stop() = 0;

    virtual ~AudioSampler() = default;

    struct Error : std::exception
    {
    };
};

#endif /* AUDIO_SAMPLER_HPP */
