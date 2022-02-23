/**
 * @file    serial_logger.cpp
 * @brief   Implements the Serial Logger.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "serial_logger.hpp"
#include "usart.h"

#include "printf.h"

unsigned SerialLogger::instance_count = 0;

SerialLogger::SerialLoggerProxy::SerialLoggerProxy(LogLevel log_level)
{
    *this << log_level_to_string(log_level);
}

SerialLogger::SerialLoggerProxy& SerialLogger::SerialLoggerProxy::operator<<(std::string s)
{
    write_bytes_over_uart(reinterpret_cast<uint8_t*>(s.data()), s.length());
    return *this;
}

SerialLogger::SerialLoggerProxy& SerialLogger::SerialLoggerProxy::operator<<(std::string_view sv)
{
    return *this << std::string(std::begin(sv), std::end(sv));
}

SerialLogger::SerialLoggerProxy& SerialLogger::SerialLoggerProxy::operator<<(const char* s_ptr)
{
    return *this << std::string(s_ptr);
}

SerialLogger::SerialLoggerProxy::~SerialLoggerProxy()
{
    *this << "\r\n";
}

SerialLogger::SerialLoggerProxy& SerialLogger::SerialLoggerProxy::operator<<(char c)
{
    write_bytes_over_uart(reinterpret_cast<uint8_t*>(&c), 1);
    return *this;
}

SerialLogger::SerialLoggerProxy& SerialLogger::SerialLoggerProxy::operator<<(unsigned char v)
{
    char b[8] = {};
    auto length{snprintf_(b, std::size(b), "0x%02X", v)};
    write_bytes_over_uart(b, length);
    return *this;
}

SerialLogger::SerialLoggerProxy& SerialLogger::SerialLoggerProxy::operator<<(float v)
{
    char b[64] = {};
    auto length{snprintf_(b, std::size(b), "%f", v)};
    write_bytes_over_uart(b, length);
    return *this;
}

SerialLogger::SerialLoggerProxy& SerialLogger::SerialLoggerProxy::operator<<(unsigned int v)
{
    char b[16] = {};
    auto length{snprintf_(b, std::size(b), "%u", v)};
    write_bytes_over_uart(b, length);
    return *this;
}

SerialLogger::SerialLoggerProxy& SerialLogger::SerialLoggerProxy::operator<<(int v)
{
    char b[16] = {};
    auto length{snprintf_(b, std::size(b), "%d", v)};
    write_bytes_over_uart(b, length);
    return *this;
}

void SerialLogger::SerialLoggerProxy::write_bytes_over_uart(unsigned char* data, unsigned int length)
{
    uint32_t timeout_ms{300};
    HAL_UART_Transmit(&hlpuart1, data, length, timeout_ms);
}

void SerialLogger::SerialLoggerProxy::write_bytes_over_uart(char* data, unsigned int length)
{
    return write_bytes_over_uart(reinterpret_cast<uint8_t*>(data), length);
}

SerialLogger::SerialLoggerProxy SerialLogger::log(LogLevel log_level) const
{
    return SerialLoggerProxy{log_level};
}

SerialLogger::SerialLogger()
{
    if (instance_count == 0)
        MX_LPUART1_UART_Init();

    ++instance_count;
}

SerialLogger::~SerialLogger()
{
    --instance_count;

    if (instance_count == 0)
        HAL_UART_DeInit(&hlpuart1);
}

void SerialLogger::log_raw(char c)
{
    SerialLoggerProxy::write_bytes_over_uart(&c, 1);
}
