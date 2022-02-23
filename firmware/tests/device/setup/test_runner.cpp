/**
 * @file	test_runner.cpp
 * @brief	Defines the test runner.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include <exception>
#include <string>

#include "jungles_os_helpers/freertos/thread.hpp"

#include "serial_logger.hpp"

extern "C" void app_main();

extern int test_main();

extern "C" void app_main()
{
    jungles::freertos::thread t{std::string("test_runner"), 1024, 5};
    t.start([]() {
        try
        {
            test_main();
        } catch (const std::exception& e)
        {
            SerialLogger{}.log(LogLevel::error) << "UNCAUGHT EXCEPTION: " << e.what();
        }
    });
    t.join();
}
