#pragma once

#include "core.h"
#include "defines.h"
#include "models/mutexLocker.h"

#include <map>
#include <utility>



namespace core
{

    enum CoreTaskKey : uint8_t
    {
        TASK_INPUT_UART = 0,
        TASK_INPUT_FILE = 1,
    };

    template <typename _ThreadT>
    class CoreTaskManager
    {
    private:
        MutexT _contextMutex;

        std::map<uint8_t, std::pair<core::ExecutionContext, _ThreadT*>> _contexts;

    public:
        ExecutionContext &getContext(uint8_t key)
        {
            MutexLocker<MutexT> lock(_contextMutex);
            return _contexts[key].first;
        }

        bool startExecutionInThread(uint8_t key, core::BehaviourFlag_t flags);
    };

}