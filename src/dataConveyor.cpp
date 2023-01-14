#include "dataConveyor.h"

#include "models/Command_t.h"
#include "strParser.h"
#include "gcodeLexer.h"
#include "global.h"

uint8_t EasyDataConveyor::parse(char *buffer, Command_t &out)
{
    ParsedStr_t parsed;
    strParser.parse(buffer, parsed);
    return gcodeLexer.parse(parsed, out);
}