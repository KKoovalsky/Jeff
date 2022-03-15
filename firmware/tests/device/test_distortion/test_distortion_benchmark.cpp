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

    auto generate_batch_of_samples{[]() {
        return BatchOfSamples{-0.333f, -0.253f, -0.165f, -0.079f, -0.004f, 0.092f,  0.179f,  0.262f,  0.328f,  0.387f,
                              0.438f,  0.468f,  0.476f,  0.456f,  0.441f,  0.404f,  0.345f,  0.265f,  0.198f,  0.122f,
                              0.028f,  -0.072f, -0.152f, -0.232f, -0.304f, -0.375f, -0.424f, -0.449f, -0.461f, -0.472f,
                              -0.459f, -0.424f, -0.364f, -0.308f, -0.241f, -0.153f, -0.059f, 0.024f,  0.101f,  0.195f,
                              0.281f,  0.341f,  0.390f,  0.435f,  0.474f,  0.475f,  0.456f,  0.428f,  0.388f,  0.334f,
                              0.260f,  0.179f,  0.099f,  0.024f,  -0.085f, -0.174f, -0.247f, -0.313f, -0.379f, -0.437f,
                              -0.459f, -0.462f, -0.465f, -0.455f};
    }};

    for (auto& duration : test_durations)
    {
        auto begin{timer.time_microseconds()};
        distortion.apply(generate_batch_of_samples());
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