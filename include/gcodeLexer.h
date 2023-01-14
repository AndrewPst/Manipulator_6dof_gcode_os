#pragma once

#include "inttypes.h"

struct ParsedStr_t;
struct Command_t;

class GcodeLexer
{
public:
    
    GcodeLexer();

    uint8_t parse(const ParsedStr_t&, Command_t& out);
};