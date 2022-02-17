/**
 * @file    event_periodic_dumper.cpp
 * @brief   Implements EventPeriodicDumper.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include <mutex>

#include "event_periodic_dumper.hpp"

#include "cmsis_os.h"

#include "os_waiters.hpp"
#include "serial_logger.hpp"

EventPeriodicDumper::EventPeriodicDumper(unsigned int dumping_period_in_milliseconds, BenchmarkTimer& timer) :
    dumping_period_in_milliseconds{dumping_period_in_milliseconds},
    timer{timer},
    runner{"EventPeriodicDumper", 512, osPriorityNormal}
{
    runner.start([this]() {
        while (!quit_runner)
        {
            os::wait_milliseconds(this->dumping_period_in_milliseconds);

            SerialLogger logger;
            std::string ruler(20, '=');
            logger.log(LogLevel::info) << ruler << "EVENT DUMP BEGIN:" << ruler;

            std::lock_guard lock_guard{buffer_mutex};
            Event event = {};
            while (true)
            {
                if (auto is_empty{!captured_events.remove(event)}; is_empty)
                    break;

                logger.log(LogLevel::info) << event.name << ": " << event.time_microseconds << " us";
            }

            logger.log(LogLevel::info) << ruler << "EVENT DUMP END:" << ruler;
        }
    });
}

EventPeriodicDumper::~EventPeriodicDumper()
{
    quit_runner = true;
    runner.join();
}

void EventPeriodicDumper::capture(std::string_view event)
{
    if (not buffer_mutex.try_lock())
        return;

    auto time_us{timer.time_microseconds()};
    captured_events.insert({.name = event, .time_microseconds = time_us});

    buffer_mutex.unlock();
}
