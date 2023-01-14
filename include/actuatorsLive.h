#pragma once

#include <vector>
#include <memory>
#include "mbed.h"
#include "rtos.h"
#include "defines.h"

#include "models/mutexLocker.h"
#include "interfaces/iActuatorDriver.h"

class ActuatorsLive
{
private:
    std::vector<std::unique_ptr<IActuatorDriver>> _actuators;

    MutexT _mutex;

private:
    void threadMethod()
    {
        while (true)
        {
            _mutex.lock();
            for (auto &d : _actuators)
            {
                (*d).tick();
            }
            _mutex.unlock();
            thread_sleep_for(20);
        }
    }

public:
    template <typename T, typename... argsT>
    T &registerActuator(const argsT &...args)
    {
        MutexLocker<MutexT> locker(_mutex);
        T *newA = new T(args...);
        std::unique_ptr<IActuatorDriver> act(newA);
        _actuators.push_back(std::move(act));
        return static_cast<T &>(*newA);
    }

    template <typename _Thr>
    void startThread(_Thr &thr)
    {
        thr.start(Callback<void()>(this, &ActuatorsLive::threadMethod));
    }
};