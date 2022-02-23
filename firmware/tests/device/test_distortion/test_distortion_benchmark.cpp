/**
 * @file    test_distortion_benchmark.cpp
 * @brief   Tests whether BasicWindowedDistortionWithMemory applies the effect within proper duration.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "unity.h"

#include <algorithm>
#include <array>
#include <numeric>

#include "benchmark_timer.hpp"
#include "serial_logger.hpp"

#include "audio_chain_config.hpp"
#include "basic_windowed_distortion_with_memory.hpp"

using BatchOfSamples = AudioChainConfig::BatchOfSamples;
using Distortion = BasicWindowedDistortionWithMemory<AudioChainConfig::WindowSize>;

void test_applying_distortion_is_not_too_long()
{
    constexpr unsigned number_of_tests{1024};
    std::array<unsigned, number_of_tests> test_durations = {};

    SerialLogger logger;
    BenchmarkTimer timer;

    Distortion distortion{0.6};

    for (auto& duration : test_durations)
    {
        auto begin{timer.time_microseconds()};
        distortion.apply(BatchOfSamples{});
        auto end{timer.time_microseconds()};
        duration = end - begin;
    }

    auto [min_it, max_it]{std::minmax_element(std::begin(test_durations), std::end(test_durations))};
    auto min{*min_it};
    auto max{*max_it};
    auto sum{std::accumulate(std::begin(test_durations), std::end(test_durations), static_cast<uint64_t>(0))};
    auto average{static_cast<float>(sum) / static_cast<float>(std::size(test_durations))};

    logger.log(LogLevel::info) << "Distortion apply() execution times: max: " << max << " us, min: " << min
                               << " us, average: " << average << " us.";

    unsigned one_millisecond_in_microseconds{1000};
    TEST_ASSERT_LESS_OR_EQUAL(one_millisecond_in_microseconds, min);
    TEST_ASSERT_LESS_OR_EQUAL(one_millisecond_in_microseconds, max);
    TEST_ASSERT_LESS_OR_EQUAL(one_millisecond_in_microseconds, average);
}