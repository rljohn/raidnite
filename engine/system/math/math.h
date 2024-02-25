#pragma once

#include <cmath>

bool AreApproxEqual(float a, float b, float epsilon = 0.0001f) 
{
    return std::fabs(a - b) < epsilon;
}