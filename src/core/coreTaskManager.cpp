#include "core/coreTaskManager.h"

#include "global.h"
#include "dataConveyor.h"
using namespace core;

template <>
bool CoreTaskManager<rtos::Thread>::startExecutionInThread(uint8_t key, core::BehaviourFlag_t flags)
{
    MutexLocker<MutexT> lock(_contextMutex);
    auto &p = _contexts[key];
    if (p.first.state() != core::ExecuteState_t::STATE_FINISHED)
        return false;
    if(p.second)
        delete p.second;
    p.second = new rtos::Thread();
    auto r = p.second->start(mbed::callback([&p, flags]() -> void
                                           { core.startExecution<EasyDataConveyor>(flags, p.first); }));
    return r == osStatus_t::osOK;
}