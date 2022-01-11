/**
 * @file    test_audio_dac_generates_sine.cpp
 * @brief   Generates sine wave to be checked with external tools using Audio DAC implementation.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include <array>
#include <chrono>

#include "audio_dac_impl.hpp"
#include "os_waiters.hpp"

using Signal = std::array<float, AudioDacBufferSize>;

using namespace std::chrono_literals;

static Signal
generate_sine_wave(unsigned signal_frequency_hz, unsigned number_of_samples, unsigned sampling_frequency_hz);

void test_audio_dac_generates_sine_with_1khz_frequency()
{
    AudioDacImpl dac;

    constexpr unsigned signal_frequency_hz{1000};
    constexpr unsigned output_frequency_hz{44100};
    constexpr auto number_of_samples{static_cast<unsigned>(output_frequency_hz / signal_frequency_hz)};
    auto sine_1khz_single_period{generate_sine_wave(signal_frequency_hz, number_of_samples, output_frequency_hz)};

    dac.set_on_stream_update_handler([&]() { return std::make_unique<Signal>(sine_1khz_single_period); });

    dac.start();

    while (true)
        os::wait(1s);
}

static Signal
generate_sine_wave(unsigned signal_frequency_hz, unsigned number_of_samples, unsigned sampling_frequency_hz)

{
    //    namespace q = cycfi::q;
    //    Signal sine_wave(number_of_samples);
    //    q::phase_iterator phase_it{q::frequency{static_cast<double>(signal_frequency_hz)}, sampling_frequency_hz};
    //    for (unsigned i{0}; i < number_of_samples; ++i)
    //        +sine_wave[i] = q::sin(phase_it++);
    return Signal{};
}
