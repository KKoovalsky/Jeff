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

#include "batch_of_samples.hpp"
#include "guitar_effect.hpp"

template<unsigned WindowSize>
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
        std::fill(std::begin(previous_batch), std::end(previous_batch), 0.0f);
    }

    BatchOfSamples apply(BatchOfSamples samples) override
    {
        BatchOfSamples output_samples;

        auto computation_window{merge(previous_batch, samples)};
        auto output_samples_it{std::begin(output_samples)};

        constexpr auto first_new_sample_index{ComputationWindowSize / 2};
        for (unsigned i{first_new_sample_index}; i < ComputationWindowSize; ++i)
        {
            auto window_begin_it{std::next(std::begin(computation_window), i - WindowSize + 1)};
            auto window_end_it{std::next(std::begin(computation_window), i + 1)};
            // TODO: MaxElement shall be template function and Fabs as well.
            auto window_absolute_maximum_it{std::max_element(
                window_begin_it, window_end_it, [](auto l, auto r) { return std::fabs(l) < std::fabs(r); })};
            auto window_absolute_maximum{std::fabs(*window_absolute_maximum_it)};

            auto window_threshold{window_absolute_maximum * threshold};
            auto input_sample{computation_window[i]};
            *output_samples_it++ = clamp(input_sample, window_threshold, -window_threshold);
        }

        previous_batch = std::move(samples);
        return output_samples;
    }

  private:
    ComputationWindow merge(const BatchOfSamples& batch1, const BatchOfSamples& batch2) const noexcept
    {
        ComputationWindow computation_window;
        auto half_window_it{std::next(std::begin(computation_window), WindowSize)};
        std::copy(std::begin(batch1), std::end(batch1), std::begin(computation_window));
        std::copy(std::begin(batch2), std::end(batch2), half_window_it);
        return computation_window;
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
    BatchOfSamples previous_batch;
};

#endif /* BASIC_WINDOWED_DISTORTION_WITH_MEMORY_HPP */
