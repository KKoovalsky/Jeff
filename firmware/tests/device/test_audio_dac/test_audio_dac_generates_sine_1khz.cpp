/**
 * @file    test_audio_dac_generates_sine_1khz.cpp
 * @brief   Generates sine wave to be checked with external tools using Audio DAC implementation.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include <array>
#include <chrono>

#include "os_waiters.hpp"
#include "sampling_trigger_timer_impl.hpp"
#include "serial_logger.hpp"
#include "audio_dac_with_dma.hpp"

#include "dummy_event_tracer.hpp"

#include "q/synth/sin.hpp"

using namespace std::chrono_literals;

[[noreturn]] void test_audio_dac_generates_sine_with_1khz_frequency();

void test_audio_dac_generates_sine_with_1khz_frequency()
{
    constexpr unsigned signal_frequency_hz{1000};

    SerialLogger logger;
    logger.log(LogLevel::info) << "Starting continuous generation of " << signal_frequency_hz << " Hz sine wave...";

    SamplingTriggerTimerImpl sampling_trigger_timer;
    DummyEventTracer event_tracer;
    AudioDacWithDma dac{sampling_trigger_timer, event_tracer};

    auto sine_generator{[]() {
        constexpr unsigned output_frequency_hz{44100};

        namespace q = cycfi::q;
        static q::phase_iterator phase_it{q::frequency{static_cast<double>(signal_frequency_hz)}, output_frequency_hz};

        AudioChainConfig::BatchOfSamples samples;
        for (auto& sample : samples)
            sample = q::sin(phase_it++);

        return samples;
    }};

    while (true)
        dac.await_stream_update(sine_generator());
}