/**
 * @file    test_audio_chain.cpp
 * @brief   Tests the Audio Chain with guitar effect which does nothing.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "audio_chain.hpp"
#include "audio_dac_impl.hpp"
#include "dummy_guitar_effect.hpp"
#include "os_waiters.hpp"
#include "sampling_trigger_timer_impl.hpp"
#include "serial_logger.hpp"
#include "threaded_audio_sampler.hpp"

#include "jungles_os_helpers/freertos/mutex.hpp"

void test_input_signal_is_duplicated_at_output()
{
    using BatchOfSamples = AudioChainConfig::BatchOfSamples;

    SamplingTriggerTimerImpl sampling_trigger_timer;
    ThreadedAudioSampler audio_sampler{sampling_trigger_timer};
    AudioDacImpl audio_dac{sampling_trigger_timer};
    DummyGuitarEffect<BatchOfSamples> guitar_effect;

    SerialLogger logger;
    logger.log(LogLevel::info) << "Creating audio chain ...";
    AudioChain<jungles::freertos::mutex, BatchOfSamples> audio_chain{audio_sampler, guitar_effect, audio_dac};

    logger.log(LogLevel::info) << "Audio chain created, looping indefinitely...";
    os::loop();
}