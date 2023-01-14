#pragma once

#include <vector>

struct Command_t
{
    const char *Key{0};
    std::vector<const char*> args;
};