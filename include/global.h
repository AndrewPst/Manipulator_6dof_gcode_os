#pragma once

#include "defines.h"

class GcodeLexer;
class StrParser;
class SdCardManager;

extern GcodeLexer gcodeLexer;
extern StrParser strParser;
extern SdCardManager sdCard;

namespace core
{
    class Core;
    extern Core core;

    template <typename _ThreadT>
    class CoreTaskManager;

    extern CoreTaskManager<ThreadT> coreTaskManager;
}
