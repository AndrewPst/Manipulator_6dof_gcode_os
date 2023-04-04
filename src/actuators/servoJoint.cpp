#include "actuators/servoJoint.h"

#include "vector"

ServoJoint::ServoJoint(const std::vector<ServoDriver *> &servo) : _servo(servo) {}

void ServoJoint::initialize()
{
    for (auto s : _servo)
    {
        s->initializate(0 + _offset);
    }
}

void ServoJoint::enable()
{
    for (auto s : _servo)
    {
        s->enable();
    }
}

void ServoJoint::disable()
{
    for (auto s : _servo)
    {
        s->disable();
    }
}

void ServoJoint::stop()
{
    for(auto s : _servo)
    {
        s->stop();
    }
}

double ServoJoint::position() const
{
    return _servo[0]->value() - _offset;
}

bool ServoJoint::posAchievable(double pos)
{
    pos += _offset;
    for (auto s : _servo)
    {
        if (pos < s->min() || pos > s->max())
            return false;
    }
    return true;
}

void ServoJoint::setPause(bool p)
{
    for (auto s : _servo)
    {
        s->setPause(p);
    }
}

JointState_t ServoJoint::state()
{
    bool state = true;
    for (auto s : _servo)
    {
        if (s->isRotation())
            state = false;
    }
    if (state)
        return {JointState::JSTATE_SUCESSFULL};
    else
        return {JointState::JSTATE_IN_PROCESS};
}

double ServoJoint::move(double pos, double speed)
{
    pos += _offset;
    pos = pos > _servo[0]->max() ? _servo[0]->max() : pos < _servo[0]->min() ? _servo[0]->min() : pos;
    for (auto s : _servo)
    {
        s->setAngleBySpeed(pos, speed);
    }
    return pos - _offset;
}
