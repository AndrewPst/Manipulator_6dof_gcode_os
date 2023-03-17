#pragma once

#include "interfaces/iJoint.h"
#include "models/position.h"
#include "models/dhTable.h"
#include "defines.h"
#include "interfaces/iEffector.h"
#include <memory>
#include <config.h>
#include <vector>

struct Manipulator
{
    DHTable_t<DOF> dhTable;
    std::vector<std::unique_ptr<IJoint>> joints;
    Position_t effectorPos;
    std::unique_ptr<IEffector> effector;
};