/**
 * @file    audio_sampler.hpp
 * @brief   Interface for an audio sampler.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef AUDIO_SAMPLER_HPP
#define AUDIO_SAMPLER_HPP

#include <exception>
#include <functional>

template<typename Sample>
struct AudioSampler
{
    using Handler = std::function<void(Sample)>;

    virtual void set_on_sample_received_handler(Handler) = 0;
    virtual void start() = 0;
    virtual void stop() = 0;

    virtual ~AudioSampler() = default;

    struct Error : std::exception
    {
    };
};

#endif /* AUDIO_SAMPLER_HPP */
