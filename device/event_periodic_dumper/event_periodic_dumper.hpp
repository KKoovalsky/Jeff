/**
 * @file        event_periodic_dumper.hpp
 * @brief       Implements EventTracer which dumps the events periodically.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef EVENT_PERIODIC_DUMPER_HPP
#define EVENT_PERIODIC_DUMPER_HPP

#include "event_tracer.hpp"

#include <string_view>

#include "jungles_os_helpers/freertos/mutex.hpp"
#include "jungles_os_helpers/freertos/thread.hpp"
#include "ringbuffer.hpp"

#include "benchmark_timer.hpp"

class EventPeriodicDumper : public EventTracer
{
  public:
    explicit EventPeriodicDumper(unsigned dumping_period_in_milliseconds, BenchmarkTimer& timer);
    ~EventPeriodicDumper() override;
    void capture(std::string_view event) override;

  private:
    struct Event
    {
        std::string_view name;
        unsigned time_microseconds;
    };

    static constexpr inline unsigned EventBufferSize{256};
    static constexpr inline bool do_fake_tso{true};
    static constexpr inline unsigned cacheline_size{4};
    using RingBuffer = jnk0le::Ringbuffer<Event, EventBufferSize, do_fake_tso, cacheline_size>;

    const unsigned dumping_period_in_milliseconds;
    RingBuffer captured_events;
    BenchmarkTimer& timer;
    std::atomic_bool quit_runner{false};
    jungles::freertos::mutex buffer_mutex;
    jungles::freertos::thread runner;
};

#endif /* EVENT_PERIODIC_DUMPER_HPP */
