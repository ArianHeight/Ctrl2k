#pragma once
#include <chrono>

typedef size_t TimeUnitType;
typedef double TimeRate;
typedef double TimeSeconds;

typedef TimeUnitType EngineClock_Nanoseconds;
typedef TimeUnitType EngineClock_Microseconds;
typedef TimeUnitType EngineClock_Milliseconds;
typedef TimeUnitType EngineClock_Seconds;

constexpr double SECONDS_PER_MILLISECOND = 1e-3;
constexpr double SECONDS_PER_MICROSECOND = 1e-6;
constexpr double SECONDS_PER_NANOSECOND = 1e-9;
#define MILLISECONDS_TO_SECONDS(ms) (SECONDS_PER_MILLISECOND * (ms))
#define MICROSECONDS_TO_SECONDS(us) (SECONDS_PER_MICROSECOND * (us))
#define NANOSECONDS_TO_SECONDS(ns) (SECONDS_PER_NANOSECOND * (ns))

typedef std::chrono::high_resolution_clock::time_point EngineClock_TimePoint;
typedef std::chrono::high_resolution_clock::duration EngineClock_Duration;