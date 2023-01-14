#pragma once

#include <mbed.h>
#include "config.h"
#include "defines.h"
#include "serial/uartSerial.h"
#include <stdio.h>

#ifdef DEBUG_INFO
#undef DEBUG_INFO
#endif
#if __DEBUG_LEVEL >= __DEBUG_LEVEL_INFO
#define DEBUG_INFO(format, ...)                                                                            \
    {                                                                                                      \
        ssize_t bufsz = snprintf(NULL, 0, format, ##__VA_ARGS__);                                          \
        char *buf = (char *)malloc(bufsz + 1);                                                             \
        snprintf(buf, bufsz + 1, format, ##__VA_ARGS__);                                                   \
        debug_out << ">INFO< " << '[' << __FILE__ << ']' << " at \"" << __LINE__ << "\": " << buf << '\n'; \
        free(buf);                                                                                         \
    };
#else
#define DEBUG_INFO(format, ...)
#endif

#ifdef DEBUG_ERR
#undef DEBUG_ERR
#endif
#if __DEBUG_LEVEL >= ___DEBUG_LEVEL_ERRORS
#define DEBUG_ERR(format, ...)                                                                              \
    {                                                                                                       \
        ssize_t bufsz = snprintf(NULL, 0, format, ##__VA_ARGS__);                                           \
        char *buf = (char *)malloc(bufsz + 1);                                                              \
        snprintf(buf, bufsz + 1, format, ##__VA_ARGS__);                                                    \
        debug_out << ">Error< " << '[' << __FILE__ << ']' << " at \"" << __LINE__ << "\": " << buf << '\n'; \
        free(buf);                                                                                          \
    };
#else
#define DEBUG_ERR(format, ...)
#endif

#ifdef DEBUG_VERBOSE
#undef DEBUG_VERBOSE
#endif
#if __DEBUG_LEVEL >= __DEBUG_LEVEL_VERBOSE
#define DEBUG_VERBOSE(format, ...)                                                                            \
    {                                                                                                         \
        ssize_t bufsz = snprintf(NULL, 0, format, ##__VA_ARGS__);                                             \
        char *buf = (char *)malloc(bufsz + 1);                                                                \
        snprintf(buf, bufsz + 1, format, ##__VA_ARGS__);                                                      \
        debug_out << ">Verbose< " << '[' << __FILE__ << ']' << " at \"" << __LINE__ << "\": " << buf << '\n'; \
        free(buf);                                                                                            \
    };
#else
#define DEBUG_VERBOSE(format, ...)
#endif

extern UartSerial<DebugType_t> debug_out;