/**
 * @file        audio_chain.hpp
 * @brief       Implements AudioChain class.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef AUDIO_CHAIN_HPP
#define AUDIO_CHAIN_HPP

#include "audio_dac.hpp"
#include "audio_sampler.hpp"
#include "event_tracer.hpp"
#include "guitar_effect.hpp"

template<typename BatchOfSamples>
class AudioChain
{
  public:
    explicit AudioChain(AudioSampler<BatchOfSamples>& audio_sampler,
                        GuitarEffect<BatchOfSamples>& guitar_effect,
                        AudioDac<BatchOfSamples>& audio_dac,
                        EventTracer& event_tracer) :
        audio_sampler{audio_sampler}, guitar_effect{guitar_effect}, audio_dac{audio_dac}, event_tracer{event_tracer}
    {
    }

    void run_once()
    {
        event_tracer.capture("AudioChain: AudioSampler begin");
        auto new_batch_of_samples{audio_sampler.await_samples()};
        event_tracer.capture("AudioChain: AudioSampler end, GuitarEffect begin");
        auto mutated_samples{guitar_effect.apply(std::move(new_batch_of_samples))};
        event_tracer.capture("AudioChain: GuitarEffect end, AudioDac begin");
        audio_dac.await_stream_update(std::move(mutated_samples));
        event_tracer.capture("AudioChain: AudioDac end");
    }

  private:
    AudioSampler<BatchOfSamples>& audio_sampler;
    GuitarEffect<BatchOfSamples>& guitar_effect;
    AudioDac<BatchOfSamples>& audio_dac;
    EventTracer& event_tracer;
};

#endif /* AUDIO_CHAIN_HPP */
