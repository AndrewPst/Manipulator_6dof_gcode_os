#include "commandExecutors/g_group.h"

#include "config.h"
#include "global.h"
#include "out.h"
#include "sdCardManager.h"
#include "serial/fileSerial.h"
#include "core/coreTaskManager.h"
#include "core/core.h"

using namespace executableCommands;

constexpr static const char TIME_KEY = 'T';
constexpr static const char JOINT_0_KEY = 'A';
constexpr static const char JOINT_1_KEY = 'B';
constexpr static const char JOINT_2_KEY = 'C';
constexpr static const char JOINT_3_KEY = 'D';
constexpr static const char JOINT_4_KEY = 'E';
constexpr static const char JOINT_5_KEY = 'F';
constexpr static const char EFFECTOR_KEY = 'P';

namespace debug_mess
{
#if __DEBUG_LEVEL >= __DEBUG_LEVEL_ERR
    constexpr static const char *START_EXECUTION_ERR = "Start execution error - %s";
#endif
};

ExecuteState_t setFile(const char *name)
{
    auto &c = core::coreTaskManager.getContext(core::CoreTaskKey::TASK_INPUT_FILE);
    if (c.state() != core::ExecuteState_t::STATE_FINISHED)
    {
        DEBUG_ERR(debug_mess::START_EXECUTION_ERR, "task was not finished");
        return ExecuteState_t::STATE_ERROR;
    }
    FILE *f = sdCard.openFile(name, "r");
    if (!f)
    {
        DEBUG_ERR(debug_mess::START_EXECUTION_ERR, "opening file err");
        return ExecuteState_t::STATE_ERROR;
    }
    if (&c.input())
        delete &c.input();
    c.setInput(*(new FileSerial(f, true)));
    return ExecuteState_t::STATE_IN_PROCESS;
}

#pragma region m32

ExecuteState_t m32::started(core::ExecutionEnivroment &m, Command_t &args)
{
    if (args.args.size() == 0)
    {
        return ExecuteState_t::STATE_ERROR;
    }
    auto p = args.args.begin();
    const char *name = *p;
    DEBUG_INFO("File name: %s", name);
    if (setFile(name) != ExecuteState_t::STATE_IN_PROCESS)
    {
        return ExecuteState_t::STATE_ERROR;
    }
    auto result = core::coreTaskManager.startExecutionInThread(core::CoreTaskKey::TASK_INPUT_FILE, 0);
    if (result == false)
    {
        return ExecuteState_t::STATE_ERROR;
    }
    return ExecuteState_t::STATE_IN_PROCESS;
}

ExecuteState_t m32::execute(core::ExecutionEnivroment &, TimeDif_ms_t time)
{
    return ExecuteState_t::STATE_FINISHED;
}

void m32::ended() {}

#pragma endregion

#pragma region m20

ExecuteState_t m20::started(core::ExecutionEnivroment &m, Command_t &args)
{
#if __USE_SDCARD == 1
    sdCard.printFiles<ISerial>("/sd/", core::coreTaskManager.getContext(core::CoreTaskKey::TASK_INPUT_UART).output());
    return ExecuteState_t::STATE_FINISHED;
#else

    return ExecuteState_t::STATE_ERROR;
#endif
}

ExecuteState_t m20::execute(core::ExecutionEnivroment &, TimeDif_ms_t time)
{
    return ExecuteState_t::STATE_FINISHED;
}

void m20::ended() {}

#pragma endregion

#pragma region m21

ExecuteState_t m21::started(core::ExecutionEnivroment &m, Command_t &args)
{
    int init_sd_result{0};
    sdCard.unmount();
    sdCard.deinit();
    init_sd_result += sdCard.init();
    init_sd_result += sdCard.mount();
    if (init_sd_result == 0)
    {
        DEBUG_INFO("Sd mount done");
        return ExecuteState_t::STATE_IN_PROCESS;
    }
    DEBUG_INFO("Sd mount err");
    return ExecuteState_t::STATE_ERROR;
}

ExecuteState_t m21::execute(core::ExecutionEnivroment &, TimeDif_ms_t time)
{
    return ExecuteState_t::STATE_FINISHED;
}

void m21::ended() {}

#pragma endregion

#pragma region m21

ExecuteState_t m22::started(core::ExecutionEnivroment &m, Command_t &args)
{
    int deinit_sd_result{0};
    deinit_sd_result += sdCard.unmount();
    deinit_sd_result += sdCard.deinit();
    if (deinit_sd_result == 0)
    {
        DEBUG_INFO("Sd unmount done");
        return ExecuteState_t::STATE_IN_PROCESS;
    }
    DEBUG_INFO("Sd unmount err");
    return ExecuteState_t::STATE_ERROR;
}

ExecuteState_t m22::execute(core::ExecutionEnivroment &, TimeDif_ms_t time)
{
    return ExecuteState_t::STATE_FINISHED;
}

void m22::ended() {}

#pragma endregion

#pragma region m17

ExecuteState_t m17::started(core::ExecutionEnivroment &m, Command_t &args)
{
    // auto newTime = std::find_if(args.args.begin(), args.args.end(), [](const char *a)
    //                             { return a[0] == TIME_KEY; });
    // if (newTime != args.args.end())
    //     _pause = atoi(*newTime + 1);
    // bool _isAllActEnable{true};
    // for (auto i = args.args.begin(); i != args.args.end(); i++)
    // {
    //     if (*i[0] == TIME_KEY)
    //         continue;
    //     _isAllActEnable = false;
    //     if (*i[0] == EFFECTOR_KEY)
    //         _act[DOF] = 1;
    //     uint8_t index = *i[0] - JOINT_0_KEY;
    //     if (index >= DOF)
    //         return ExecuteState_t::STATE_ERROR;
    //     _act[index] = 1;
    // }
    // if (_isAllActEnable)
    // {
    //     _act = std::bitset<DOF+1>().set();
    // }
    // return ExecuteState_t::STATE_IN_PROCESS;
    for (auto &j : m.manipulator.joints)
    {
        j->enable();
    }
    m.manipulator.effector->enable();
    return ExecuteState_t::STATE_IN_PROCESS;
}

ExecuteState_t m17::execute(core::ExecutionEnivroment &env, TimeDif_ms_t time)
{
    // if (time - _last < _pause)
    //     return ExecuteState_t::STATE_IN_PROCESS;
    // while (_act[_pos] == 0)
    // {
    //     ++_pos;
    //     if (_pos > DOF)
    //         break;
    // }
    // if (_pos < DOF)
    //     env.manipulator.joints[_pos]->enable();
    // else if (_pos == DOF)
    //     env.manipulator.effector->enable();
    // else
    //     return ExecuteState_t::STATE_FINISHED;
    // _last = time;
    // return ExecuteState_t::STATE_IN_PROCESS;
    return ExecuteState_t::STATE_FINISHED;
}

void m17::ended() {}

#pragma endregion

#pragma region m18

ExecuteState_t m18::started(core::ExecutionEnivroment &m, Command_t &args)
{
    for (auto &j : m.manipulator.joints)
    {
        j->disable();
    }
    m.manipulator.effector->disable();
    return ExecuteState_t::STATE_IN_PROCESS;
}

ExecuteState_t m18::execute(core::ExecutionEnivroment &, TimeDif_ms_t time)
{
    return ExecuteState_t::STATE_FINISHED;
}

void m18::ended() {}

#pragma endregion

#pragma region m112

ExecuteState_t m112::started(core::ExecutionEnivroment &m, Command_t &args)
{
    for (auto &j : m.manipulator.joints)
    {
        j->disable();
    }
    // TODO: disable effector
    auto &c = core::coreTaskManager.getContext(core::CoreTaskKey::TASK_INPUT_FILE);
    c.setState(core::ExecuteState_t::STATE_FINISHED);
    return ExecuteState_t::STATE_IN_PROCESS;
}

ExecuteState_t m112::execute(core::ExecutionEnivroment &, TimeDif_ms_t time)
{
    return ExecuteState_t::STATE_FINISHED;
}

void m112::ended() {}

#pragma endregion

#pragma region m25

ExecuteState_t m25::started(core::ExecutionEnivroment &m, Command_t &args)
{
    auto &c = core::coreTaskManager.getContext(core::CoreTaskKey::TASK_INPUT_FILE);
    if (c.state() == core::ExecuteState_t::STATE_RUNNING)
    {
        c.setState(core::ExecuteState_t::STATE_SUSPENDED);
        for (auto &j : m.manipulator.joints)
        {
            j->setPause(true);
        }
    }
    return ExecuteState_t::STATE_IN_PROCESS;
}

ExecuteState_t m25::execute(core::ExecutionEnivroment &, TimeDif_ms_t time)
{
    return ExecuteState_t::STATE_FINISHED;
}

void m25::ended() {}

#pragma endregion

#pragma region m24

ExecuteState_t m24::started(core::ExecutionEnivroment &m, Command_t &args)
{
    auto &c = core::coreTaskManager.getContext(core::CoreTaskKey::TASK_INPUT_FILE);
    if (c.state() == core::ExecuteState_t::STATE_FINISHED)
        core::coreTaskManager.startExecutionInThread(core::CoreTaskKey::TASK_INPUT_FILE, 0);
    else
        c.setState(core::ExecuteState_t::STATE_RUNNING);
    for (auto &j : m.manipulator.joints)
    {
        j->setPause(false);
    }
    return ExecuteState_t::STATE_IN_PROCESS;
}

ExecuteState_t m24::execute(core::ExecutionEnivroment &, TimeDif_ms_t time)
{
    return ExecuteState_t::STATE_FINISHED;
}

void m24::ended() {}

#pragma endregion

#pragma region m23

ExecuteState_t m23::started(core::ExecutionEnivroment &m, Command_t &args)
{
    auto &c = core::coreTaskManager.getContext(core::CoreTaskKey::TASK_INPUT_FILE);
    if (c.state() != core::ExecuteState_t::STATE_FINISHED)
        return ExecuteState_t::STATE_ERROR;
    if (args.args.size() == 0)
    {
        return ExecuteState_t::STATE_ERROR;
    }
    auto p = args.args.begin();
    const char *name = *p;
    DEBUG_VERBOSE("File name: %s", name);
    if (setFile(name) != ExecuteState_t::STATE_IN_PROCESS)
    {
        return ExecuteState_t::STATE_ERROR;
    }
    return ExecuteState_t::STATE_IN_PROCESS;
}

ExecuteState_t m23::execute(core::ExecutionEnivroment &, TimeDif_ms_t time)
{
    return ExecuteState_t::STATE_FINISHED;
}

void m23::ended() {}

#pragma endregion