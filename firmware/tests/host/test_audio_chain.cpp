/**
 * @file        test_audio_chain.cpp
 * @brief       Tests the AudioChain.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include <string>
#include <vector>

#include "audio_chain.hpp"
#include "dummy_event_tracer.hpp"

using BatchOfSamples = std::string;

struct AudioSamplerMock : AudioSampler<BatchOfSamples>
{
    BatchOfSamples await_samples() override
    {
        if (samples_collected_handler)
            return samples_collected_handler();
        else
            return BatchOfSamples{};
    }

    std::function<BatchOfSamples()> samples_collected_handler;
};

struct GuitarEffectMock : GuitarEffect<BatchOfSamples>
{
    BatchOfSamples apply(BatchOfSamples samples) override
    {
        if (effect_applier)
            return effect_applier(std::move(samples));
        else
            return BatchOfSamples{};
    }

    std::function<BatchOfSamples(BatchOfSamples)> effect_applier;
};

struct AudioDacMock : AudioDac<BatchOfSamples>
{
    void await_stream_update(BatchOfSamples samples) override
    {
        if (stream_updater)
            stream_updater(std::move(samples));
    }

    std::function<void(BatchOfSamples)> stream_updater;
};

TEST_CASE("AudioChain applies effect to samples and forwards it to DAC", "[audio_chain]")
{
    AudioSamplerMock audio_sampler;
    GuitarEffectMock guitar_effect;
    AudioDacMock audio_dac;
    DummyEventTracer event_tracer;

    auto make_audio_chain_under_test{[&]() {
        return AudioChain{audio_sampler, guitar_effect, audio_dac, event_tracer};
    }};

    SECTION("Collected samples are mutated by the guitar effect")
    {
        auto audio_chain{make_audio_chain_under_test()};

        audio_sampler.samples_collected_handler = []() {
            return "Samples from the AudioSampler";
        };

        std::string result;
        guitar_effect.effect_applier = [&](auto samples) {
            result = samples;
            return samples;
        };

        audio_chain.run_once();

        REQUIRE(result == "Samples from the AudioSampler");
    }

    SECTION("Forwards mutated samples to DAC")
    {
        auto audio_chain{make_audio_chain_under_test()};

        guitar_effect.effect_applier = [&](auto) {
            return "Samples from the GuitarEffect";
        };

        std::string result;
        audio_dac.stream_updater = [&](auto samples) {
            result = samples;
        };

        audio_chain.run_once();

        REQUIRE(result == "Samples from the GuitarEffect");
    }

    SECTION("Maintains the signal chain continuously")
    {
        auto audio_chain{make_audio_chain_under_test()};

        audio_sampler.samples_collected_handler = [ordinal_number = 0]() mutable {
            return std::to_string(ordinal_number++);
        };

        auto sample_forwarder{[](auto samples) {
            return samples;
        }};

        guitar_effect.effect_applier = sample_forwarder;

        std::vector<std::string> samples_captured_by_dac;
        audio_dac.stream_updater = [&](auto samples) {
            samples_captured_by_dac.push_back(samples);
        };

        audio_chain.run_once();
        audio_chain.run_once();
        audio_chain.run_once();
        audio_chain.run_once();

        REQUIRE_THAT(samples_captured_by_dac, Catch::Matchers::Equals(std::vector<std::string>{"0", "1", "2", "3"}));
    }
}
