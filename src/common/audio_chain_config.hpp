/**
 * @file        audio_chain_config.hpp
 * @brief       Global config for the Jeff's audio chain.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef AUDIO_CHAIN_CONFIG_HPP
#define AUDIO_CHAIN_CONFIG_HPP

#include "batch_of_samples.hpp"

namespace AudioChainConfig
{

static inline constexpr unsigned WindowSize{64};

using BatchOfSamples = BatchOfSamplesTemplate<WindowSize>;

} // namespace AudioChainConfig

#endif /* AUDIO_CHAIN_CONFIG_HPP */
