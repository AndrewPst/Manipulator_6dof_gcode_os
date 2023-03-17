#include "commandExecutors/g_group.h"
#include "out.h"
#include "kinematics/kinematics.h"
#include "models/angleConverter.h"
#include "global.h"

using namespace executableCommands;

constexpr static const char TIME_KEY = 'T';
constexpr static const char JOINT_0_KEY = 'A';
constexpr static const char JOINT_1_KEY = 'B';
constexpr static const char JOINT_2_KEY = 'C';
constexpr static const char JOINT_3_KEY = 'D';
constexpr static const char JOINT_4_KEY = 'E';
constexpr static const char JOINT_5_KEY = 'F';
constexpr static const char EFFECTOR_KEY = 'P';
constexpr static const char SPEED_KEY = 'S';
constexpr static const char X_COORD_KEY = 'X';
constexpr static const char Y_COORD_KEY = 'Y';
constexpr static const char Z_COORD_KEY = 'Z';
constexpr static const char A_COORD_KEY = 'A';
constexpr static const char B_COORD_KEY = 'B';
constexpr static const char G_COORD_KEY = 'G';
constexpr static const char CORD_MOVE_SPEED_KEY = 'F';

inline kinematics::CalculationResult_t calcCurPos(const Manipulator &m, Position_t &out)
{
    std::vector<double> angles(DOF);
    for (uint8_t i{0}; i < DOF; i++)
        angles[i] = degToRad(m.joints[i].get()->position());
    return kinematics::kinematics6dof.forward(angles, out);
}

#pragma region g0

const double EXECUTION_TIME_KOEF = 1.15;

double g0::_exTime{1000};


ExecuteState_t g0::started(core::ExecutionEnivroment &m, Command_t &args)
{
    // Find time paramater
    auto newTime = std::find_if(args.args.begin(), args.args.end(), [](const char *a)
                                { return a[0] == TIME_KEY; });
    if (newTime != args.args.end())
        _exTime = atof(*newTime + 1);
    if (_exTime == 0)
        return ExecuteState_t::STATE_ERROR;
    // Copy current effector pos
    _newPos = m.manipulator.effectorPos;
    _newEffValue = (*m.manipulator.effector).position();
    for (auto i = args.args.begin(); i != args.args.end(); i++)
    {
        if (*i[0] == TIME_KEY)
            continue;
        if (*i[0] == X_COORD_KEY)
            _newPos.x = atof(*i + 1);
        else if (*i[0] == Y_COORD_KEY)
            _newPos.y = atof(*i + 1);
        else if (*i[0] == Z_COORD_KEY)
            _newPos.z = atof(*i + 1);
        else if (*i[0] == A_COORD_KEY)
            _newPos.a = degToRad(atof(*i + 1));
        else if (*i[0] == B_COORD_KEY)
            _newPos.b = degToRad(atof(*i + 1));
        else if (*i[0] == G_COORD_KEY)
            _newPos.g = degToRad(atof(*i + 1));
        else if (*i[0] == EFFECTOR_KEY)
            _newEffValue = atof(*i + 1);
    }
    // set current manipulators table
    kinematics::kinematics6dof.setDHTable(m.manipulator.dhTable);
    std::vector<double> angles(DOF);
    auto r = kinematics::kinematics6dof.inverse(_newPos, angles, 0);
    if (r == kinematics::CalculationResult_t::CALC_ERROR)
        return ExecuteState_t::STATE_ERROR;
    double speed{0}; // Actuators speed
    // Сhecking that the position is achievable
    for (uint8_t i = 0; i < angles.size(); i++)
    {
        angles[i] = radToDeg(angles[i]); // Convert all angles to degrees!!!! <<<
        if (m.manipulator.joints[i].get()->posAchievable(angles[i]) == false)
            return ExecuteState_t::STATE_ERROR;
    }
    // Move actuators
    for (uint8_t i = 0; i < angles.size(); i++)
    {
        speed = abs(angles[i] - m.manipulator.joints[i]->position()) / (_exTime / 1000.0); // Calculate speed
        m.manipulator.joints[i].get()->move(angles[i], speed);
    }
    speed = abs(_newEffValue - m.manipulator.effector->position()) / (_exTime / 1000.0);
    m.manipulator.effector->move(_newEffValue, speed);
    return ExecuteState_t::STATE_IN_PROCESS;
}

ExecuteState_t g0::execute(core::ExecutionEnivroment &env, TimeDif_ms_t time)
{
    // Monitoring of current effector position
    auto r = calcCurPos(env.manipulator, env.manipulator.effectorPos);
    if (r == kinematics::CalculationResult_t::CALC_ERROR)
        return ExecuteState_t::STATE_ERROR;
    if (time >= _exTime * EXECUTION_TIME_KOEF)
    {
        // position reached
        env.manipulator.effectorPos = _newPos;
        return ExecuteState_t::STATE_FINISHED;
    }
    return ExecuteState_t::STATE_IN_PROCESS;
}
void g0::ended() {}

#pragma endregion

#pragma region g1

double g1::_speed{10};


ExecuteState_t g1::started(core::ExecutionEnivroment &m, Command_t &args)
{
    // Find time paramater
    auto speed = std::find_if(args.args.begin(), args.args.end(), [](const char *a)
                              { return a[0] == CORD_MOVE_SPEED_KEY; });
    if (speed != args.args.end())
        _speed = atof(*speed + 1);
    if (_speed == 0)
        return ExecuteState_t::STATE_ERROR;
    // Copy current effector pos
    _endPos = m.manipulator.effectorPos;
    _startPos = _endPos;
    _startEffValue = (*m.manipulator.effector).position();
    _newEffValue = _startEffValue;
    for (auto i = args.args.begin(); i != args.args.end(); i++)
    {
        if (*i[0] == CORD_MOVE_SPEED_KEY)
            continue;
        if (*i[0] == X_COORD_KEY)
            _endPos.x = atof(*i + 1);
        else if (*i[0] == Y_COORD_KEY)
            _endPos.y = atof(*i + 1);
        else if (*i[0] == Z_COORD_KEY)
            _endPos.z = atof(*i + 1);
        else if (*i[0] == A_COORD_KEY)
            _endPos.a = degToRad(atof(*i + 1));
        else if (*i[0] == B_COORD_KEY)
            _endPos.b = degToRad(atof(*i + 1));
        else if (*i[0] == G_COORD_KEY)
            _endPos.g = degToRad(atof(*i + 1));
        else if (*i[0] == EFFECTOR_KEY)
            _newEffValue = atof(*i + 1);
    }
    // set current manipulators table
    kinematics::kinematics6dof.setDHTable(m.manipulator.dhTable);
    std::vector<double> angles(DOF);
    // Сhecking that the position is achievable
    auto r = kinematics::kinematics6dof.inverse(_endPos, angles, 0);
    if (r == kinematics::CalculationResult_t::CALC_ERROR)
        return ExecuteState_t::STATE_ERROR;
    // calculate time of execution
    _exTime = sqrt((m.manipulator.effectorPos.x - _endPos.x) * (m.manipulator.effectorPos.x - _endPos.x) +
                   (m.manipulator.effectorPos.y - _endPos.y) * (m.manipulator.effectorPos.y - _endPos.y) +
                   (m.manipulator.effectorPos.z - _endPos.z) * (m.manipulator.effectorPos.z - _endPos.z)) /
              _speed * 1000;
    // If only rotation
    if (_exTime == 0)
    {
        DEBUG_VERBOSE("Rot only");
        _exTime = radToDeg(sqrt((m.manipulator.effectorPos.a - _endPos.a) * (m.manipulator.effectorPos.a - _endPos.a) +
                                (m.manipulator.effectorPos.b - _endPos.b) * (m.manipulator.effectorPos.b - _endPos.b) +
                                (m.manipulator.effectorPos.g - _endPos.g) * (m.manipulator.effectorPos.g - _endPos.g))) /
                  _speed * 1000 ;
    }
    if(_exTime == 0)
    {
        _exTime = abs(_newEffValue - _startEffValue) / _speed * 1000;
    }
    if (_exTime == 0)
        _exTime = 1;
    return ExecuteState_t::STATE_IN_PROCESS;
}

ExecuteState_t g1::execute(core::ExecutionEnivroment &env, TimeDif_ms_t time)
{
    TimeDif_ms_t diffTime = time - _lastTime;
    if (diffTime < 10)
        return ExecuteState_t::STATE_IN_PROCESS;
    Position_t newPos;
    EffectorArgType newEffValue;
    if (time >= _exTime * EXECUTION_TIME_KOEF)
    {
        newPos = _endPos;
        newEffValue = _newEffValue;
    }
    else
    {
        newPos.x = _startPos.x + (_endPos.x - _startPos.x) * (double)time / _exTime;
        newPos.y = _startPos.y + (_endPos.y - _startPos.y) * (double)time / _exTime;
        newPos.z = _startPos.z + (_endPos.z - _startPos.z) * (double)time / _exTime;
        newPos.a = _startPos.a + (_endPos.a - _startPos.a) * (double)time / _exTime;
        newPos.b = _startPos.b + (_endPos.b - _startPos.b) * (double)time / _exTime;
        newPos.g = _startPos.g + (_endPos.g - _startPos.g) * (double)time / _exTime;
        newEffValue = _startEffValue + (_newEffValue - _startEffValue) * (double)time / _exTime;
    }
    std::vector<double> angles(DOF);
    auto result = kinematics::kinematics6dof.inverse(newPos, angles, 0);
    if (result == kinematics::CalculationResult_t::CALC_ERROR)
        return ExecuteState_t::STATE_ERROR;
    // Сhecking that the position is achievable
    for (uint8_t i = 0; i < angles.size(); i++)
    {
        angles[i] = radToDeg(angles[i]); // Convert all angles to degrees!!!! <<<
        if (env.manipulator.joints[i].get()->posAchievable(angles[i]) == false)
            return ExecuteState_t::STATE_ERROR;
    }
    // Move actuators
    double speed{0}; // Actuators speed
    for (uint8_t i = 0; i < angles.size(); i++)
    {
        speed = abs(angles[i] - env.manipulator.joints[i]->position()) / ((double)diffTime / 1000.0); // Calculate speed
        env.manipulator.joints[i].get()->move(angles[i], speed);
    }
    speed = abs(newEffValue - env.manipulator.effector->position()) / ((double)diffTime / 1000.0);
    env.manipulator.effector->move(newEffValue, speed);

    env.manipulator.effectorPos = newPos;
    _lastTime = time;
    if (time >= _exTime)
    {
        return ExecuteState_t::STATE_FINISHED;
    }
    return ExecuteState_t::STATE_IN_PROCESS;
}
void g1::ended()
{
    _lastTime = 0;
}

#pragma endregion

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

ExecuteState_t g5::started(core::ExecutionEnivroment &env, Command_t &args)
{
    auto tspeed = std::find_if(args.args.begin(), args.args.end(), [](const char *a)
                               { return a[0] == SPEED_KEY; });
    if (tspeed != args.args.end())
        _speed = atof(*tspeed + 1);
    if (_speed == 0)
        return ExecuteState_t::STATE_ERROR;
    std::vector<double> newAngles(DOF);
    for (uint8_t i{0}; i < DOF; i++)
        newAngles[i] = degToRad(env.manipulator.joints[i].get()->position());
    double max_dif = 0;
    for (auto i = args.args.begin(); i != args.args.end(); i++)
    {
        if (*i[0] == SPEED_KEY)
            continue;
        if (*i[0] == EFFECTOR_KEY)
        {
            _newEffValue = atof(*i + 1);
            env.manipulator.effector->move(_newEffValue, _speed);
            continue;
        }
        uint8_t index = *i[0] - JOINT_0_KEY;
        if (index >= DOF)
            return ExecuteState_t::STATE_ERROR;
        double value = atof(*i + 1);
        newAngles[index] = env.manipulator.joints[index]->move(value, _speed);
        max_dif = std::max(max_dif, abs(newAngles[index] - env.manipulator.joints[index]->position()));
        newAngles[index] = degToRad(newAngles[index]);
    }
    auto r = kinematics::kinematics6dof.forward(newAngles, _newPos);
    if (r == kinematics::CalculationResult_t::CALC_ERROR)
        return ExecuteState_t::STATE_ERROR;
    _exTime = max_dif / _speed * 1000.0;
    return ExecuteState_t::STATE_IN_PROCESS;
}

ExecuteState_t g5::execute(core::ExecutionEnivroment &env, TimeDif_ms_t time)
{
    auto r = calcCurPos(env.manipulator, env.manipulator.effectorPos);
    if (r == kinematics::CalculationResult_t::CALC_ERROR)
        return ExecuteState_t::STATE_ERROR;
    if (time >= _exTime * EXECUTION_TIME_KOEF)
    {
        env.manipulator.effectorPos = _newPos;
        return ExecuteState_t::STATE_FINISHED;
    }
    return ExecuteState_t::STATE_IN_PROCESS;
}
void g5::ended() {}

#pragma endregion

#pragma region g6

double g6::_exTime{1000};

ExecuteState_t g6::started(core::ExecutionEnivroment &env, Command_t &args)
{
    auto stime = std::find_if(args.args.begin(), args.args.end(), [](const char *a)
                              { return a[0] == TIME_KEY; });
    if (stime != args.args.end())
        _exTime = atof(*stime + 1);
    if (_exTime == 0)
        return ExecuteState_t::STATE_ERROR;
    std::vector<double> newAngles(DOF);
    for (uint8_t i{0}; i < DOF; i++)
        newAngles[i] = degToRad(env.manipulator.joints[i].get()->position());
    for (auto i = args.args.begin(); i != args.args.end(); i++)
    {
        if (*i[0] == TIME_KEY)
            continue;
        if (*i[0] == EFFECTOR_KEY)
        {
            _newEffValue = atof(*i + 1);
            double speed = abs(_newEffValue - env.manipulator.effector->position()) / (_exTime / 1000.0);
            env.manipulator.effector->move(_newEffValue, speed);
            continue;
        }
        uint8_t index = *i[0] - JOINT_0_KEY;
        if (index >= DOF)
            return ExecuteState_t::STATE_ERROR;
        double value = atof(*i + 1);
        double speed = abs(value - env.manipulator.joints[index]->position()) / (_exTime / 1000.0);
        newAngles[index] = degToRad(env.manipulator.joints[index]->move(value, speed));
    }
    auto r = kinematics::kinematics6dof.forward(newAngles, _newPos);
    if (r == kinematics::CalculationResult_t::CALC_ERROR)
        return ExecuteState_t::STATE_ERROR;
    return ExecuteState_t::STATE_IN_PROCESS;
}

ExecuteState_t g6::execute(core::ExecutionEnivroment &env, TimeDif_ms_t time)
{
    auto r = calcCurPos(env.manipulator, env.manipulator.effectorPos);
    if (r == kinematics::CalculationResult_t::CALC_ERROR)
        return ExecuteState_t::STATE_ERROR;
    if (time >= _exTime * EXECUTION_TIME_KOEF)
    {
        env.manipulator.effectorPos = _newPos;
        return ExecuteState_t::STATE_FINISHED;
    }
    return ExecuteState_t::STATE_IN_PROCESS;
}
void g6::ended() {}

#pragma endregion