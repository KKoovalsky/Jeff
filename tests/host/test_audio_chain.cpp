/**
 * @file        test_audio_chain.cpp
 * @brief       Tests the AudioChain.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include <catch2/catch_test_macros.hpp>

#include <array>
#include <memory>
#include <mutex>

#include "audio_chain.hpp"
#include "dummy_event_tracer.hpp"

static inline constexpr unsigned AudioBufferSize{64};
using AudioBuffer = std::array<unsigned char, AudioBufferSize>;
using BatchOfSamples = std::unique_ptr<AudioBuffer>;

struct AudioSamplerMock : AudioSampler<BatchOfSamples>
{
    void set_on_batch_of_samples_received_handler(Handler handler) override
    {
        samples_collected_handler = handler;
    }

    void start() override
    {
        is_started = true;
    }

    void stop() override
    {
        is_started = false;
    }

    Handler samples_collected_handler;
    bool is_started{false};
};

struct GuitarEffectMock : GuitarEffect<BatchOfSamples>
{
    BatchOfSamples apply(BatchOfSamples samples) override
    {
        return effect_applier(std::move(samples));
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

    void set_on_stream_update_handler(Handler handler) override
    {
        stream_updater = handler;
    }

    Handler stream_updater;
    bool is_started;
};

TEST_CASE("AudioChain applies effect to samples and forwards it to DAC", "[audio_chain]")
{
    AudioSamplerMock audio_sampler;
    GuitarEffectMock guitar_effect;
    AudioDacMock audio_dac;
    DummyEventTracer event_tracer;

    auto make_audio_chain_under_test{[&]() {
        return AudioChain<std::mutex, BatchOfSamples>{audio_sampler, guitar_effect, audio_dac, event_tracer};
    }};

    SECTION("Collected samples are mutated by the guitar effect")
    {
        auto audio_chain{make_audio_chain_under_test()};

        auto collected_samples_ptr{std::make_unique<AudioBuffer>()};

        (*collected_samples_ptr)[0] = 0x0A;
        (*collected_samples_ptr)[1] = 0xBC;

        bool is_samples_mutated{false};

        guitar_effect.effect_applier = [&](auto samples_ptr) {
            const auto& samples{*samples_ptr};
            if (samples[0] == 0x0A and samples[1] == 0xBC)
                is_samples_mutated = true;
            return std::move(samples_ptr);
        };

        audio_sampler.samples_collected_handler(std::move(collected_samples_ptr));
        audio_dac.stream_updater();

        REQUIRE(is_samples_mutated);
    }

    SECTION("Forwards mutated samples to DAC")
    {
        auto audio_chain{make_audio_chain_under_test()};

        guitar_effect.effect_applier = [&](auto samples_ptr) {
            auto& samples{*samples_ptr};
            samples[0] = 0x37;
            samples[1] = 0xB6;
            return std::move(samples_ptr);
        };

        audio_sampler.samples_collected_handler(std::make_unique<AudioBuffer>());
        auto mutated_samples_pointer{audio_dac.stream_updater()};

        const auto& mutated_samples{*mutated_samples_pointer};
        REQUIRE(mutated_samples[0] == 0x37);
        REQUIRE(mutated_samples[1] == 0xB6);
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
