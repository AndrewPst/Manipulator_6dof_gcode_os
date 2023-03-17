#include "global.h"

#include "models/threadRegistrator.h"

#include "kinematics/kinematics.h"

#include "core/core.h"
#include "core/coreTaskManager.h"

#include "gcodeLexer.h"
#include "strParser.h"
#include "sdCardManager.h"

GcodeLexer gcodeLexer;
StrParser strParser({' ', '\n', ',', '\t'});
SdCardManager sdCard;

namespace kinematics
{
    Kinematics<DOF> kinematics6dof;
}

namespace core
{
    Core core;
    CoreTaskManager<ThreadRegistrator> coreTaskManager;
}