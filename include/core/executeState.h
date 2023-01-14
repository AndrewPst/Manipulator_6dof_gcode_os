#pragma once

#include "inttypes.h"

namespace core
{

    enum ExecuteState_t : uint8_t
    {
        STATE_UNKNOWN = 0,
        STATE_RUNNING,
        STATE_SUSPENDED,
        STATE_FINISHED
    };
}