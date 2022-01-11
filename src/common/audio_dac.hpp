/**
 * @file        audio_dac.hpp
 * @brief       Defines interface for an Audio DAC.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef AUDIO_DAC_HPP
#define AUDIO_DAC_HPP

#include <exception>
#include <functional>

template<typename BatchOfSamples>
struct AudioDac
{
    using Handler = std::function<BatchOfSamples()>;

    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void set_on_stream_update_handler(Handler) = 0;

    struct Error : std::exception
    {
    };

    virtual ~AudioDac() = default;
};

#endif /* AUDIO_DAC_HPP */
