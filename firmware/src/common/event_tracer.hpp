/**
 * @file        event_tracer.hpp
 * @brief       Interface for tracing events within the system.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef EVENT_TRACER_HPP
#define EVENT_TRACER_HPP

#include <string_view>

struct EventTracer
{
    virtual void capture(std::string_view event) = 0;
    virtual ~EventTracer() = default;

    struct RainEventsCaptor
    {
        explicit RainEventsCaptor(EventTracer& event_tracer,
                                  std::string_view event_begin_message,
                                  std::string_view event_end_message) :
            event_tracer{event_tracer}, event_begin_message{event_begin_message}, event_end_message{event_end_message}
        {
            event_tracer.capture(event_begin_message);
        }

        ~RainEventsCaptor()
        {
            event_tracer.capture(event_end_message);
        }

        EventTracer& event_tracer;
        std::string_view event_begin_message;
        std::string_view event_end_message;
    };

    /** @brief Creates RAII captor to trace begin and end events for the calling scope.
     *
     * Creates a RAII object, which calls EventTracer&::capture() with corresponding messages on construction and
     * destruction of the RAII object.
     */
    [[nodiscard]] RainEventsCaptor make_raii_events_captor(std::string_view event_begin_message,
                                                           std::string_view event_end_message)
    {
        return RainEventsCaptor(*this, event_begin_message, event_end_message);
    }
};

#endif /* EVENT_TRACER_HPP */
