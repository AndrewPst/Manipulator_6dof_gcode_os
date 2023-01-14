#include "models/mutexLocker.h"

#include "Mutex.h"
#include "out.h"

template <>
void MutexLocker<rtos::Mutex>::lock()
{
    _m->lock();
}

template <>
void MutexLocker<rtos::Mutex>::unlock()
{
    _m->unlock();
}

template <>
bool MutexLocker<rtos::Mutex>::tryLock()
{
    return _m->trylock();
}