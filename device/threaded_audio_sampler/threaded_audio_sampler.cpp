/**
 * @file    threaded_audio_sampler.cpp
 * @brief   Implements the ThreadedAudioSampler.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include <algorithm>
#include <chrono>
#include <cstring>
#include <limits>
#include <string>
#include <string_view>

#include "os_waiters.hpp"
#include "threaded_audio_sampler.hpp"

#include "adc.h"

#include "cmsis_os2.h"
#include "jungles_os_helpers/freertos/poller.hpp"

static std::function<void(uint16_t raw_sample)> adc1_interrupt_handler;

ThreadedAudioSampler::ThreadedAudioSampler() :
    worker{"AudioSampler", 1024, osPriorityNormal},
    active{[this](auto&& v) { this->handle_new_sample(v); }, message_pump, worker}
{
    adc1_interrupt_handler = [this](uint16_t raw_sample) {
        this->active.send(std::move(raw_sample));
    };

    MX_ADC1_Init();
}

ThreadedAudioSampler::~ThreadedAudioSampler()
{
    // TODO: deregister adc1_interrupt_handler
    LL_ADC_DeInit(ADC1);
}

void ThreadedAudioSampler::set_on_batch_of_samples_received_handler(Handler handler)
{
    on_sample_received_handler = std::move(handler);
}

void ThreadedAudioSampler::start()
{
    if (is_started)
        return;

    calibrate();
    enable();

    is_started = true;
}

void ThreadedAudioSampler::stop()
{
    if (!is_started)
        return;

    // TODO: implement
    // TODO: synchronize ending or prevent from calling the IT handler, by disabling IT and clearing the IT flag.

    is_started = false;
}

void ThreadedAudioSampler::calibrate() const
{
    auto wait_for_calibration_to_end_or_throw_if_failure{[]() {
        auto poller{jungles::freertos::make_poller<1, 5>()};
        auto calibration_finished{poller.poll([]() { return !LL_ADC_IsCalibrationOnGoing(ADC1); })};
        if (!calibration_finished)
            throw Error{"Calibration error!"};
    }};

    auto delay_before_enabling{[]() {
        os::wait(std::chrono::milliseconds{200});
    }};

    LL_ADC_StartCalibration(ADC1, LL_ADC_SINGLE_ENDED);
    wait_for_calibration_to_end_or_throw_if_failure();
    delay_before_enabling();
}

void ThreadedAudioSampler::enable()
{
    auto wait_for_adc_to_start_or_throw_if_failure{[]() {
        auto poller{jungles::freertos::make_poller<1, 5>()};
        auto calibration_finished{poller.poll([]() { return LL_ADC_IsActiveFlag_ADRDY(ADC1) != 0; })};
        if (!calibration_finished)
            throw Error{"Failed to start!"};
    }};

    LL_ADC_EnableIT_EOC(ADC1);
    LL_ADC_Enable(ADC1);
    wait_for_adc_to_start_or_throw_if_failure();
    LL_ADC_REG_StartConversion(ADC1);
}

void ThreadedAudioSampler::handle_new_sample(uint16_t raw_sample)
{
    // TODO: to make it faster, use DMA and here receive multiple samples instead of single one, but send
    // samples one-by-one to the callback.

    static constexpr unsigned reference_voltage_in_millivolts{3300};
    unsigned sample_in_millivolts{
        __LL_ADC_CALC_DATA_TO_VOLTAGE(reference_voltage_in_millivolts, raw_sample, LL_ADC_RESOLUTION_12B)};

    static constexpr unsigned dc_offset_in_millivolts{1650};

    static constexpr unsigned maximum_positive_amplitude{reference_voltage_in_millivolts - dc_offset_in_millivolts};
    static constexpr unsigned maximum_negative_amplitude{dc_offset_in_millivolts - 0};
    static constexpr unsigned maximum_amplitude{std::max(maximum_positive_amplitude, maximum_negative_amplitude)};

    static constexpr int normalizing_factor_to_int_max{std::numeric_limits<int>::max() / maximum_amplitude};

    auto sample_in_millivolts_without_dc_offset{static_cast<int>(sample_in_millivolts - dc_offset_in_millivolts)};
    int sample{sample_in_millivolts_without_dc_offset * normalizing_factor_to_int_max};

    if (on_sample_received_handler)
        on_sample_received_handler(std::begin(sample_buffer), std::end(sample_buffer));
}

extern "C" void ADC1_IRQHandler(void)
{
    auto is_conversion_finished{[]() {
        return LL_ADC_IsActiveFlag_EOC(ADC1);
    }};

    auto clear_conversion_flag{[]() {
        LL_ADC_ClearFlag_EOC(ADC1);
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
