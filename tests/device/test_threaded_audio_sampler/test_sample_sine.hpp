/**
 * @file        test_sample_sine.hpp
 * @brief       Declares helpers to test input sine signal detection.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef TEST_INPUT_SINE_HPP
#define TEST_INPUT_SINE_HPP

/**
 * @brief Test which checks whether input signal is a sine wave with specified frequency.
 *
 * This test shall be used when checking for a specific sine wave at input. The input is the analog input used normally
 * for the audio input. The intention of use is to verify whether the input audio signal is correctly sampled.
 *
 * The input signal will be sampled over few periods. The period is computed from the input signal_frequency_hz.
 * Then the corresponding, expected sine wave will be generated and compared against the sampled signal.
 * Cross-correlation function is used to compare the signals. If for at least one offset, the cross-correlation factor
 * is bigger than 0.9 then the test is successful.
 *
 * The test expects that the input signal is applied before the test runs. One shall apply it using some function
 * generator.
 *
 * The output of the test will be printed using the serial logger.
 *
 * The input signal shall have 1.6V amplitude.
 *
 * @param signal_frequency_hz Expected input signal frequency. Shall not be bigger than 20 kHz.
 */
void assert_input_signal_matches_sine(unsigned signal_frequency_hz);

#endif /* TEST_INPUT_SINE_HPP */
