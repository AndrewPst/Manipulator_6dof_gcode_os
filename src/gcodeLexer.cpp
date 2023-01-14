#include "gcodeLexer.h"

#include "strParser.h"
#include "models/Command_t.h"

GcodeLexer::GcodeLexer()
{
}

constexpr static const char GCODE_KEY_LIST[] = "GMDT";

uint8_t GcodeLexer::parse(const ParsedStr_t &str, Command_t &out)
{
    if (str.count() == 0)
        return 1;
    uint8_t index_offset{0};
    char k = str.at(0)[0];
    for (uint8_t i = 0; i < sizeof(GCODE_KEY_LIST) - 1; i++)
    {
        if (k == GCODE_KEY_LIST[i])
        {
            index_offset = 1;
            out.Key = str.at(0);
            break;
        }
    }
    out.args.resize(str.count() - index_offset);
    int16_t i = 0;
    for (; i < str.count() - 1; i++)
    {
        out.args[i] = str.at(i + index_offset);
    }
    return 0;
}