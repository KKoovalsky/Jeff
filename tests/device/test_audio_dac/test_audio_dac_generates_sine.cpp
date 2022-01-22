/**
 * @file    test_audio_dac_generates_sine.cpp
 * @brief   Generates sine wave to be checked with external tools using Audio DAC implementation.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include <array>
#include <chrono>

#include "audio_dac_impl.hpp"
#include "os_waiters.hpp"
#include "sampling_trigger_timer_impl.hpp"
#include "serial_logger.hpp"

#include "q/synth/sin.hpp"

using Signal = std::array<float, AudioDacBufferSize>;

using namespace std::chrono_literals;

[[noreturn]] void test_audio_dac_generates_sine_with_1khz_frequency();

void test_audio_dac_generates_sine_with_1khz_frequency()
{
    constexpr unsigned signal_frequency_hz{1000};

    SerialLogger logger;
    logger.log(LogLevel::info) << "Starting continuous generation of " << signal_frequency_hz << " Hz sine wave...";

    SamplingTriggerTimerImpl sampling_trigger_timer;
    AudioDacImpl dac{sampling_trigger_timer};

    constexpr unsigned output_frequency_hz{44100};

    dac.set_on_stream_update_handler([&]() {
        namespace q = cycfi::q;
        static q::phase_iterator phase_it{q::frequency{static_cast<double>(signal_frequency_hz)}, output_frequency_hz};

        auto samples{std::make_unique<Signal>()};
        for (auto& sample : *samples)
            sample = q::sin(phase_it++);

        return samples;
    });

    dac.start();

    while (true)
        os::wait(1s);
}