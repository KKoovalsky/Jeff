/**
 * @file    jeff_app_wrapper.cpp
 * @brief   Implements the JeffAppWrapper.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "jeff_app_wrapper.hpp"
#include "threaded_audio_dac.hpp"
#include "os_waiters.hpp"
#include "sampling_trigger_timer_impl.hpp"
#include "threaded_audio_sampler.hpp"

#include "audio_chain.hpp"

#include "jungles_os_helpers/freertos/mutex.hpp"

JeffAppWrapper::JeffAppWrapper(GuitarEffectImpl& guitar_effect) : guitar_effect{guitar_effect}
{
}

void JeffAppWrapper::loop()
{
    SamplingTriggerTimerImpl sampling_trigger_timer;
    ThreadedAudioSampler audio_sampler{sampling_trigger_timer};
    ThreadedAudioDac audio_dac{sampling_trigger_timer};

    AudioChain<jungles::freertos::mutex, AudioChainConfig::BatchOfSamples> audio_chain{
        audio_sampler, guitar_effect, audio_dac};

    os::loop();
}