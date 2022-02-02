/**
 * @file        batch_of_samples.hpp
 * @brief       Defines batch of samples.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef BATCH_OF_SAMPLES_HPP
#define BATCH_OF_SAMPLES_HPP

#include <array>
#include <experimental/array>
#include <memory>

template<unsigned Size>
using SampleBufferTemplate = std::array<float, Size>;

template<unsigned Size>
struct BatchOfSamplesTemplate
{
    using SampleBuffer = SampleBufferTemplate<Size>;
    using PointerToBatch = std::unique_ptr<SampleBuffer>;
    using value_type = typename SampleBuffer::value_type;
    using iterator = typename SampleBuffer::iterator;

    constexpr BatchOfSamplesTemplate() : pointer_to_batch{std::make_unique<SampleBuffer>()}
    {
    }

    template<typename... value_types>
    constexpr BatchOfSamplesTemplate(value_types&&... values) :
        pointer_to_batch{
            std::make_unique<SampleBuffer>(std::experimental::make_array(std::forward<value_types>(values)...))}
    {
    }

    constexpr value_type& operator[](unsigned index)
    {
        return *pointer_to_batch[index];
    }

    constexpr const value_type& operator[](unsigned index) const
    {
        return *pointer_to_batch[index];
    }

    constexpr auto begin() noexcept
    {
        return pointer_to_batch->begin();
    }

    constexpr auto begin() const noexcept
    {
        return pointer_to_batch->begin();
    }

    constexpr auto cbegin() const noexcept
    {
        return pointer_to_batch->cbegin();
    }

    constexpr auto end() noexcept
    {
        return pointer_to_batch->end();
    }

    constexpr auto end() const noexcept
    {
        return pointer_to_batch->end();
    }

    constexpr auto cend() const noexcept
    {
        return pointer_to_batch->cend();
    }

    PointerToBatch pointer_to_batch;
};

#endif /* BATCH_OF_SAMPLES_HPP */
