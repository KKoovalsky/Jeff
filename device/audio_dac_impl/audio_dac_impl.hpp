/**
 * @file        audio_dac_impl.hpp
 * @brief       Defines AudioDac implementation.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef AUDIO_DAC_IMPL_HPP
#define AUDIO_DAC_IMPL_HPP

#include "audio_dac.hpp"

#include <array>
#include <memory>

#include "jungles/singleton.hpp"

static inline constexpr unsigned AudioDacBufferSize{64};
using AudioDacBuffer = std::array<float, AudioDacBufferSize>;
using AudioDacBatchOfSamples = std::unique_ptr<AudioDacBuffer>;
using AudioDacInterface = AudioDac<AudioDacBatchOfSamples>;

class AudioDacImpl : public AudioDacInterface, jungles::Singleton<AudioDacInterface, AudioDacInterface::Error>
{
  public:
    void start() override;
    void stop() override;
    void set_on_stream_update_handler(Handler) override;
};

#endif /* AUDIO_DAC_IMPL_HPP */
