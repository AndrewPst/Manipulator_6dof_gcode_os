#pragma once
#include <vector>
#include "out.h"
#include <inttypes.h>

enum JointError : uint8_t
{
    JERROR_UNKNOWN = 0,
    JERROR_OUT_OF_RANGE,
    JERROR_TARGET_AND_REAL_VALUE_DESCREPANCY,
};

enum JointState : uint8_t
{
    JSTATE_UNKNOWN = 0,
    JSTATE_ERROR,
    JSTATE_IN_PROCESS,
    JSTATE_SUCESSFULL,
};

typedef struct
{
    JointState state;
    uint8_t key;
} JointState_t;

class IJoint
{

public:
    virtual void initialize() = 0;

    virtual void enable() = 0;
    virtual void disable() = 0;

    virtual void setPause(bool) = 0;

    virtual void move(double, double) = 0;
    virtual double position() const = 0;
    virtual JointState_t state() = 0;

    virtual ~IJoint() = default;
};
