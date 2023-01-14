#pragma once

#include "inttypes.h"

namespace core
{
    using BehaviourFlag_t = uint8_t;

    enum BehaviourFlags : uint8_t
    {
        DEHAVIOUR_WAIT_INPUT = 1,
        DEHAVIOUR_FINISH_PROGRAM_IF_PARSE_ERROR = 2,
        DEHAVIOUR_FINISH_PROGRAM_IF_UNKNOWN_COMMAND = 4,
        DEHAVIOUR_FINISH_PROGRAN_IF_EXECUTION_ERROR = 8
    };
}