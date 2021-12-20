/**
 * @file    button.cpp
 * @brief   Implements Button class.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "button.hpp"

#include "gpio.h"

#include "os_waiters.hpp"

bool Button::wait_milliseconds_for_press(unsigned int timeout_milliseconds)
{
    auto wait_for_falling_edge{[=]() {
        unsigned polling_interval_ms{10};
        bool is_press_detected{false};
        for (unsigned time_ms{0}; time_ms < timeout_milliseconds; time_ms += polling_interval_ms)
        {
            if (auto is_pin_cleared{!LL_GPIO_IsInputPinSet(BUTTON0_GPIO_Port, BUTTON0_Pin)}; is_pin_cleared)
            {
                is_press_detected = true;
                break;
            }
            os::wait_milliseconds(polling_interval_ms);
        }
        return is_press_detected;
    }};

    auto wait_for_rising_edge{[]() {
        do
        {
            if (auto is_pin_set{LL_GPIO_IsInputPinSet(BUTTON0_GPIO_Port, BUTTON0_Pin)}; is_pin_set)
                break;
        } while (true);
    }};

    auto is_press_detected{wait_for_falling_edge()};
    if (is_press_detected)
        wait_for_rising_edge();
    return is_press_detected;
}
