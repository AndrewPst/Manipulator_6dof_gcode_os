#pragma once

#include <inttypes.h>

struct Command_t;
namespace core
{
    struct ExecutionEnivroment;
}

enum ExecuteState_t : uint8_t
{
    STATE_UNKNOWN = 0,
    STATE_IN_PROCESS,
    STATE_FINISHED,
    STATE_ERROR,
};

struct IExecutableCommand
{
public:
    typedef uint32_t TimeDif_ms_t;

    virtual ::ExecuteState_t started(core::ExecutionEnivroment &, Command_t &) = 0;
    virtual ::ExecuteState_t execute(core::ExecutionEnivroment &, TimeDif_ms_t) = 0;
    virtual void ended() = 0;
    virtual ~IExecutableCommand(){}
};
