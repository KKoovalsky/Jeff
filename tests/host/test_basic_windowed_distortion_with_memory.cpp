/**
 * @file        test_basic_windowed_distortion_with_memory.cpp
 * @brief       Tests the basic windowed distortion with memory.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "catch2/catch_approx.hpp"
#include "catch2/catch_test_macros.hpp"
#include "catch2/generators/catch_generators.hpp"
#include "catch2/generators/catch_generators_range.hpp"
#include "catch2/matchers/catch_matchers.hpp"
#include "catch2/matchers/catch_matchers_templated.hpp"

#include "basic_windowed_distortion_with_memory.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <utility>

bool is_sample_equal(float a, float b)
{
    // Samples are within range < -1 ; 1 > so we assume constant epsilon.
    constexpr float epsilon{1e-7};
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

using Distortion = BasicWindowedDistortionWithMemory<WindowSize>;

using TestData = std::tuple<Input, ExpectedOutput>;

static const std::initializer_list<TestData> test_data{
    {Input{-0.1f, 0.2f, 0.7f, 0.8f}, ExpectedOutput{-0.07f, 0.14f, 0.49f, 0.56f}},

    // Scenarios more or less follow patterns:
    // lower, higher, lower, higher
    // lower, higher, higher, lower
    // higher, lower, lower, higher
    // higher, lower, lower, lower
    // ...
    // Only positive values ...
    {Input{0.1f, 0.8f, 0.2f, 0.7f}, ExpectedOutput{0.07f, 0.56f, 0.2f, 0.56f}},
    {Input{0.1f, 0.8f, 0.5f, 0.2f}, ExpectedOutput{0.07f, 0.56f, 0.5f, 0.2f}},
    {Input{0.6f, 0.04f, 0.3f, 0.9f}, ExpectedOutput{0.42f, 0.04f, 0.3f, 0.63f}},
    {Input{0.5f, 0.04f, 0.3f, 0.2f}, ExpectedOutput{0.35f, 0.04f, 0.3f, 0.2f}},
    // Only negative ...
    {Input{-0.1f, -0.8f, -0.2f, -0.7f}, ExpectedOutput{-0.07f, -0.56f, -0.2f, -0.56f}},
    {Input{-0.1f, -0.8f, -0.5f, -0.2f}, ExpectedOutput{-0.07f, -0.56f, -0.5f, -0.2f}},
    {Input{-0.6f, -0.04f, -0.3f, -0.9f}, ExpectedOutput{-0.42f, -0.04f, -0.3f, -0.63f}},
    {Input{-0.5f, -0.04f, -0.3f, -0.2f}, ExpectedOutput{-0.35f, -0.04f, -0.3f, -0.2f}},
    // Mixed - positives higher ...
    {Input{-0.1f, 0.8f, -0.2f, 0.7f}, ExpectedOutput{-0.07f, 0.56f, -0.2f, 0.56f}},
    {Input{-0.1f, 0.8f, 0.5f, -0.6f}, ExpectedOutput{-0.07f, 0.56f, 0.5f, -0.56f}},
    {Input{0.6f, -0.04f, -0.3f, 0.9f}, ExpectedOutput{0.42f, -0.04f, -0.3f, 0.63f}},
    {Input{0.5f, -0.04f, -0.3f, -0.6f}, ExpectedOutput{0.35f, -0.04f, -0.3f, -0.42f}},
    // Mixed - negatives lower ...
    {Input{-0.9f, 0.4f, -0.2f, 0.64f}, ExpectedOutput{-0.63f, 0.4f, -0.2f, 0.63f}},
    {Input{-0.6f, 0.4f, 0.5f, -0.3f}, ExpectedOutput{-0.42f, 0.4f, 0.42f, -0.3f}},
    {Input{0.2f, -0.4f, -0.5f, 0.1f}, ExpectedOutput{0.14f, -0.28f, -0.35f, 0.1f}},
    {Input{0.2f, -0.4f, -0.3f, -0.6f}, ExpectedOutput{0.14f, -0.28f, -0.28f, -0.42f}},
    // More or less constant signal ...
    {Input{0.4f, 0.42f, 0.38f, 0.36f}, ExpectedOutput{0.28f, 0.294f, 0.294f, 0.294f}},
    {Input{-0.4f, -0.42f, -0.38f, -0.36f}, ExpectedOutput{-0.28f, -0.294f, -0.294f, -0.294f}},
    // Around zero ...
    {Input{0.001f, -0.02f, 0.01f, 0.008f}, ExpectedOutput{0.0007f, -0.014f, 0.01f, 0.008f}},
};

} // namespace HardClippingAtBeginningTestData

TEST_CASE("Basic windowed distortion applies hard clipping", "[distortion]")
{
    SECTION("Signal is hard-clipped at the beginning")
    {
        using namespace HardClippingAtBeginningTestData;

        Distortion distortion{threshold};

        const auto& [input, expected_output]{GENERATE(Catch::Generators::table<Input, ExpectedOutput>(test_data))};
        Distortion::BatchOfSamples input_adapted;
        std::copy(std::begin(input), std::end(input), std::begin(input_adapted));

        auto result{distortion.apply(std::move(input_adapted))};
        REQUIRE_THAT(result, EqualsSamples(expected_output));
    }

    SECTION("Signal is hard-clipped continuously")
    {
    }

    SECTION("Threshold cannot be negative")
    {
    }

    SECTION("Signal is hard-clipped for the default type")
    {
    }

    SECTION("Various thresholds are applied")
    {
    }

    SECTION("Various window sized are handled")
    {
    }
}
