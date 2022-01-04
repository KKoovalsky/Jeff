/**
 * @file    test_sample_sine.cpp
 * @brief   Test helper which checks whether input signal matches sine wave.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "serial_logger.hpp"
#include "threaded_audio_sampler.hpp"

#include "unity.h"

#include "jungles/cross_correlation.hpp"
#include "jungles_os_helpers/freertos/flag.hpp"
#include "q/synth/sin.hpp"

using Signal = std::vector<float>;

static Signal
generate_sine_wave(unsigned signal_frequency_hz, unsigned number_of_samples, unsigned sampling_frequency_hz);
static void assert_signal_matches_pattern(const Signal& signal, const Signal& pattern);

void assert_input_signal_matches_sine(unsigned signal_frequency_hz)
{
    SerialLogger logger;
    logger.log(LogLevel::info) << "Testing " << signal_frequency_hz << " Hz sine wave detection ...";
    logger.log(LogLevel::info) << "Applied sine wave shall have amplitude 1.6V and " << signal_frequency_hz
                               << " Hz frequency.";

    unsigned number_of_periods{6};
    unsigned sampling_frequency_hz{44100};
    auto number_of_samples{static_cast<unsigned>(number_of_periods * sampling_frequency_hz / signal_frequency_hz)};
    unsigned margin{128};

    ThreadedAudioSampler sampler;
    std::vector<float> collected_samples;
    collected_samples.reserve(number_of_samples + margin);

    jungles::freertos::flag collection_finished_flag;
    sampler.set_on_batch_of_samples_received_handler([&](auto samples) {
        collected_samples.insert(std::end(collected_samples), std::begin(samples), std::end(samples));
        if (collected_samples.size() >= number_of_samples)
            collection_finished_flag.set();
    });

    sampler.start();
    collection_finished_flag.wait();
    sampler.stop();

    auto expected_signal{generate_sine_wave(signal_frequency_hz, number_of_samples, sampling_frequency_hz)};

    assert_signal_matches_pattern(collected_samples, expected_signal);
}

static Signal
generate_sine_wave(unsigned signal_frequency_hz, unsigned number_of_samples, unsigned sampling_frequency_hz)
{
    namespace q = cycfi::q;

    Signal sine_wave(number_of_samples);

    q::phase_iterator phase_it{q::frequency{static_cast<double>(signal_frequency_hz)}, sampling_frequency_hz};
    for (unsigned i{0}; i < number_of_samples; ++i)
        sine_wave[i] = q::sin(phase_it++);

    return sine_wave;
}

static void assert_signal_matches_pattern(const Signal& signal, const Signal& pattern)
{
    auto signal_beg{std::begin(signal)};
    auto signal_end{std::next(signal_beg, pattern.size())};

    unsigned offset{static_cast<unsigned>(std::distance(signal_beg, signal_end))};

    auto cross_correlation_function{jungles::calculate_correlation_function(
        signal_beg, signal_end, std::begin(pattern), std::end(pattern), offset)};

    auto cross_correlation_function_begin{std::begin(cross_correlation_function)};
    auto cross_correlation_function_end{std::end(cross_correlation_function)};

    auto maximum{*std::max_element(cross_correlation_function_begin, cross_correlation_function_end)};
    auto minimum{*std::min_element(cross_correlation_function_begin, cross_correlation_function_end)};

    SerialLogger{}.log(LogLevel::info) << "Cross-correlation factor: maximum: " << maximum << ", minimum: " << minimum;

    auto is_correlated{std::any_of(cross_correlation_function_begin,
                                   cross_correlation_function_end,
                                   [](auto coefficient) { return std::abs(coefficient) >= 0.9; })};

    TEST_ASSERT_TRUE(is_correlated);
}
