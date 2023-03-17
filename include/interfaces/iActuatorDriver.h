#pragma once

#include "inttypes.h"

class IActuatorDriver
{
public:

    typedef double TimeDiff_ms_t;

    virtual void setValue(double) = 0;
    virtual double value() const  = 0;

    virtual void tick(TimeDiff_ms_t) = 0;
};