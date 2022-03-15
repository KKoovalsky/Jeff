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
#include <iterator>

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

  private:
    static constexpr unsigned ComputationWindowSize{WindowSize * 2};
    using ComputationWindow = std::array<typename BatchOfSamples::value_type, ComputationWindowSize>;

  public:
    explicit BasicWindowedDistortionWithMemory(float threshold) :
        threshold{threshold},
        normalization_factor{1 / threshold},
        window_absolute_maximum{0.0f},
        computation_window_absolute_maximum_index{WindowSize - 1}
    {
        if (threshold < 0.0f)
            throw Error{"Threshold must not be negative"};

        std::fill(std::begin(previous_batch_absolute_values), std::end(previous_batch_absolute_values), 0.0f);
    }

    BatchOfSamples apply(BatchOfSamples samples) override
    {
        BatchOfSamples output_samples;
        auto current_batch_absolute_values{to_absolute(samples)};

        auto computation_window{merge(previous_batch_absolute_values, current_batch_absolute_values)};
        auto output_samples_it{std::begin(output_samples)};
        auto input_samples_it{std::cbegin(samples)};

        constexpr auto first_new_sample_index{ComputationWindowSize / 2};
        for (unsigned i{first_new_sample_index}; i < ComputationWindowSize; ++i)
        {
            auto window_begin_idx{i - WindowSize + 1};
            auto absolute_sample{computation_window[i]};

            if (computation_window_absolute_maximum_index < window_begin_idx)
            {
                auto window_begin_it{std::next(std::cbegin(computation_window), window_begin_idx)};
                auto window_end_it{std::next(std::cbegin(computation_window), i + 1)};
                auto window_absolute_maximum_it{MaximumFinder(window_begin_it, window_end_it)};
                computation_window_absolute_maximum_index =
                    std::distance(std::cbegin(computation_window), window_absolute_maximum_it);
                window_absolute_maximum = *window_absolute_maximum_it;
            } else if (absolute_sample > window_absolute_maximum)
            {
                computation_window_absolute_maximum_index = i;
                window_absolute_maximum = absolute_sample;
            }

            auto window_threshold{window_absolute_maximum * threshold};
            auto input_sample{*input_samples_it++};
            *output_samples_it++ = clamp(input_sample, window_threshold, -window_threshold);
        }

        previous_batch_absolute_values = std::move(current_batch_absolute_values);
        computation_window_absolute_maximum_index -= WindowSize;

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

    ComputationWindow merge(const BatchOfSamples& batch1, const BatchOfSamples& batch2) const noexcept
    {
        ComputationWindow computation_window;
        auto half_window_it{std::next(std::begin(computation_window), WindowSize)};
        std::copy(std::begin(batch1), std::end(batch1), std::begin(computation_window));
        std::copy(std::begin(batch2), std::end(batch2), half_window_it);
        return computation_window;
    }

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
    BatchOfSamples previous_batch_absolute_values;

    float window_absolute_maximum;
    unsigned computation_window_absolute_maximum_index;
};

#endif /* BASIC_WINDOWED_DISTORTION_WITH_MEMORY_HPP */
