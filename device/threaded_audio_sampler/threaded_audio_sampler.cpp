/**
 * @file    threaded_audio_sampler.cpp
 * @brief   Implements the ThreadedAudioSampler.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include <cstring>
#include <string>
#include <string_view>

#include "threaded_audio_sampler.hpp"

#include "adc.h"

static std::function<void(uint16_t raw_sample)> adc1_interrupt_handler;

ThreadedAudioSampler::ThreadedAudioSampler()
{
}

ThreadedAudioSampler::~ThreadedAudioSampler()
{
}

void ThreadedAudioSampler::set_on_sample_received_handler(Handler handler)
{
    on_sample_received_handler = std::move(handler);
}

void ThreadedAudioSampler::start()
{
    if (is_started)
        return;

    is_started = true;
}

void ThreadedAudioSampler::stop()
{
    if (!is_started)
        return;

    is_started = false;
}

}

extern "C" void ADC1_IRQHandler(void)
{
    auto is_conversion_finished{[]() {
        // TODO: check if LL_ADC_IsActiveFlag_EOC() shall be used instead (end of conversion).
        // Currently we check for End of Sequence.
        return LL_ADC_IsActiveFlag_EOS(ADC1);
    }};

    auto clear_conversion_flag{[]() {
        LL_ADC_ClearFlag_EOS(ADC1);
    }};

    if (is_conversion_finished())
    {
        clear_conversion_flag();
        uint16_t raw_sample{LL_ADC_REG_ReadConversionData12(ADC1)};

        if (adc1_interrupt_handler)
        {
            adc1_interrupt_handler(raw_sample);
        }
    }

    // TODO: handle errors from LL_ADC_IsActiveFlag_OVR(ADC1) (overrun detection).
}

ThreadedAudioSampler::Error::Error(const char* message)
{
    static constexpr std::string_view prefix{"ThreadedAudioSampler Error: "};

    exception_message.reserve(prefix.length() + std::strlen(message));
    exception_message.append(prefix);
    exception_message.append(message);
}

const char* ThreadedAudioSampler::Error::what() const noexcept
{
    return exception_message.c_str();
}
