#include "commandExecutors/g_group.h"
#include "out.h"

using namespace executableCommands;

constexpr static const char TIME_KEY = 'T';
constexpr static const char JOINT_0_KEY = 'A';
constexpr static const char JOINT_1_KEY = 'B';
constexpr static const char JOINT_2_KEY = 'C';
constexpr static const char JOINT_3_KEY = 'D';
constexpr static const char JOINT_4_KEY = 'E';
constexpr static const char JOINT_5_KEY = 'F';
constexpr static const char EFFECTOR_KEY = 'G';
constexpr static const char SPEED_KEY = 'S';

#pragma region g4

double g4::pause_time{0};

ExecuteState_t g4::started(core::ExecutionEnivroment &m, Command_t &args)
{
    if (args.args.size() == 0)
        return ExecuteState_t::STATE_IN_PROCESS;
    if (args.args[0][0] == 'P')
        pause_time = atof(args.args[0] + 1);
    if (args.args[0][0] == 'S')
        pause_time = atof(args.args[0] + 1) * 1000;
    return ExecuteState_t::STATE_IN_PROCESS;
}

ExecuteState_t g4::execute(core::ExecutionEnivroment &, TimeDif_ms_t time)
{
    if (time >= pause_time)
    {
        return ExecuteState_t::STATE_FINISHED;
    }
    return ExecuteState_t::STATE_IN_PROCESS;
}
void g4::ended() {}

#pragma endregion

#pragma region g5

double g5::_speed{30};

ExecuteState_t g5::started(core::ExecutionEnivroment &m, Command_t &args)
{
    auto r = std::find_if(args.args.begin(), args.args.end(), [](const char *a)
                          { return a[0] == SPEED_KEY; });
    if (r != args.args.end())
        _speed = atof(*r + 1);
    double max_dif = 0;
    for (auto i = args.args.begin(); i != args.args.end(); i++)
    {
        if (*i[0] == SPEED_KEY)
            continue;
        if (*i[0] == EFFECTOR_KEY)
            continue;
        uint8_t index = *i[0] - JOINT_0_KEY;
        if (index >= DOF)
            return ExecuteState_t::STATE_ERROR;
        double value = atof(*i + 1);
        max_dif = std::max(max_dif, abs(value - m.manipulator.joints[index]->position()));
        m.manipulator.joints[index]->move(value, _speed);
    }
    _exTime = max_dif / _speed * 1000.0;
    DEBUG_VERBOSE("Exe time:  %f", _exTime);
    return ExecuteState_t::STATE_IN_PROCESS;
}

ExecuteState_t g5::execute(core::ExecutionEnivroment &, TimeDif_ms_t time)
{
    if (time >= _exTime)
    {
        return ExecuteState_t::STATE_FINISHED;
    }
    return ExecuteState_t::STATE_IN_PROCESS;
}
void g5::ended() {}

#pragma endregion

#pragma region g6

double g6::_exTime{1000};

ExecuteState_t g6::started(core::ExecutionEnivroment &m, Command_t &args)
{
    auto r = std::find_if(args.args.begin(), args.args.end(), [](const char *a)
                          { return a[0] == TIME_KEY; });
    if (r != args.args.end())
        _exTime = atof(*r + 1);
    for (auto i = args.args.begin(); i != args.args.end(); i++)
    {
        if (*i[0] == TIME_KEY)
            continue;
        if (*i[0] == EFFECTOR_KEY)
            continue;
        uint8_t index = *i[0] - JOINT_0_KEY;
        if (index >= DOF)
            return ExecuteState_t::STATE_ERROR;
        double value = atof(*i + 1);
        double speed = abs(value - m.manipulator.joints[index]->position()) / (_exTime / 1000.0);
        m.manipulator.joints[index]->move(value, speed);
    }
    DEBUG_VERBOSE("Exe time: %f", _exTime);
    return ExecuteState_t::STATE_IN_PROCESS;
}

ExecuteState_t g6::execute(core::ExecutionEnivroment &, TimeDif_ms_t time)
{
    if (time >= _exTime)
    {
        return ExecuteState_t::STATE_FINISHED;
    }
    return ExecuteState_t::STATE_IN_PROCESS;
}
void g6::ended() {}

#pragma endregion