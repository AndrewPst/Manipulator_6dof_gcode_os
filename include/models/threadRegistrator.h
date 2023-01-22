#pragma once

#include "mbed.h"
#include "rtos.h"

class ThreadRegistrator
{
public:
    // typedef rtos::Thread Thread_t;

    using Thread_t = rtos::Thread;

    template <typename Functor>
    static uint8_t start(Thread_t &thread, Functor func)
    {
        auto r = thread.start(mbed::callback(func));
        return r == osStatus_t::osOK;
    }
};