#include <math.h>

#include "EngineClock.h"

LifeTimeClock_InternalStorage::LifeTimeClock_InternalStorage(TimeUnitType& ref) : ref(ref) {}


MiniLifeTimeClock::MiniLifeTimeClock(EngineClock_Nanoseconds& ref) : LifeTimeClock_InternalStorage(ref)
{
	start = std::chrono::high_resolution_clock::now();
}

MiniLifeTimeClock::~MiniLifeTimeClock()
{
	ref = abs(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count());
}


ShortLifeTimeClock::ShortLifeTimeClock(EngineClock_Microseconds& ref) : LifeTimeClock_InternalStorage(ref)
{
	start = std::chrono::high_resolution_clock::now();
}

ShortLifeTimeClock::~ShortLifeTimeClock()
{
	ref = abs(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count());
}


MedLifeTimeClock::MedLifeTimeClock(EngineClock_Milliseconds& ref) : LifeTimeClock_InternalStorage(ref)
{
	start = std::chrono::steady_clock::now();
}

MedLifeTimeClock::~MedLifeTimeClock()
{
	ref = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
}


LongLifeTimeClock::LongLifeTimeClock(EngineClock_Seconds& ref) : LifeTimeClock_InternalStorage(ref)
{
	start = std::chrono::system_clock::now();
}

LongLifeTimeClock::~LongLifeTimeClock()
{
	ref = abs(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start).count());
}


//empty cstr for now
EngineBaseClock::EngineBaseClock() : m_lastQueryPoint(std::chrono::high_resolution_clock::now()), m_timeElapsed(0) {}

//empty dstr for now
EngineBaseClock::~EngineBaseClock() {}

const EngineClock_Microseconds& EngineBaseClock::QueryTime()
{
	m_currentQueryPoint = std::chrono::high_resolution_clock::now();
	m_durationTimeElapsed = std::chrono::duration_cast<ClockTimeUnit>(m_currentQueryPoint - m_lastQueryPoint);
	m_timeElapsed = m_durationTimeElapsed.count();
	m_lastQueryPoint = m_currentQueryPoint;

	return m_timeElapsed;
}

const EngineClock_Microseconds& EngineBaseClock::GetTimeElapsed()
{
	return m_timeElapsed;
}


CtrlEngineClock::CtrlEngineClock() :
	m_internalClock(),
	m_currentIndex(0),
	m_lastIndex(0), 
	m_PQTotal(0), 
	m_PQLowestIndex(0), 
	m_PQHighestIndex(0), 
	m_QPS(0.0),
	m_lowestQPS(0.0),
	m_averageQPS(0.0),
	m_highestQPS(0.0)
{
	m_pastQueries[m_currentIndex] = 0;
}

//empty for now
CtrlEngineClock::~CtrlEngineClock() {}

const EngineClock_Microseconds& CtrlEngineClock::QueryTime()
{
	//modify total time count
	m_PQTotal -= m_pastQueries[m_currentIndex];
	
	m_pastQueries[m_currentIndex] = m_internalClock.QueryTime();
	m_PQTotal += m_pastQueries[m_currentIndex];
	
	//TODO highest lowest qp counts

	//modify qps counts
	m_QPS = 1.0 / m_pastQueries[m_currentIndex];
	m_averageQPS = ENGINECLOCK_INTERNAL_TIMESLOTS / m_PQTotal;
	//TODO finish with highest lowest qps

	//increment current index
	m_lastIndex = m_currentIndex;
	m_currentIndex = (m_currentIndex + 1) % ENGINECLOCK_INTERNAL_TIMESLOTS;
	return m_pastQueries[m_lastIndex];
}

const EngineClock_Microseconds& CtrlEngineClock::GetTimeElapsed()
{
	return m_pastQueries[m_lastIndex];
}

const TimeRate& CtrlEngineClock::getQPS()
{
	return m_QPS;
}

const TimeRate& CtrlEngineClock::getAvgQPS()
{
	return m_averageQPS;
}

const TimeRate& CtrlEngineClock::getLowQPS()
{
	return m_lowestQPS;
}

const TimeRate& CtrlEngineClock::getHighQPS()
{
	return m_highestQPS;
}

const TimeSeconds CtrlEngineClock::GetSecondsElapsedModifiedForHitch()
{
	return 0.0; //TODO finish this
}

const TimeSeconds CtrlEngineClock::GetTrueSecondsElapsed()
{
	return SECONDS_PER_MICROSECOND * m_pastQueries[m_lastIndex];
}