/**
 * @file    test_sample_sine_20khz.cpp
 * @brief   Tests whether the input signal has 20 kHz
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "test_sample_sine.hpp"

void test_20kHz_sine_wave_sampling()
{
    assert_input_signal_matches_sine(20000);
}
