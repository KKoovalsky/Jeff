/**
 * @file    distortion_app.cpp
 * @brief   Implements specialized BasicWindowedDistortionWithMemory for the device.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "audio_chain_config.hpp"
#include "basic_windowed_distortion_with_memory.hpp"
#include "jeff_app_wrapper.hpp"

using Distortion = BasicWindowedDistortionWithMemory<AudioChainConfig::WindowSize>;

extern "C" void app_main()
{
    static constexpr float threshold{0.7};
    Distortion distortion{threshold};

    JeffAppWrapper jeff{distortion};
    jeff.loop();
}
