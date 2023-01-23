#include "actuators/servoJoint.h"

#include "vector"

ServoJoint::ServoJoint(const std::vector<ServoDriver *> &servo) : _servo(servo) {}

void ServoJoint::initialize()
{
    for (auto s : _servo)
    {
        s->setValue(0 + _offset);
    }
}

void ServoJoint::enable()
{
    for (auto s : _servo)
    {
        s->enable();
        thread_sleep_for(10);
    }
}

void ServoJoint::disable()
{
    for (auto s : _servo)
    {
        s->disable();
    }
}

double ServoJoint::position() const
{
    return _servo[0]->value() - _offset;
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

void ServoJoint::move(double pos, double speed)
{
    for (auto s : _servo)
    {
        s->setAngleBySpeed(pos + _offset, speed);
    }
}
