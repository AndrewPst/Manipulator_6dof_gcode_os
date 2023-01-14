#include "global.h"

#include "core/core.h"
#include "core/coreTaskManager.h"

#include "gcodeLexer.h"
#include "strParser.h"
#include "sdCardManager.h"

GcodeLexer gcodeLexer;
StrParser strParser({' ', '\n', ',', '\t'});
SdCardManager sdCard;

namespace core
{
    Core core;
    CoreTaskManager<ThreadT> coreTaskManager;
}