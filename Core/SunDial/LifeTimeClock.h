#pragma once
#include "timedefines.h"

template <typename ClockTypeTimePoint, typename ClockType, typename TimeType>
class LifeTimeClock
{
private:
	TimeUnitType& ref;
	ClockTypeTimePoint start;

public:
	LifeTimeClock(TimeUnitType& ref) : ref(ref)
	{
		start = ClockType::now();
	}
	~LifeTimeClock()
	{
		ref = std::chrono::duration_cast<TimeType>(ClockType::now() - start).count();
	}
};

//A clock that outputs its lifetime into the given reference with TimeUnit std::chrono::nanoseconds
//Anything much much smaller than a second should be measured as such
//Uses std::high_resolution_clock internally
#define MiniLifeTimeClock(time_nanoseconds) LifeTimeClock<std::chrono::high_resolution_clock::time_point, std::chrono::high_resolution_clock, std::chrono::nanoseconds>(time_nanoseconds)

//A clock that outputs its lifetime into the given reference with TimeUnit std::chrono::microseconds
//Anything smaller than a second should be measured as such
//Uses std::high_resolution_clock internally
#define ShortLifeTimeClock(time_microseconds) LifeTimeClock<std::chrono::high_resolution_clock::time_point, std::chrono::high_resolution_clock, std::chrono::microseconds>(time_microseconds)

//A clock that outputs its lifetime into the given reference with TimeUnit std::chrono::milliseconds
//Anything smaller than an hour can be measured as such
//Uses std::steady_clock internally
#define MedLifeTimeClock(time_milliseconds) LifeTimeClock<std::chrono::steady_clock::time_point, std::chrono::steady_clock, std::chrono::milliseconds>(time_milliseconds)

//A clock that outputs its lifetime into the given reference with TimeUnit std::chrono::seconds
//Anything longer than an hour should be measured as such
//Uses std::system_clock internally
#define LongLifeTimeClock(time_seconds) LifeTimeClock<std::chrono::system_clock::time_point, std::chrono::system_clock, std::chrono::seconds>(time_seconds)
