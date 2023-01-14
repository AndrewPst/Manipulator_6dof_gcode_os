#pragma once

#include <array>

template <char _Dof>
struct DHTable_t
{
    std::array<double, _Dof> theta;
    std::array<double, _Dof> alfa;
    std::array<double, _Dof> d;
    std::array<double, _Dof> r;
};