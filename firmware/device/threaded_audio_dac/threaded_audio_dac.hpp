/**
 * @file        threaded_audio_dac.hpp
 * @brief       Defines AudioDac implementation.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef AUDIO_DAC_IMPL_HPP
#define AUDIO_DAC_IMPL_HPP

#include "audio_chain_config.hpp"
#include "audio_dac.hpp"
#include "event_tracer.hpp"
#include "sampling_trigger_timer.hpp"

#include <array>
#include <cstdint>

#include "jungles/singleton.hpp"

#include "FreeRTOS.h"
#include "event_groups.h"

using AudioDacInterface = AudioDac<AudioChainConfig::BatchOfSamples>;

extern "C" void DMA1_Channel3_IRQHandler();

class AudioDacWithDma : public AudioDacInterface, jungles::Singleton<AudioDacInterface>
{
  public:
    explicit AudioDacWithDma(SamplingTriggerTimer&, EventTracer&);
    ~AudioDacWithDma() override;

    using BatchOfSamples = AudioChainConfig::BatchOfSamples;

    void await_stream_update(BatchOfSamples) override;

  private:
    void start();
    void stop();
    void configure_dma();
    static void enable_dac();
    static void disable_dma();
    static void disable_dac();

    static uint16_t convert_sample(float);

    static inline AudioDacWithDma* singleton_pointer{nullptr};

    static inline constexpr unsigned RawBufferSize{AudioChainConfig::WindowSize * 2};
    using RawSampleBuffer = std::array<uint16_t, RawBufferSize>;

    RawSampleBuffer raw_sample_buffer = {};

    SamplingTriggerTimer& sampling_trigger_timer;

    EventTracer& event_tracer;

    EventGroupHandle_t event_group_handle;

    friend void DMA1_Channel3_IRQHandler();
};

#endif /* AUDIO_DAC_IMPL_HPP */
