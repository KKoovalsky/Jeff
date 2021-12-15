/**
 * @file    test_threaded_audio_sampler.cpp
 * @brief   Tests the ThreadedAudioSampler.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include <chrono>

#include "os_waiters.hpp"
#include "threaded_audio_sampler.hpp"

#include "jungles_os_helpers/freertos/flag.hpp"

#include "unity.h"

/**
 * @defgroup ThreadedAudioSamplerTests     Tests of ThreadedAudioSampler
 * @{
 */
void test_single_batch_of_samples_is_obtained();
void test_multiple_instances_can_be_created_one_after_another();
void test_proper_number_of_samples_is_collected_within_specific_period();
void test_sampling_frequency_is_stable();
void test_cant_create_two_instances();
/** @} */ // End of group ThreadedAudioSamplerTests

// =====================================================================================================================
// Tests' definition
// =====================================================================================================================

void test_single_batch_of_samples_is_obtained()
{
    jungles::freertos::flag samples_received_flag;

    ThreadedAudioSampler sampler;
    sampler.set_on_batch_of_samples_received_handler([&](auto, auto) { samples_received_flag.set(); });
    sampler.start();

    samples_received_flag.wait();
    sampler.stop();

    TEST_ASSERT_TRUE(true);
}

void test_multiple_instances_can_be_created_one_after_another()
{
    TEST_IGNORE();
}

void test_proper_number_of_samples_is_collected_within_specific_period()
{
    jungles::freertos::flag samples_received_flag;

    unsigned samples_received{0};

    ThreadedAudioSampler sampler;
    sampler.set_on_batch_of_samples_received_handler([&](auto, auto) { samples_received++; });
    sampler.start();

    using namespace std::chrono_literals;
    os::wait(1s);
    sampler.stop();

    unsigned samples_received_after_one_second{samples_received};

    TEST_ASSERT_UINT_WITHIN(100, 44100, samples_received_after_one_second);
}

void test_sampling_frequency_is_stable()
{
    TEST_IGNORE();
}

void test_cant_create_two_instances()
{
    TEST_IGNORE();
}
