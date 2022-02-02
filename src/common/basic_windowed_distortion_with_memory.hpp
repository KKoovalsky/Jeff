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

namespace detail
{

template<typename InputIt1, typename InputIt2>
static inline auto max_element(InputIt1 beg, InputIt2 end)
{
    return std::max_element(beg, end);
};

static inline float calculate_absolute(float v)
{
    return std::fabs(v);
}

} // namespace detail

template<unsigned WindowSize,
         const auto& MaximumFinder = detail::max_element<typename BatchOfSamplesTemplate<WindowSize>::iterator,
                                                         typename BatchOfSamplesTemplate<WindowSize>::iterator>,
         const auto& AbsoluteCalculator = detail::calculate_absolute>
class BasicWindowedDistortionWithMemory : public GuitarEffect<BatchOfSamplesTemplate<WindowSize>>
{
  public:
    using BatchOfSamples = BatchOfSamplesTemplate<WindowSize>;

  private:
    static constexpr unsigned ComputationWindowSize{WindowSize * 2};
    using ComputationWindow = std::array<typename BatchOfSamples::value_type, ComputationWindowSize>;

  public:
    explicit BasicWindowedDistortionWithMemory(float threshold) : threshold{threshold}
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
        auto input_samples_it{std::begin(samples)};

        constexpr auto first_new_sample_index{ComputationWindowSize / 2};
        for (unsigned i{first_new_sample_index}; i < ComputationWindowSize; ++i)
        {
            auto window_begin_it{std::next(std::begin(computation_window), i - WindowSize + 1)};
            auto window_end_it{std::next(std::begin(computation_window), i + 1)};
            auto window_absolute_maximum{*MaximumFinder(window_begin_it, window_end_it)};

            auto window_threshold{window_absolute_maximum * threshold};
            auto input_sample{*input_samples_it++};
            *output_samples_it++ = clamp(input_sample, window_threshold, -window_threshold);
        }

        previous_batch_absolute_values = std::move(current_batch_absolute_values);
        return output_samples;
    }

  private:
    struct Error : GuitarEffect<BatchOfSamples>::Error
    {
        explicit Error(const char* message) : message{message}
        {
        }

        [[nodiscard]] virtual const char* what() const noexcept override
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

    float clamp(float value, float max_value, float min_value) const noexcept
    {
        if (value > max_value)
            return max_value;
        else if (value < min_value)
            return min_value;
        else
            return value;
    }

    float threshold;
    BatchOfSamples previous_batch_absolute_values;
};

#endif /* BASIC_WINDOWED_DISTORTION_WITH_MEMORY_HPP */
