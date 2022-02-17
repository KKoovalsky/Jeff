/**
 * @file    jeff_app_wrapper.cpp
 * @brief   Implements the JeffAppWrapper.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "jeff_app_wrapper.hpp"
#include "filter_cutoff_setter_clock.hpp"
#include "os_waiters.hpp"
#include "sampling_trigger_timer_impl.hpp"
#include "threaded_audio_dac.hpp"
#include "threaded_audio_sampler.hpp"

#include "audio_chain.hpp"

#include "jungles_os_helpers/freertos/mutex.hpp"

#ifdef JEFF_TRACE_AND_DUMP_EVENTS
#include "benchmark_timer.hpp"
#include "event_periodic_dumper.hpp"
#else /* JEFF_TRACE_AND_DUMP_EVENTS */
#include "dummy_event_tracer.hpp"
#endif /* JEFF_TRACE_AND_DUMP_EVENTS */

JeffAppWrapper::JeffAppWrapper(GuitarEffectImpl& guitar_effect) : guitar_effect{guitar_effect}
{
}

void JeffAppWrapper::loop()
{
    FilterCutoffSetterClock filter_cutoff_setter_clock;

#ifdef JEFF_TRACE_AND_DUMP_EVENTS
    BenchmarkTimer benchmark_timer;
    EventPeriodicDumper event_tracer{2000, benchmark_timer};
#else  /* JEFF_TRACE_AND_DUMP_EVENTS */
    DummyEventTracer event_tracer;
#endif /* JEFF_TRACE_AND_DUMP_EVENTS */

    SamplingTriggerTimerImpl sampling_trigger_timer;
    ThreadedAudioSampler audio_sampler{sampling_trigger_timer, event_tracer};
    ThreadedAudioDac audio_dac{sampling_trigger_timer, event_tracer};

    AudioChain<jungles::freertos::mutex, AudioChainConfig::BatchOfSamples> audio_chain{
        audio_sampler, guitar_effect, audio_dac, event_tracer};

    os::loop();
}
