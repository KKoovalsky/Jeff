/**
 * @file    test_sample_sine.cpp
 * @brief   Test helper which checks whether input signal matches sine wave.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "sampling_trigger_timer_impl.hpp"
#include "serial_logger.hpp"
#include "threaded_audio_sampler.hpp"

#include "dummy_event_tracer.hpp"

#include "unity.h"

#include "jungles_os_helpers/freertos/flag.hpp"

#include "arm_math.h"

#include <cmath>
#include <numeric>

using Signal = std::vector<float>;

static void
assert_signal_contains_single_harmonic(Signal& signal, unsigned harmonic_hz, unsigned sampling_frequency_hz);

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

    SamplingTriggerTimerImpl sampling_trigger_timer;
    DummyEventTracer event_tracer;
    ThreadedAudioSampler sampler{sampling_trigger_timer, event_tracer};
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

    assert_signal_contains_single_harmonic(collected_samples, signal_frequency_hz, sampling_frequency_hz);
}

static void
assert_signal_contains_single_harmonic(Signal& signal, unsigned int harmonic_hz, unsigned sampling_frequency_hz)
{
    auto to_fft_size{[](unsigned value) {
        static constexpr unsigned powers_of_two[] = {
            1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384};
        auto match_it{std::upper_bound(std::begin(powers_of_two), std::end(powers_of_two), value)};
        return *std::prev(match_it);
    }};

    auto to_harmonic_index_in_audio_fft{[sampling_frequency_hz](unsigned fft_length, unsigned harmonic_hz) {
        auto fft_max_frequency{static_cast<float>(sampling_frequency_hz) / 2.0};
        return static_cast<unsigned>(std::round(harmonic_hz / fft_max_frequency * fft_length));
    }};

    auto compute_fft{[&to_fft_size](Signal& signal) {
        arm_rfft_fast_instance_f32 fft_instance = {};
        auto fft_size{to_fft_size(signal.size())};
        arm_rfft_fast_init_f32(&fft_instance, fft_size);

        Signal fft(fft_size);
        uint8_t no_ifft{0};
        arm_rfft_fast_f32(&fft_instance, signal.data(), fft.data(), no_ifft);

        std::transform(std::begin(fft), std::end(fft), std::begin(fft), [](auto v) { return std::fabs(v); });

        return fft;
    }};

    auto get_sum_around{[]<typename Container>(const Container& container, unsigned index, unsigned neighbour_count) {
        auto index_begin{static_cast<int>(index) - static_cast<int>(neighbour_count)};
        auto index_end{index + neighbour_count + 1};
        if (index_begin < 0)
            index_begin = 0;
        if (index_end > container.size())
            index_end = container.size();

        auto begin{std::next(std::begin(container), index_begin)};
        auto end{std::next(std::begin(container), index_end)};

        using Value = typename Container::value_type;
        auto zero{Value{}};
        return std::accumulate(begin, end, zero);
    }};

    // The longer the FFT the more indices in FFT the harmonic will occupy. This function will calculate the spread
    // in one side. So e.g. if the result is 1, then the harmonic occupies 3 indices; if 2 then the harmonic occupies
    // 5 indices (the actual middle index + 2 neighbours on the left side + 2 neighbours on the right side); if 3, then
    // 7 indices ...
    auto get_harmonic_fft_spread_count{[](unsigned fft_size) {
        if (fft_size > 1024)
            return 2;
        else
            return 1;
    }};

    auto fft{compute_fft(signal)};
    auto harmonic_index{to_harmonic_index_in_audio_fft(fft.size(), harmonic_hz)};
    auto spread_count{get_harmonic_fft_spread_count(fft.size())};
    auto signal_magnitude{get_sum_around(fft, harmonic_index, spread_count)};

    auto iterator_ignoring_dc_offset{std::next(std::begin(fft), 1)};
    auto noise_magnitude{std::accumulate(iterator_ignoring_dc_offset, std::end(fft), -signal_magnitude)};
    auto snr{signal_magnitude / noise_magnitude};

    SerialLogger{}.log(LogLevel::info) << "SNR: " << snr;
    TEST_ASSERT(snr > 0.5);
}
