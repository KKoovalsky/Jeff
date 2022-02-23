/**
 * @file    test_audio_chain.cpp
 * @brief   Tests the Audio Chain with guitar effect which does nothing.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "dummy_guitar_effect.hpp"
#include "jeff_app_wrapper.hpp"
#include "serial_logger.hpp"

void test_input_signal_is_duplicated_at_output()
{
    using BatchOfSamples = AudioChainConfig::BatchOfSamples;

    DummyGuitarEffect<BatchOfSamples> guitar_effect;

    SerialLogger logger;
    logger.log(LogLevel::info) << "Creating audio chain ...";
    JeffAppWrapper jeff{guitar_effect};

    logger.log(LogLevel::info) << "Audio chain created, looping indefinitely...";
    jeff.loop();
}