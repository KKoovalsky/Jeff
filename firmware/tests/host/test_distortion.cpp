/**
 * @file        test_distortion.cpp
 * @brief       Tests the basic windowed distortion with memory.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "catch2/benchmark/catch_benchmark.hpp"
#include "catch2/catch_approx.hpp"
#include "catch2/catch_test_macros.hpp"
#include "catch2/generators/catch_generators.hpp"
#include "catch2/generators/catch_generators_range.hpp"
#include "catch2/matchers/catch_matchers.hpp"
#include "catch2/matchers/catch_matchers_templated.hpp"

#include "distortion.hpp"
#include "guitar_effect.hpp"

#include <algorithm>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <utility>

bool is_sample_equal(float a, float b)
{
    // Samples are within range < -1 ; 1 > so we assume constant epsilon.
    constexpr float epsilon{1e-6};
    float r{a - b};
    return r > -epsilon && r < epsilon;
}

template<typename Range>
struct SamplesMatcher : Catch::Matchers::MatcherGenericBase
{
    SamplesMatcher(const Range& samples) : samples{samples}
    {
    }

    template<typename BatchOfSamples>
    bool match(const BatchOfSamples& other_samples) const
    {
        return std::equal(std::begin(samples),
                          std::end(samples),
                          std::begin(other_samples),
                          std::end(other_samples),
                          is_sample_equal);
    }

    std::string describe() const override
    {
        return "Equals: " + Catch::rangeToString(samples);
    }

    Range samples;
};

template<typename Range>
auto EqualsSamples(const Range& samples)
{
    return SamplesMatcher{samples};
}

namespace HardClippingAtBeginningTestData
{

static constexpr unsigned WindowSize{4};
static constexpr float threshold{0.7};

using Input = std::vector<float>;
using ExpectedOutput = std::vector<float>;

using DistortionUnderTest = Distortion<WindowSize>;

using TestData = std::tuple<Input, ExpectedOutput>;

static const std::initializer_list<TestData> test_data{
    {Input{-0.1f, 0.2f, 0.7f, 0.8f}, ExpectedOutput{-0.1f, 0.2f, 0.7f, 0.8f}},

    // Scenarios more or less follow patterns:
    // lower, higher, lower, higher
    // lower, higher, higher, lower
    // higher, lower, lower, higher
    // higher, lower, lower, lower
    // ...
    // Only positive values ...
    {Input{0.1f, 0.8f, 0.2f, 0.7f}, ExpectedOutput{0.1f, 0.8f, 0.285714f, 0.8f}},
    {Input{0.1f, 0.8f, 0.5f, 0.2f}, ExpectedOutput{0.1f, 0.8f, 0.714286f, 0.285714f}},
    {Input{0.6f, 0.04f, 0.3f, 0.9f}, ExpectedOutput{0.6f, 0.057143f, 0.428571f, 0.9f}},
    {Input{0.5f, 0.04f, 0.3f, 0.2f}, ExpectedOutput{0.5f, 0.057143f, 0.428571f, 0.285714f}},
    // Only negative ...
    {Input{-0.1f, -0.8f, -0.2f, -0.7f}, ExpectedOutput{-0.1f, -0.8f, -0.285714f, -0.8f}},
    {Input{-0.1f, -0.8f, -0.5f, -0.2f}, ExpectedOutput{-0.1f, -0.8f, -0.714286f, -0.285714f}},
    {Input{-0.6f, -0.04f, -0.3f, -0.9f}, ExpectedOutput{-0.6f, -0.057143f, -0.428571f, -0.9f}},
    {Input{-0.5f, -0.04f, -0.3f, -0.2f}, ExpectedOutput{-0.5f, -0.057143f, -0.428571f, -0.285714f}},
    // Mixed - positives higher ...
    {Input{-0.1f, 0.8f, -0.2f, 0.7f}, ExpectedOutput{-0.1f, 0.8f, -0.285714f, 0.8f}},
    {Input{-0.1f, 0.8f, 0.5f, -0.6f}, ExpectedOutput{-0.1f, 0.8f, 0.714286f, -0.8f}},
    {Input{0.6f, -0.04f, -0.3f, 0.9f}, ExpectedOutput{0.6f, -0.057143f, -0.428571f, 0.9f}},
    {Input{0.5f, -0.04f, -0.3f, -0.6f}, ExpectedOutput{0.5f, -0.057143f, -0.428571f, -0.6f}},
    // Mixed - negatives lower ...
    {Input{-0.9f, 0.4f, -0.2f, 0.64f}, ExpectedOutput{-0.9f, 0.571429f, -0.285714f, 0.9f}},
    {Input{-0.6f, 0.4f, 0.5f, -0.3f}, ExpectedOutput{-0.6f, 0.571429f, 0.6f, -0.428571f}},
    {Input{0.2f, -0.4f, -0.5f, 0.1f}, ExpectedOutput{0.2f, -0.4f, -0.5f, 0.142857f}},
    {Input{0.2f, -0.4f, -0.3f, -0.6f}, ExpectedOutput{0.2f, -0.4f, -0.4f, -0.6f}},
    // More or less constant signal ...
    {Input{0.4f, 0.42f, 0.38f, 0.36f}, ExpectedOutput{0.4f, 0.42f, 0.42f, 0.42f}},
    {Input{-0.4f, -0.42f, -0.38f, -0.36f}, ExpectedOutput{-0.4f, -0.42f, -0.42f, -0.42f}},
    // Around zero ...
    {Input{0.001f, -0.02f, 0.01f, 0.008f}, ExpectedOutput{0.001f, -0.02f, 0.014286f, 0.011429f}},
};

} // namespace HardClippingAtBeginningTestData

namespace HardClippingContinuousTestData
{
static constexpr unsigned WindowSize{4};
static constexpr float threshold{0.6};

using Input1 = std::vector<float>;
using Input2 = std::vector<float>;
using ExpectedOutput = std::vector<float>;

using DistortionUnderTest = Distortion<WindowSize>;

using TestData = std::tuple<Input1, Input2, ExpectedOutput>;

static const std::initializer_list<TestData> test_data{
    // clang-format off
    {Input1{-0.1f, 0.2f, 0.7f, 0.8f}, Input2{0.3f, -0.2f, 0.7f, -0.1f}, ExpectedOutput{0.5f, -0.333333f, 0.8f, -0.166667f}},
    {Input1{0.8f, 0.7f, -0.1f, 0.55f}, Input2{0.2f, -0.2f, 0.9f, 0.7f}, ExpectedOutput{0.333333f, -0.333333f, 0.9f, 0.9f}},
    {Input1{0.8f, 0.7f, -0.1f, -0.6f}, Input2{0.38f, -0.38f, 0.9f, 0.7f}, ExpectedOutput{0.633333f, -0.6f, 0.9f, 0.9f}},
    {Input1{0.8f, -0.7f, -0.1f, -0.6f}, Input2{0.45f, -0.38f, 0.9f, 0.7f}, ExpectedOutput{0.7f, -0.6f, 0.9f, 0.9f}},
    {Input1{0.8f, -0.2f, -0.15f, 0.6f}, Input2{0.45f, -0.26f, 0.8f, 0.2f}, ExpectedOutput{0.6f, -0.433333f, 0.8f, 0.333333f}},
    {Input1{0.8f, -0.2f, -0.1f, -0.7f}, Input2{0.45f, -0.2f, 0.8f, -0.3f}, ExpectedOutput{0.7f, -0.333333f, 0.8f, -0.5f}},
    {Input1{0.8f, -0.2f, -0.1f, -0.7f}, Input2{0.5f, -0.2f, -0.8f, -0.3f}, ExpectedOutput{0.7f, -0.333333f, -0.8f, -0.5f}},
    {Input1{0.8f, -0.2f, -0.1f, 0.7f}, Input2{0.5f, -0.2f, -0.8f, -0.3f}, ExpectedOutput{0.7f, -0.333333f, -0.8f, -0.5f}},
    {Input1{-0.9f, -0.6f, -0.3f, -0.1f}, Input2{0.1f, 0.3f, 0.6f, 0.9f}, ExpectedOutput{0.166667f, 0.3f, 0.6f, 0.9f}},
    {Input1{0.9f, 0.6f, 0.3f, 0.1f}, Input2{-0.1f, -0.3f, -0.6f, -0.9f}, ExpectedOutput{-0.166667f, -0.3f, -0.6f, -0.9f}},
    {Input1{0.9f, 0.6f, 0.3f, 0.1f}, Input2{-0.1f, -0.3f, -0.6f, -0.9f}, ExpectedOutput{-0.166667f, -0.3f, -0.6f, -0.9f}},
    {Input1{0.1f, -0.3f, -0.1f, 0.2f}, Input2{0.2f, -0.1f, -0.3f, -0.9f}, ExpectedOutput{0.3f, -0.166667f, -0.3f, -0.9f}},
    {Input1{0.1f, -0.3f, -0.1f, 0.2f}, Input2{0.2f, -0.1f, -0.3f, -0.9f}, ExpectedOutput{0.3f, -0.166667f, -0.3f, -0.9f}},
    {Input1{0.1f, 0.3f, 0.1f, -0.2f}, Input2{-0.2f, 0.1f, 0.3f, 0.9f}, ExpectedOutput{-0.3f, 0.166667f, 0.3f, 0.9f}},
    // clang-format on
};
} // namespace HardClippingContinuousTestData

template<typename T>
static auto adapt_to_guitar_effect_input(const std::vector<float>& input)
{
    T input_adapted;
    std::copy(std::begin(input), std::end(input), std::begin(input_adapted));
    return input_adapted;
}

static std::vector<float> generate_random_batch_of_samples(unsigned batch_size)
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<float> dis{-1, 1};

    std::vector<float> result(batch_size);
    std::generate(std::begin(result), std::end(result), [&]() { return dis(rng); });
    return result;
}

TEST_CASE("Distortion applies hard clipping", "[distortion]")
{
    SECTION("Signal is hard-clipped at the beginning")
    {
        using namespace HardClippingAtBeginningTestData;

        DistortionUnderTest distortion{threshold};

        const auto& [input, expected_output]{GENERATE(Catch::Generators::table<Input, ExpectedOutput>(test_data))};
        auto input_adapted{adapt_to_guitar_effect_input<DistortionUnderTest::BatchOfSamples>(input)};

        auto result{distortion.apply(std::move(input_adapted))};
        REQUIRE_THAT(result, EqualsSamples(expected_output));
    }

    SECTION("Signal is hard-clipped continuously")
    {
        using namespace HardClippingContinuousTestData;

        DistortionUnderTest distortion{threshold};

        const auto& [input1, input2, expected_output]{
            GENERATE(Catch::Generators::table<Input1, Input2, ExpectedOutput>(test_data))};
        auto input1_adapted{adapt_to_guitar_effect_input<DistortionUnderTest::BatchOfSamples>(input1)};
        auto input2_adapted{adapt_to_guitar_effect_input<DistortionUnderTest::BatchOfSamples>(input2)};

        distortion.apply(std::move(input1_adapted));
        auto result{distortion.apply(std::move(input2_adapted))};
        REQUIRE_THAT(result, EqualsSamples(expected_output));
    }

    SECTION("Signal is hard-clipped in the long run")
    {
        static constexpr unsigned WindowSize{4};
        static constexpr float threshold{0.8};

        using DistortionUnderTest = Distortion<WindowSize>;
        DistortionUnderTest distortion{threshold};

        DistortionUnderTest::BatchOfSamples input1{0.4f, 0.2f, -0.2f, -0.5f};
        DistortionUnderTest::BatchOfSamples input2{0.2f, -0.2f, -0.3f, 0.5f};
        DistortionUnderTest::BatchOfSamples input3{0.4f, 0.2f, -0.1f, -0.5f};

        distortion.apply(std::move(input1));
        distortion.apply(std::move(input2));
        auto result{distortion.apply(std::move(input3))};
        REQUIRE_THAT(result, EqualsSamples(std::vector<float>{0.5f, 0.25f, -0.125f, -0.5f}));
    }

    SECTION("Threshold cannot be negative")
    {
        static constexpr unsigned WindowSize{4};
        static constexpr float threshold{-0.001};

        using DistortionUnderTest = Distortion<WindowSize>;

        REQUIRE_THROWS_AS(DistortionUnderTest{threshold}, GuitarEffect<DistortionUnderTest::BatchOfSamples>::Error);
    }

    SECTION("Various thresholds are applied")
    {
        SECTION("Threshold equal to 0.5")
        {
            static constexpr unsigned WindowSize{4};
            static constexpr float threshold{0.5};

            using DistortionUnderTest = Distortion<WindowSize>;
            DistortionUnderTest distortion{threshold};

            auto result{distortion.apply({0.4f, 0.3f, 0.18f, -0.21f})};
            REQUIRE_THAT(result, EqualsSamples(std::vector<float>{0.4f, 0.4f, 0.36f, -0.4f}));
        }

        SECTION("Threshold equal to 0.1")
        {
            static constexpr unsigned WindowSize{4};
            static constexpr float threshold{0.1};

            using DistortionUnderTest = Distortion<WindowSize>;
            DistortionUnderTest distortion{threshold};

            auto result{distortion.apply({0.4f, 0.2f, 0.001f, -0.05f})};
            REQUIRE_THAT(result, EqualsSamples(std::vector<float>{0.4f, 0.4f, 0.01f, -0.4f}));
        }
    }

    SECTION("Various window sizes are handled")
    {
        SECTION("Window size is 4")
        {
            static constexpr unsigned WindowSize{4};
            static constexpr float threshold{0.5};

            using DistortionUnderTest = Distortion<WindowSize>;
            DistortionUnderTest distortion{threshold};

            auto result{distortion.apply({0.2f, 0.3f, -0.17f, -0.21f})};
            REQUIRE_THAT(result, EqualsSamples(std::vector<float>{0.2f, 0.3f, -0.3f, -0.3f}));
        }

        SECTION("Window size is 8")
        {
            static constexpr unsigned WindowSize{8};
            static constexpr float threshold{0.5};

            using DistortionUnderTest = Distortion<WindowSize>;
            DistortionUnderTest distortion{threshold};

            auto result{distortion.apply({0.4f, 0.3f, 0.18f, -0.2f, 0.5f, -0.1f, -0.2f, 0.6f})};
            REQUIRE_THAT(result, EqualsSamples(std::vector<float>{0.4f, 0.4f, 0.36f, -0.4f, 0.5f, -0.2f, -0.4f, 0.6f}));
        }
    }
}

TEST_CASE("Distortion benchmark", "[.][distortion_benchmark][!benchmark]")
{
    static constexpr unsigned WindowSize{64};
    static constexpr float threshold{0.7};

    using DistortionUnderTest = Distortion<WindowSize>;
    DistortionUnderTest distortion{threshold};

    auto random_samples{generate_random_batch_of_samples(64)};

    constexpr unsigned number_of_tests{10240};

    std::cout << "\r\nBenchmarking Distortion for " << number_of_tests << " iterations and window size: " << WindowSize
              << std::endl;

    BENCHMARK("Applying distortion")
    {
        auto i{number_of_tests};
        while (i--)
        {
            auto random_samples_adapted{
                adapt_to_guitar_effect_input<DistortionUnderTest::BatchOfSamples>(random_samples)};
            distortion.apply(std::move(random_samples_adapted));
        }
    };
}
