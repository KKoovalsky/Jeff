/**
 * @file    test_button_semi_automatic.cpp
 * @brief   Tests button with some manual actions involved.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include <chrono>

#include "button.hpp"
#include "serial_logger.hpp"

#include "unity.h"

/**
 * @defgroup TestButtonSemiAutomatic     Tests of the button in semi-automatic fashion.
 * @{
 */
void test_single_press_is_awaited();
void test_multiple_presses_are_awaited();
void test_multiple_presses_are_awaited_for_multiple_instances();
/** @} */ // End of group TestButtonSemiAutomatic

// =====================================================================================================================
// Tests' definition
// =====================================================================================================================

using namespace std::chrono_literals;

void test_single_press_is_awaited()
{
    SerialLogger logger;
    logger.log(LogLevel::info) << "Testing for single, simple button pressing... ";
    logger.log(LogLevel::info) << "Press the button ...";
    Button button;
    auto is_button_pressed{button.wait_for_press(15s)};
    TEST_ASSERT_TRUE(is_button_pressed);
}

void test_multiple_presses_are_awaited()
{
    SerialLogger logger;

    logger.log(LogLevel::info) << "Testing for multiple, simple button pressing in a row... ";

    logger.log(LogLevel::info) << "Press the button once...";

    Button button;

    auto is_button_pressed_first_time{button.wait_for_press(15s)};
    TEST_ASSERT_TRUE(is_button_pressed_first_time);

    logger.log(LogLevel::info) << "Press the button for the second time now...";

    auto is_button_pressed_second_time{button.wait_for_press(15s)};
    TEST_ASSERT_TRUE(is_button_pressed_second_time);
}

void test_multiple_presses_are_awaited_for_multiple_instances()
{
    SerialLogger logger;

    logger.log(LogLevel::info)
        << "Testing for multiple, simple button pressing, for two instances created in a row... ";

    logger.log(LogLevel::info) << "For the first instance...";
    {
        Button button;
        logger.log(LogLevel::info) << "Press the button once...";
        auto is_button_pressed_first_time{button.wait_for_press(15s)};
        TEST_ASSERT_TRUE(is_button_pressed_first_time);
        logger.log(LogLevel::info) << "Press the button for the second time now...";
        auto is_button_pressed_second_time{button.wait_for_press(15s)};
        TEST_ASSERT_TRUE(is_button_pressed_second_time);
    }

    logger.log(LogLevel::info) << "For the second instance...";
    {
        Button button;
        logger.log(LogLevel::info) << "Press the button once...";
        auto is_button_pressed_first_time{button.wait_for_press(15s)};
        TEST_ASSERT_TRUE(is_button_pressed_first_time);
        logger.log(LogLevel::info) << "Press the button for the second time now...";
        auto is_button_pressed_second_time{button.wait_for_press(15s)};
        TEST_ASSERT_TRUE(is_button_pressed_second_time);
    }
}