#pragma once

class IActuatorDriver
{
public:
    virtual void setValue(double) = 0;
    virtual double value() const  = 0;

    virtual void tick() = 0;
};