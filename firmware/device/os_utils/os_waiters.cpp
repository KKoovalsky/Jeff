/**
 * @file    os_waiters.cpp
 * @brief   Implements the waiters.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "FreeRTOS.h"
#include "task.h"

namespace os
{

void wait_milliseconds(unsigned milliseconds_to_wait)
{
    vTaskDelay(pdMS_TO_TICKS(milliseconds_to_wait));
}

[[noreturn]] void loop()
{
    while (true)
        vTaskDelay(pdMS_TO_TICKS(1000));
}

} // namespace os
