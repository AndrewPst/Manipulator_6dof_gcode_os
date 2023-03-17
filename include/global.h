#pragma once

#include "defines.h"
#include "config.h"

//if delete this include prog not compiling :(
#include "models/threadRegistrator.h"

class GcodeLexer;
class StrParser;
class SdCardManager;

extern GcodeLexer gcodeLexer;
extern StrParser strParser;
extern SdCardManager sdCard;

namespace kinematics
{
    template<int _Dof>
    class Kinematics;

    extern Kinematics<DOF> kinematics6dof;
}

namespace core
{
    class Core;
    extern Core core;

    template <typename _ThreadRegistrator>
    class CoreTaskManager;

    extern CoreTaskManager<ThreadRegistrator> coreTaskManager;
}
