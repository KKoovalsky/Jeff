/**
 * @file        test_audio_chain.cpp
 * @brief       Tests the AudioChain.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include <catch2/catch_test_macros.hpp>

#include <array>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

#include "audio_chain.hpp"
#include "dummy_event_tracer.hpp"
#include "jungles_os_helpers/native/flag.hpp"

static inline constexpr unsigned AudioBufferSize{64};
using AudioBuffer = std::array<unsigned char, AudioBufferSize>;
using BatchOfSamples = AudioBuffer;

struct AudioSamplerMock : AudioSampler<BatchOfSamples>
{
    void start() override
    {
        is_started = true;
    }

    BatchOfSamples await_samples() override
    {
        if (samples_collected_handler)
            return samples_collected_handler();
        else
            return BatchOfSamples{};
    }

    void stop() override
    {
        is_started = false;
    }

    std::function<BatchOfSamples()> samples_collected_handler;
    bool is_started{false};
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
    void start() override
    {
        is_started = true;
    }

    void stop() override
    {
        is_started = false;
    }

    void await_stream_update(BatchOfSamples samples) override
    {
        if (stream_updater)
            stream_updater(std::move(samples));
    }

    std::function<void(BatchOfSamples)> stream_updater;
    bool is_started;
};

TEST_CASE("AudioChain applies effect to samples and forwards it to DAC", "[audio_chain]")
{
    using Flag = jungles::flag;
    using namespace std::chrono_literals;

    AudioSamplerMock audio_sampler;
    GuitarEffectMock guitar_effect;
    AudioDacMock audio_dac;
    DummyEventTracer event_tracer;

    auto make_audio_chain_under_test{[&]() {
        return AudioChain{audio_sampler, guitar_effect, audio_dac, event_tracer};
    }};

    SECTION("AudioChain asks the AudioSampler for samples")
    {
        auto audio_chain{make_audio_chain_under_test()};
        Flag audio_sampler_asked_for_samples_flag;

        audio_sampler.samples_collected_handler = [&]() {
            audio_sampler_asked_for_samples_flag.set();
            return BatchOfSamples{};
        };

        std::thread t{[&]() {
            audio_chain.run();
        }};

        REQUIRE(audio_sampler_asked_for_samples_flag.wait_for(1s));

        audio_chain.stop();
        t.join();
    }

    SECTION("AudioChain asks the AudioSampler continuously for samples")
    {
        auto audio_chain{make_audio_chain_under_test()};
        Flag audio_sampler_asked_multiple_times_for_samples_flag;

        audio_sampler.samples_collected_handler = [&, called_times = 0]() mutable {
            ++called_times;
            if (called_times > 10)
                audio_sampler_asked_multiple_times_for_samples_flag.set();
            return BatchOfSamples{};
        };

        std::thread t{[&]() {
            audio_chain.run();
        }};

        REQUIRE(audio_sampler_asked_multiple_times_for_samples_flag.wait_for(1s));

        audio_chain.stop();
        t.join();
    }

    SECTION("Collected samples are mutated by the guitar effect")
    {
        auto audio_chain{make_audio_chain_under_test()};

        audio_sampler.samples_collected_handler = []() {
            BatchOfSamples samples;
            samples[0] = 0x0A;
            samples[1] = 0xBC;
            return samples;
        };

        Flag is_samples_mutated_flag;

        guitar_effect.effect_applier = [&](auto samples) {
            if (samples[0] == 0x0A and samples[1] == 0xBC)
                is_samples_mutated_flag.set();
            return samples;
        };

        std::thread t{[&]() {
            audio_chain.run();
        }};

        REQUIRE(is_samples_mutated_flag.wait_for(1s));

        audio_chain.stop();
        t.join();
    }

    SECTION("Forwards mutated samples to DAC")
    {
        auto audio_chain{make_audio_chain_under_test()};

        guitar_effect.effect_applier = [&](auto) {
            BatchOfSamples samples;
            samples[0] = 0x37;
            samples[1] = 0xB6;
            return samples;
        };

        Flag is_mutated_samples_forwarded_to_audio_dac_flag;

        audio_dac.stream_updater = [&](auto samples) {
            if (samples[0] == 0x37 and samples[1] == 0xB6)
                is_mutated_samples_forwarded_to_audio_dac_flag.set();
        };

        std::thread t{[&]() {
            audio_chain.run();
        }};

        REQUIRE(is_mutated_samples_forwarded_to_audio_dac_flag.wait_for(1s));

        audio_chain.stop();
        t.join();
    }

    SECTION("Starts the sampler at the beginning")
    {
        auto audio_chain{make_audio_chain_under_test()};
        REQUIRE(audio_sampler.is_started);
    }

    SECTION("Starts the dac at the beginning")
    {
        auto audio_chain{make_audio_chain_under_test()};
        REQUIRE(audio_dac.is_started);
    }

    SECTION("Stops the sampler at the end")
    {
        make_audio_chain_under_test();
        REQUIRE_FALSE(audio_sampler.is_started);
    }

    SECTION("Stops the dac at the end")
    {
        make_audio_chain_under_test();
        REQUIRE_FALSE(audio_dac.is_started);
    }
}
