#pragma once

#include <vector>
#include <memory>
#include "mbed.h"
#include "rtos.h"
#include "defines.h"

#include "models/mutexLocker.h"
#include "interfaces/iActuatorDriver.h"

template <typename _ThreadRegistrator>
class ActuatorsLive
{
private:
    std::vector<std::unique_ptr<IActuatorDriver>> _actuators;

    constexpr static const uint16_t PAUSE_TIME_MS = 5;

    MutexT _mutex;
    TimerT _timer;

private:
    void threadMethod()
    {
        _timer.reset();
        _timer.start();
        while (true)
        {
            _mutex.lock();
            IActuatorDriver::TimeDiff_ms_t elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(_timer.elapsed_time()).count();
            for (auto &d : _actuators)
            {
                (*d).tick(elapsedTime);
            }
            _timer.reset();
            _mutex.unlock();
            thread_sleep_for(PAUSE_TIME_MS);
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
        _ThreadRegistrator::start(thr, [&]() -> void {threadMethod();});
        //thr.start(Callback<void()>(this, &ActuatorsLive::threadMethod));
    }
};