/**
 * @file        test_basic_windowed_distortion_with_memory.cpp
 * @brief       Tests the basic windowed distortion with memory.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "catch2/catch_approx.hpp"
#include "catch2/catch_test_macros.hpp"
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

TEST_CASE("Basic windowed distortion applies hard clipping", "[distortion]")
{
    SECTION("Signal is hard-clipped at the beginning")
    {
        constexpr unsigned window_size{4};
        constexpr float threshold{0.7};

        BasicWindowedDistortionWithMemory<window_size> distortion{threshold};

        auto result{distortion.apply({-0.1f, 0.2f, 0.7f, 0.8f})};
        std::vector<float> expected{-0.07f, 0.14f, 0.49f, 0.56f};

        REQUIRE_THAT(result, EqualsSamples(expected));
    }

    // Scenarios:
    // lower, higher, lower, higher
    // lower, higher, higher, lower
    // higher, lower, lower, higher
    // higher, lower, lower, lower
    // ...

    SECTION("Signal is hard-clipped continuously")
    {
    }

    SECTION("Threshold cannot be negative")
    {
    }
}
