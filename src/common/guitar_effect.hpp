/**
 * @file        guitar_effect.hpp
 * @brief       Guitar effect interface.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef GUITAR_EFFECT_HPP
#define GUITAR_EFFECT_HPP

template<typename BatchOfSamples>
struct GuitarEffect
{
    virtual BatchOfSamples apply(BatchOfSamples) = 0;

    virtual ~GuitarEffect() = default;
};

#endif /* GUITAR_EFFECT_HPP */
