/**
 * @file        guitar_effect.hpp
 * @brief       Guitar effect interface.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef GUITAR_EFFECT_HPP
#define GUITAR_EFFECT_HPP

#include <exception>

template<typename BatchOfSamples>
struct GuitarEffect
{
    virtual BatchOfSamples apply(BatchOfSamples) = 0;

    virtual ~GuitarEffect() = default;

    struct Error : std::exception
    {
    };
};

#endif /* GUITAR_EFFECT_HPP */
