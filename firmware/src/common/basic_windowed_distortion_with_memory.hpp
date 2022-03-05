/**
 * @file        basic_windowed_distortion_with_memory.hpp
 * @brief       Implements simple distortion with moving window and memory (saving previous samples for calculation).
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef BASIC_WINDOWED_DISTORTION_WITH_MEMORY_HPP
#define BASIC_WINDOWED_DISTORTION_WITH_MEMORY_HPP

#include <algorithm>
#include <array>
#include <cmath>
#include <concepts>
#include <functional>
#include <iterator>
#include <set>

#include "ring_buffer.hpp"

#include "batch_of_samples.hpp"
#include "guitar_effect.hpp"

namespace default_implementation
{

template<unsigned WindowSize>
using BatchOfSamplesConstIterator = typename BatchOfSamplesTemplate<WindowSize>::const_iterator;

template<unsigned WindowSize>
static inline BatchOfSamplesConstIterator<WindowSize> max_element(BatchOfSamplesConstIterator<WindowSize> beg,
                                                                  BatchOfSamplesConstIterator<WindowSize> end)
{
    return std::max_element(beg, end);
};

static inline float calculate_absolute(float v)
{
    return std::fabs(v);
}

} // namespace default_implementation

template<unsigned WindowSize,
         const auto& MaximumFinder = default_implementation::max_element<WindowSize>,
         const auto& AbsoluteCalculator = default_implementation::calculate_absolute>
class BasicWindowedDistortionWithMemory : public GuitarEffect<BatchOfSamplesTemplate<WindowSize>>
{
  public:
    using BatchOfSamples = BatchOfSamplesTemplate<WindowSize>;

    explicit BasicWindowedDistortionWithMemory(float threshold) :
        threshold{threshold}, normalization_factor{1 / threshold}
    {
        if (threshold < 0.0f)
            throw Error{"Threshold must not be negative"};

        for (unsigned i{0}; i < WindowSize; ++i)
            links_to_sorted_absolute_values_of_samples.push(sorted_absolute_values_of_samples.insert(0.f));
    }

    BatchOfSamples apply(BatchOfSamples samples) override
    {
        BatchOfSamples output_samples;
        auto current_batch_absolute_values{to_absolute(samples)};

        auto output_samples_it{std::begin(output_samples)};
        auto samples_it{std::begin(samples)};
        auto absolute_samples_it{std::begin(current_batch_absolute_values)};

        while (samples_it != std::end(samples))
        {
            auto sample{*samples_it++};
            auto sample_absolute{*absolute_samples_it++};

            auto old_absolute_window_begin_it{links_to_sorted_absolute_values_of_samples.pop()};
            sorted_absolute_values_of_samples.erase(old_absolute_window_begin_it);

            auto new_absolute_window_end_it{sorted_absolute_values_of_samples.insert(sample_absolute)};
            links_to_sorted_absolute_values_of_samples.push(new_absolute_window_end_it);

            auto window_absolute_maximum{*sorted_absolute_values_of_samples.begin()};
            auto window_threshold{window_absolute_maximum * threshold};
            *output_samples_it++ = clamp(sample, window_threshold, -window_threshold);
        }

        normalize(output_samples);
        return output_samples;
    }

  private:
    struct Error : GuitarEffect<BatchOfSamples>::Error
    {
        explicit Error(const char* message) : message{message}
        {
        }

        [[nodiscard]] const char* what() const noexcept override
        {
            return message;
        }

        const char* message;
    };

    BatchOfSamples to_absolute(const BatchOfSamples& samples) const noexcept
    {
        BatchOfSamples result;
        std::transform(std::begin(samples), std::end(samples), std::begin(result), AbsoluteCalculator);
        return result;
    }

    [[nodiscard]] float clamp(float value, float max_value, float min_value) const noexcept
    {
        if (value > max_value)
            return max_value;
        else if (value < min_value)
            return min_value;
        else
            return value;
    }

    void normalize(BatchOfSamples& samples) const noexcept
    {
        std::transform(
            std::begin(samples),
            std::end(samples),
            std::begin(samples),
            [normalization_factor = this->normalization_factor](float v) { return v * normalization_factor; });
    }

    float threshold;
    float normalization_factor;

    using Multiset = std::multiset<float, std::greater<float>>;

    Multiset sorted_absolute_values_of_samples;
    jungles::RingBuffer<Multiset::iterator, WindowSize> links_to_sorted_absolute_values_of_samples;
};

#endif /* BASIC_WINDOWED_DISTORTION_WITH_MEMORY_HPP */
