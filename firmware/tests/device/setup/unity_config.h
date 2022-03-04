/**
 * @file	unity_config.h
 * @brief	Unity test framework custom config file.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef UNITY_CONFIG_H
#define UNITY_CONFIG_H

void unity_putchar(int c);

#define UNITY_OUTPUT_CHAR(c) unity_putchar(c)
#define UNITY_OUTPUT_START()
#define UNITY_OUTPUT_FLUSH()
#define UNITY_OUTPUT_COMPLETE()

#define UNITY_PRINT_EOL()        \
    do                           \
    {                            \
        UNITY_OUTPUT_CHAR('\r'); \
        UNITY_OUTPUT_CHAR('\n'); \
    } while (0)

#define UNITY_EXCLUDE_SETJMP_H

#endif /* UNITY_CONFIG_H */
