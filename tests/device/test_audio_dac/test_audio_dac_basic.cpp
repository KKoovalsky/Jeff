/**
 * @file    test_audio_dac_basic.cpp
 * @brief   Tests Audio DAC implementation.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "unity.h"

#include "jungles_os_helpers/freertos/flag.hpp"
#include "os_waiters.hpp"
#include "sampling_trigger_timer_impl.hpp"
#include "threaded_audio_dac.hpp"

#include "dummy_event_tracer.hpp"

#include <chrono>
#include <memory>

/**
 * @defgroup AudioDacTests     Declaration of the tests of DAC.
 * @{
 */
void test_two_instances_work_one_after_another();
void test_single_batch_is_streamed();
void test_multiple_batches_are_streamed();
/** @} */ // End of group AudioDacTests

using namespace std::chrono_literals;

// =====================================================================================================================
// Tests' definition
// =====================================================================================================================
void test_two_instances_work_one_after_another()
{
    SamplingTriggerTimerImpl sampling_trigger_timer;
    DummyEventTracer event_tracer;
    {
        ThreadedAudioDac dac{sampling_trigger_timer, event_tracer};
        dac.start();
        os::wait(100ms);
        dac.stop();
    }

    {
        ThreadedAudioDac dac{sampling_trigger_timer, event_tracer};
        dac.start();
        os::wait(100ms);
        dac.stop();
    }

    TEST_ASSERT(true);
}

void test_single_batch_is_streamed()
{
    jungles::freertos::flag stream_updated_flag;

    SamplingTriggerTimerImpl sampling_trigger_timer;
    DummyEventTracer event_tracer;

    ThreadedAudioDac dac{sampling_trigger_timer, event_tracer};
    dac.set_on_stream_update_handler([&]() {
        stream_updated_flag.set();
        return AudioChainConfig::BatchOfSamples{};
    });

    dac.start();
    stream_updated_flag.wait();
    dac.stop();

    TEST_ASSERT(true);
}

void test_multiple_batches_are_streamed()
{
    jungles::freertos::flag stream_updated_multiple_times_flag;

    SamplingTriggerTimerImpl sampling_trigger_timer;
    DummyEventTracer event_tracer;

    ThreadedAudioDac dac{sampling_trigger_timer, event_tracer};
    dac.set_on_stream_update_handler([&]() {
        static unsigned stream_update_count{0};
        stream_update_count++;
        if (stream_update_count == 5)
            stream_updated_multiple_times_flag.set();
        return AudioChainConfig::BatchOfSamples{};
    });

    dac.start();
    stream_updated_multiple_times_flag.wait();
    dac.stop();

    TEST_ASSERT(true);
}
