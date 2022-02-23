/**
 * @file        jeff_app_wrapper.hpp
 * @brief       Wraps the application to be able to provide only the guitar effect, to apply DRY.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef JEFF_APP_WRAPPER_HPP
#define JEFF_APP_WRAPPER_HPP

#include "audio_chain_config.hpp"
#include "guitar_effect.hpp"

class JeffAppWrapper
{
  public:
    using GuitarEffectImpl = GuitarEffect<AudioChainConfig::BatchOfSamples>;

    explicit JeffAppWrapper(GuitarEffectImpl& guitar_effect);
    void loop();

    GuitarEffectImpl& guitar_effect;
};

#endif /* JEFF_APP_WRAPPER_HPP */
