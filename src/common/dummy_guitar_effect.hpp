/**
 * @file        dummy_guitar_effect.hpp
 * @brief       Implements GuitarEffect which does nothing.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef DUMMY_GUITAR_EFFECT_HPP
#define DUMMY_GUITAR_EFFECT_HPP

#include "guitar_effect.hpp"

template <typename BatchOfSamples>
struct DummyGuitarEffect : GuitarEffect<BatchOfSamples>
{
    BatchOfSamples apply(BatchOfSamples samples) override
    {
        return samples;
    }
};

#endif /* DUMMY_GUITAR_EFFECT_HPP */
