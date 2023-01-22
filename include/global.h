#pragma once

#include "defines.h"

//if delete this include prog not compiling :(
#include "models/threadRegistrator.h"

class GcodeLexer;
class StrParser;
class SdCardManager;
//class ThreadRegistrator;

extern GcodeLexer gcodeLexer;
extern StrParser strParser;
extern SdCardManager sdCard;

namespace core
{
    class Core;
    extern Core core;

    template <typename _ThreadRegistrator>
    class CoreTaskManager;

    extern CoreTaskManager<ThreadRegistrator> coreTaskManager;
}
