/**
 * @file    test_threaded_audio_sampler.cpp
 * @brief   Tests the ThreadedAudioSampler.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include <chrono>

#include "os_waiters.hpp"
#include "threaded_audio_sampler.hpp"

#include "jungles_os_helpers/freertos/flag.hpp"
#include "serial_logger.hpp"

#include "unity.h"

#include "sampling_trigger_timer_impl.hpp"

using namespace std::chrono_literals;

/**
 * @defgroup ThreadedAudioSamplerTests     Tests of ThreadedAudioSampler
 * @{
 */
void test_single_batch_of_samples_is_obtained();
void test_multiple_instances_can_be_run_one_after_another();
void test_proper_number_of_samples_is_collected_within_specific_period();
void test_cant_create_two_instances();
/** @} */ // End of group ThreadedAudioSamplerTests

// =====================================================================================================================
// Tests' definition
// =====================================================================================================================

void test_single_batch_of_samples_is_obtained()
{
    jungles::freertos::flag samples_received_flag;

    SamplingTriggerTimerImpl sampling_trigger_timer;
    ThreadedAudioSampler sampler{sampling_trigger_timer};
    sampler.set_on_batch_of_samples_received_handler([&](auto) { samples_received_flag.set(); });
    sampler.start();

    samples_received_flag.wait();
    sampler.stop();

    // TODO: Refactor tests in such a way that we assert for something specific, e.g. wait() has timeout.
    TEST_ASSERT_TRUE(true);
}

void test_multiple_instances_can_be_run_one_after_another()
{
    {
        jungles::freertos::flag samples_received_flag;
        SamplingTriggerTimerImpl sampling_trigger_timer;
        ThreadedAudioSampler sampler{sampling_trigger_timer};
        sampler.set_on_batch_of_samples_received_handler([&](auto) { samples_received_flag.set(); });
        sampler.start();
        samples_received_flag.wait();
        sampler.stop();
    }

    os::wait(300ms);

    {
        jungles::freertos::flag samples_received_flag;
        SamplingTriggerTimerImpl sampling_trigger_timer;
        ThreadedAudioSampler sampler{sampling_trigger_timer};
        sampler.set_on_batch_of_samples_received_handler([&](auto) { samples_received_flag.set(); });
        sampler.start();
        samples_received_flag.wait();
        sampler.stop();
    }

    TEST_ASSERT_TRUE(true);
}

void test_proper_number_of_samples_is_collected_within_specific_period()
{
    jungles::freertos::flag samples_received_flag;

    unsigned samples_received{0};

    SamplingTriggerTimerImpl sampling_trigger_timer;
    ThreadedAudioSampler sampler{sampling_trigger_timer};
    sampler.set_on_batch_of_samples_received_handler([&](auto samples) { samples_received += samples->size(); });
    sampler.start();

    os::wait(1s);
    sampler.stop();

    unsigned samples_received_after_one_second{samples_received};

    TEST_ASSERT_UINT_WITHIN(100, 44100, samples_received_after_one_second);
}

void test_cant_create_two_instances()
{
    bool is_failed{false};
    try
    {
        SamplingTriggerTimerImpl sampling_trigger_timer;
        ThreadedAudioSampler sampler{sampling_trigger_timer};
        ThreadedAudioSampler{sampling_trigger_timer};
    } catch (const ThreadedAudioSampler::Error&)
    {
        is_failed = true;
    }
    TEST_ASSERT_TRUE(is_failed);
}
