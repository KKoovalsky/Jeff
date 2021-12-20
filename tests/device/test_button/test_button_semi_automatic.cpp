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
void test_one_instance_can_run_at_a_time();
/** @} */ // End of group TestButtonSemiAutomatic

// =====================================================================================================================
// Tests' definition
// =====================================================================================================================

using namespace std::chrono_literals;

void test_single_press_is_awaited()
{
    SerialLogger logger;
    logger.log(LogLevel::info) << "Press the button ...";
    Button button;
    auto is_button_pressed{button.wait_for_press(15s)};
    TEST_ASSERT_TRUE(is_button_pressed);
}

void test_multiple_presses_are_awaited()
{
    TEST_IGNORE();
}

void test_multiple_presses_are_awaited_for_multiple_instances()
{
    TEST_IGNORE();
}

void test_one_instance_can_run_at_a_time()
{
    TEST_IGNORE();
}
