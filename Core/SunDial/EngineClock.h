#pragma once
#include <array>
#include "Core/Monument/Monument.h"
#include "timedefines.h"

//A clock that can be used to time various things
//Refrain from using across threads
//Uses microseconds internally
class EngineBaseClock
{
private:
	EngineClock_Duration m_timeElapsed;

	EngineClock_TimePoint m_lastQueryPoint;
	EngineClock_TimePoint m_currentQueryPoint; //temp data storage
	
public:
	EngineBaseClock();
	~EngineBaseClock();

	//Queries the current time and updates the time elapsed since last second
	EngineClock_Duration QueryTime();
	//Check the time elapsed
	EngineClock_Duration GetTimeElapsed() const { return m_timeElapsed; }
	//Check time point of last query
	EngineClock_TimePoint GetCurrentTimePoint() const { return m_currentQueryPoint; }
};

//size of the array holding past queries, must be a power of 2 for fast modulus of query indices
constexpr int ENGINECLOCK_INTERNAL_TIMESLOTS = 8;
constexpr int ENGINECLOCK_INTERNAL_TIMESLOTS_MINUS_ONE = ENGINECLOCK_INTERNAL_TIMESLOTS - 1;
compile_assert(ENGINECLOCK_INTERNAL_TIMESLOTS >= 0 && (ENGINECLOCK_INTERNAL_TIMESLOTS_MINUS_ONE & ENGINECLOCK_INTERNAL_TIMESLOTS) == 0);
//percentage of qps variance which when passed, counts as a hitch
constexpr double ENGINECLOCK_INTENAL_HITCH_VARIANCE = 0.2;

//A more advanced clock that uses the EngineBaseClock to keep time
//This will be fitted with average qps(queries per second), highest qps, lowest qps, 
//	and actual qsls(queries since last second)
//This will also be fitted with an anti-hitch system, hitching being periods where the program freezes and more time
//	pass as a result which would typically explode certain calculations
//This class is not thread safe
class CtrlEngineClock
{
	using ClockTimeType = EngineClock_Microseconds;
private:
	typedef size_t QueryIndex;

	EngineBaseClock m_internalClock;
	std::array<EngineClock_Duration, ENGINECLOCK_INTERNAL_TIMESLOTS> m_pastQueries;
	QueryIndex m_currentIndex;
	QueryIndex m_lastIndex;

	EngineClock_Duration m_PQTotal; //past queries total
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
	EngineClock_Duration QueryTime();

	TimeSeconds GetSecondsElapsedModifiedForHitch() const;
	TimeSeconds GetTrueSecondsElapsed() const;

	//Check the time elapsed
	EngineClock_Duration GetTimeElapsed() const { return m_pastQueries[m_lastIndex]; }
	TimeRate getQPS() const { return m_QPS; }
	TimeRate getAvgQPS() const { return m_averageQPS; }
	TimeRate getLowQPS() const { return m_lowestQPS; }
	TimeRate getHighQPS() const { return m_highestQPS; }

	inline EngineClock_TimePoint GetCurrentTimePoint() const { return m_internalClock.GetCurrentTimePoint(); }
};