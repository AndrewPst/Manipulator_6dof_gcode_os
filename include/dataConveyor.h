#pragma once

#include "inttypes.h"
struct Command_t;

class EasyDataConveyor
{
public:
    uint8_t parse(char *buffer, Command_t &out);
};