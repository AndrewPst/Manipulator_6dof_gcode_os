#pragma once

#include "defines.h"

enum EffectorState : uint8_t
{
    ESTATE_UNKNOWN = 0,
    ESTATE_ERROR,
    ESTATE_IN_PROCESS,
    ESTATE_SUCESSFULL,
};

class IEffector
{
public:
    IEffector() = default;

    virtual void initializate()=0;

    virtual void enable() = 0;
    virtual void disable() = 0;

    virtual void move(const EffectorArgType &, double);
    virtual const EffectorArgType &position() const;

    virtual EffectorState state();

    virtual void stop() = 0;
    virtual void setPause(bool) = 0;

    virtual ~IEffector() {}
};