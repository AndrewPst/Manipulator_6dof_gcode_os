#pragma once

#include "interfaces/iJoint.h"
#include <memory>
#include <config.h>
#include <vector>

struct Manipulator
{
    std::vector<std::unique_ptr<IJoint>> joints;
};