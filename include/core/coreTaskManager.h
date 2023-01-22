#pragma once

#include "core.h"
#include "defines.h"
#include "models/mutexLocker.h"

#include "global.h"
#include "dataConveyor.h"

#include <map>
#include <utility>

namespace core
{

    enum CoreTaskKey : uint8_t
    {
        TASK_INPUT_UART = 0,
        TASK_INPUT_FILE = 1,
    };

    template <typename _ThreadRegistrator>
    class CoreTaskManager
    {
    private:
        MutexT _contextMutex;

        using _threadT = typename _ThreadRegistrator::Thread_t;

        std::map<uint8_t, std::pair<core::ExecutionContext, _threadT *>> _contexts;

    public:
        ExecutionContext &getContext(uint8_t key)
        {
            MutexLocker<MutexT> lock(_contextMutex);
            return _contexts[key].first;
        }

        uint8_t startExecutionInThread(uint8_t key, core::BehaviourFlag_t flags)
        {
            MutexLocker<MutexT> lock(_contextMutex);
            auto &p = _contexts[key];
            if (p.first.state() != core::ExecuteState_t::STATE_FINISHED)
                return false;
            if (p.second)
                delete p.second;
            p.second = new _threadT();
            uint8_t result = _ThreadRegistrator::start(*p.second, [&p, flags]() -> void
                                                       { core.startExecution<EasyDataConveyor>(flags, p.first); });
            return result;
        }
    };

}