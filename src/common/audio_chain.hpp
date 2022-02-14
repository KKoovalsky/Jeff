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

#include <concepts>
#include <mutex>
#include <queue>

template<typename Mutex, typename BatchOfSamples>
class AudioChain
{
  public:
    explicit AudioChain(AudioSampler<BatchOfSamples>& audio_sampler,
                        GuitarEffect<BatchOfSamples>& guitar_effect,
                        AudioDac<BatchOfSamples>& audio_dac,
                        EventTracer& event_tracer) :
        audio_sampler{audio_sampler}, guitar_effect{guitar_effect}, audio_dac{audio_dac}, event_tracer{event_tracer}
    {
        audio_sampler.set_on_batch_of_samples_received_handler([this](auto samples) {
            auto raii_events_captor{this->event_tracer.make_raii_events_captor(
                "AudioChain: ADC samples received callback begin", "AudioChain: ADC samples received callback end")};
            std::lock_guard g{queue_mutex};
            queue_with_batches_of_samples.push(std::move(samples));
        });

        audio_dac.set_on_stream_update_handler([this]() {
            auto raii_events_captor{this->event_tracer.make_raii_events_captor(
                "AudioChain: DAC stream update callback begin", "AudioChain: DAC stream update callback end")};
            return this->guitar_effect.apply(pop_next_batch_of_samples());
        });

        audio_sampler.start();

        // TODO: might implement DAC start delay by asking some Timer for delaying, and by asking the DAC and the
        // sampler for sampling frequency, and also throw if it is different (might be also a constexpr check).

        audio_dac.start();
    }

    ~AudioChain()
    {
        audio_dac.stop();
        audio_sampler.stop();
    }

  private:
    BatchOfSamples pop_next_batch_of_samples()
    {
        std::lock_guard g{queue_mutex};
        auto batch{std::move(queue_with_batches_of_samples.front())};
        queue_with_batches_of_samples.pop();
        return batch;
    }

    AudioSampler<BatchOfSamples>& audio_sampler;
    GuitarEffect<BatchOfSamples>& guitar_effect;
    AudioDac<BatchOfSamples>& audio_dac;
    EventTracer& event_tracer;

    Mutex queue_mutex;
    std::queue<BatchOfSamples> queue_with_batches_of_samples;
};

#endif /* AUDIO_CHAIN_HPP */
