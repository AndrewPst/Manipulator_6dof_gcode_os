#pragma once

#include "interfaces/iEffector.h"
#include "actuators/servo.h"
#include "defines.h"

class ServoGripper : public IEffector
{
private:
    const double _min{0}, _max{46};
    const double _radius = 15;
    const double _offset{11};

    mutable EffectorArgType _value;
    ServoDriver &_servo;

public:
    ServoGripper(ServoDriver &);

    void initializate() override;

    void enable() override;
    void disable() override;

    void move(const EffectorArgType &, double) override;
    const EffectorArgType &position() const override;

    void stop() override;
    void setPause(bool) override;

    EffectorState state() override;
};