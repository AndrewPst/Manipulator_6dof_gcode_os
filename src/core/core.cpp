#include "core/core.h"

#include "out.h"

using namespace core;

Core::Core() {}

void Core::initializate()
{
}

void Core::initJoints(ExecutionContext &context)
{
    for (auto &j : context.enivroment().manipulator.joints)
    {
        j->initialize();
    }
}

void Core::setWaitFunc(WaitFunction w)
{
    _waitFunc = w;
}

WaitFunction Core::waitFunc() const
{
    return _waitFunc;
}

void Core::execute(Command_t &command, BehaviourFlag_t flag, ExecutionContext &context)
{
    MutexLocker<MutexT> _executorLock(_executorsMut, false);
    auto ex_pointer = _avaiableExecutors.find(command.Key);
    if (ex_pointer == _avaiableExecutors.end())
    {
        DEBUG_ERR("[%s]-unknown command", command.Key)
        if (flag & BehaviourFlags::DEHAVIOUR_FINISH_PROGRAM_IF_UNKNOWN_COMMAND)
            context.setState(core::ExecuteState_t::STATE_FINISHED);
        return;
    }
    _executorLock.unlock();
    std::unique_ptr<IExecutableCommand> executer(((*ex_pointer).second)());
    ::ExecuteState_t _exState{::ExecuteState_t::STATE_IN_PROCESS};
    TimerT timer;
    timer.reset();
    timer.start();
    executer->started(context.enivroment(), command);
    while (_exState != ::ExecuteState_t::STATE_FINISHED)
    {
        if (context.state() == core::ExecuteState_t::STATE_RUNNING)
        {
            timer.start();
            _exState = executer->execute(context.enivroment(), std::chrono::duration_cast<std::chrono::milliseconds>(timer.elapsed_time()).count());
            if (_exState == ::ExecuteState_t::STATE_FINISHED)
            {
                break;
            }
            else if (_exState == ::ExecuteState_t::STATE_ERROR)
            {
                DEBUG_ERR("[%s]-execution error: %d\n", command.Key, _exState)
                if (flag & BehaviourFlags::DEHAVIOUR_FINISH_PROGRAN_IF_EXECUTION_ERROR)
                    context.setState(core::ExecuteState_t::STATE_FINISHED);
            }
        }
        else if (context.state() == core::ExecuteState_t::STATE_SUSPENDED)
        {
            timer.stop();
        }
        else if (context.state() == core::ExecuteState_t::STATE_FINISHED)
        {
            break;
        }
        _waitFunc(2);
    }
    executer->ended();
}