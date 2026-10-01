#include "pch.h"
#include <random>

static std::random_device s_randomDevice;
static std::mt19937 s_randomGen(s_randomDevice());

int genRandInt(int lower, int upper)
{
    std::uniform_int_distribution<> distr(lower, upper);
    return distr(s_randomGen);
}