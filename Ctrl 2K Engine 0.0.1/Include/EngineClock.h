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

class LifeTimeClock_InternalStorage
{
protected:
	TimeUnitType& ref;

public:
	LifeTimeClock_InternalStorage(TimeUnitType& ref);
};

//A clock that outputs its lifetime into the given reference with TimeUnit std::chrono::nanoseconds
//Anything much much smaller than a second should be measured as such
//Uses std::high_resolution_clock internally
class MiniLifeTimeClock : public LifeTimeClock_InternalStorage
{
private:
	std::chrono::high_resolution_clock::time_point start;

public:
	MiniLifeTimeClock(EngineClock_Nanoseconds& ref);
	~MiniLifeTimeClock();
};

//A clock that outputs its lifetime into the given reference with TimeUnit std::chrono::microseconds
//Anything smaller than a second should be measured as such
//Uses std::high_resolution_clock internally
class ShortLifeTimeClock : public LifeTimeClock_InternalStorage
{
private:
	std::chrono::high_resolution_clock::time_point start;

public:
	ShortLifeTimeClock(EngineClock_Microseconds& ref);
	~ShortLifeTimeClock();
};

//A clock that outputs its lifetime into the given reference with TimeUnit std::chrono::milliseconds
//Anything smaller than an hour can be measured as such
//Uses std::steady_clock internally
class MedLifeTimeClock : public LifeTimeClock_InternalStorage
{
private:
	std::chrono::steady_clock::time_point start;

public:
	MedLifeTimeClock(EngineClock_Milliseconds& ref);
	~MedLifeTimeClock();
};

//A clock that outputs its lifetime into the given reference with TimeUnit std::chrono::seconds
//Anything longer than an hour should be measured as such
//Uses std::system_clock internally
class LongLifeTimeClock : public LifeTimeClock_InternalStorage
{
private:
	std::chrono::system_clock::time_point start;

public:
	LongLifeTimeClock(EngineClock_Seconds& ref);
	~LongLifeTimeClock();
};

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
	std::chrono::high_resolution_clock::duration m_durationTimeElapsed; //temp data storage
	
public:
	EngineBaseClock();
	~EngineBaseClock();

	//Queries the current time and updates the time elapsed since last second
	const EngineClock_Microseconds& QueryTime();
	//Check the time elapsed
	const EngineClock_Microseconds& GetTimeElapsed();
};

//size of the array holding past queries
#define ENGINECLOCK_INTERNAL_TIMESLOTS 5
#define SECONDS_PER_MILLISECOND 1e-3
#define SECONDS_PER_MICROSECOND 1e-6
#define SECONDS_PER_NANOSECOND 1e-9

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
	//Check the time elapsed
	const EngineClock_Microseconds& GetTimeElapsed();
	const TimeSeconds GetSecondsElapsedModifiedForHitch();
	const TimeSeconds GetTrueSecondsElapsed();
	const TimeRate& getQPS();
	const TimeRate& getAvgQPS();
	const TimeRate& getLowQPS();
	const TimeRate& getHighQPS();
};