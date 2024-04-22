#pragma once
#include <chrono>
#include <array>

typedef long TimeUnitType;
typedef double TimeRate;
typedef double TimeSeconds;

typedef TimeUnitType EngineClock_Nanoseconds;
typedef TimeUnitType EngineClock_Microseconds;
typedef TimeUnitType EngineClock_Milliseconds;
typedef TimeUnitType EngineClock_Seconds;

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

//A clock that can be used to time various things
//Refrain from using across threads
//Uses microseconds internally
class EngineBaseClock
{
private:
	typedef std::chrono::microseconds ClockTimeUnit;

	EngineClock_Microseconds m_timeElapsed;

	std::chrono::high_resolution_clock::time_point m_lastQueryPoint;
	std::chrono::high_resolution_clock::time_point m_currentQueryPoint; //temp data storage
	
public:
	EngineBaseClock();
	~EngineBaseClock();

	//Queries the current time and updates the time elapsed since last second
	const EngineClock_Microseconds& QueryTime();
	//Check the time elapsed
	const EngineClock_Microseconds& GetTimeElapsed()
	{
		return m_timeElapsed;
	}
};

//size of the array holding past queries
#define ENGINECLOCK_INTERNAL_TIMESLOTS 8
//percentage of qps variance which when passed, counts as a hitch
#define ENGINECLOCK_INTENAL_HITCH_VARIANCE 0.2

#define SECONDS_PER_MILLISECOND 1e-3
#define SECONDS_PER_MICROSECOND 1e-6
#define SECONDS_PER_NANOSECOND 1e-9
#define MILLISECONDS_TO_SECONDS(ms) (SECONDS_PER_MILLISECOND * (ms))
#define MICROSECONDS_TO_SECONDS(us) (SECONDS_PER_MICROSECOND * (us))
#define NANOSECONDS_TO_SECONDS(ns) (SECONDS_PER_NANOSECOND * (ns))

//A more advanced clock that uses the EngineBaseClock to keep time
//This will be fitted with average qps(queries per second), highest qps, lowest qps, 
//	and actual qsls(queries since last second)
//This will also be fitted with an anti-hitch system, hitching being periods where the program freezes and more time
//	pass as a result which would typically explode certain calculations
//This class is not thread safe
class CtrlEngineClock
{
private:
	typedef size_t QueryIndex;

	EngineBaseClock m_internalClock;
	std::array<EngineClock_Microseconds, ENGINECLOCK_INTERNAL_TIMESLOTS> m_pastQueries;
	QueryIndex m_currentIndex;
	QueryIndex m_lastIndex;

	EngineClock_Microseconds m_PQTotal; //past queries total
	QueryIndex m_PQLowestIndex; //index to lowest time in past queries
	QueryIndex m_PQHighestIndex; //index to highest time in past queries
	
	TimeRate m_QPS; //current queries per second
	TimeRate m_averageQPS; //average queries per second
	TimeRate m_lowestQPS; //lowest queries per second
	TimeRate m_highestQPS; //highest queries per second

public:
	CtrlEngineClock();
	~CtrlEngineClock();

	//Queries the current time and updates the time elapsed since last second
	//Also updates all internal data
	const EngineClock_Microseconds& QueryTime();

	const TimeSeconds GetSecondsElapsedModifiedForHitch();
	const TimeSeconds GetTrueSecondsElapsed();

	//Check the time elapsed
	const EngineClock_Microseconds& GetTimeElapsed()
	{
		return m_pastQueries[m_lastIndex];
	}
	const TimeRate& getQPS()
	{
		return m_QPS;
	}
	const TimeRate& getAvgQPS()
	{
		return m_averageQPS;
	}
	const TimeRate& getLowQPS()
	{
		return m_lowestQPS;
	}
	const TimeRate& getHighQPS()
	{
		return m_highestQPS;
	}
};