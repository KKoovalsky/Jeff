/**
 * @file        dummy_event_tracer.hpp
 * @brief       EventTracer implementation which does nothing.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef DUMMY_EVENT_TRACER_HPP
#define DUMMY_EVENT_TRACER_HPP

#include "event_tracer.hpp"

struct DummyEventTracer : EventTracer
{
    void capture(std::string_view) override
    {
    }
};

#endif /* DUMMY_EVENT_TRACER_HPP */
