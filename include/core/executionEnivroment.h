#pragma once

#include "models/manipulator.h"
#include "models/position.h"
#include <map>

namespace core
{
    struct ExecutionEnivroment
    {
        Manipulator manipulator;
        char *last_executable_command_key{0};
        struct
        {
            uint8_t calc_system{0};
            uint8_t unit_type{0};
        } global_variables;
    };
}