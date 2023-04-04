#include "actuators/gripper.h"

#include "out.h"
#include "models/angleConverter.h"

ServoGripper::ServoGripper(ServoDriver &servo) : _servo(servo)
{
}

void ServoGripper::initializate()
{
    double angle = acos((_max / 2.0 - _offset) / _radius);
    angle = radToDeg(angle);
    _servo.initializate(angle);
}

void ServoGripper::enable()
{
    DEBUG_INFO("Init gripper done");
    _servo.enable();
}

void ServoGripper::disable()
{
    DEBUG_INFO("Deinit gripper done");
    _servo.disable();
}

void ServoGripper::move(const EffectorArgType &v, double speed)
{
    // DEBUG_INFO("Set: %f", v);
    // _value = v;
    double angle = acos((v / 2.0 - _offset) / _radius);
    angle = radToDeg(angle);
    _servo.setAngleBySpeed(angle, speed*2); // not linear speed (TODO: repair this) and x2 speed to compensate for 2 fingers 
    //DEBUG_INFO("Value: %f\tAngle: %f\t Speed: %f", v, angle, speed);
}

const double &ServoGripper::position() const
{
    _value = (cos(degToRad(_servo.value())) * _radius + _offset) * 2.0;
    //DEBUG_INFO("Out: %f", _value);
    return _value;
}

void ServoGripper::stop()
{
    _servo.stop();
}

void ServoGripper::setPause(bool pause)
{
    _servo.setPause(pause);
}

EffectorState ServoGripper::state()
{
    return _servo.isRotation() ? EffectorState::ESTATE_IN_PROCESS : EffectorState::ESTATE_SUCESSFULL;
}
