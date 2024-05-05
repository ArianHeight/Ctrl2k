#include "EngineClock.h"

#include <math.h>
#include <algorithm>
#include <iterator>

//empty cstr for now
EngineBaseClock::EngineBaseClock() : m_lastQueryPoint(std::chrono::high_resolution_clock::now()), m_timeElapsed(0) {}

//empty dstr for now
EngineBaseClock::~EngineBaseClock() {}

EngineClock_Duration EngineBaseClock::QueryTime()
{
	m_currentQueryPoint = std::chrono::high_resolution_clock::now();
	m_timeElapsed = m_currentQueryPoint - m_lastQueryPoint;
	m_lastQueryPoint = m_currentQueryPoint;

	return m_timeElapsed;
}


CtrlEngineClock::CtrlEngineClock() :
	m_internalClock(),
	m_currentIndex(0),
	m_lastIndex(0), 
	m_PQTotal(ENGINECLOCK_INTERNAL_TIMESLOTS), 
	m_PQLowestIndex(0), 
	m_PQHighestIndex(0), 
	m_QPS(1.0),
	m_lowestQPS(1.0),
	m_averageQPS(1.0),
	m_highestQPS(1.0)
{
	m_pastQueries.fill(EngineClock_Duration(1)); //prevent divide by 0
}

//empty for now
CtrlEngineClock::~CtrlEngineClock() {}

EngineClock_Duration CtrlEngineClock::QueryTime()
{
	//modify total time count
	m_PQTotal -= m_pastQueries[m_currentIndex];
	
	m_pastQueries[m_currentIndex] = m_internalClock.QueryTime();
	m_PQTotal += m_pastQueries[m_currentIndex];
	
	//lower and higher qp index update
	if(m_PQLowestIndex == m_currentIndex || m_PQHighestIndex == m_currentIndex)
	{
		auto minmaxIndices = std::minmax_element(m_pastQueries.begin(), m_pastQueries.end());
		m_PQLowestIndex = std::distance(m_pastQueries.begin(), minmaxIndices.first);
		m_PQHighestIndex = std::distance(m_pastQueries.begin(), minmaxIndices.second);
	}
	else
	{
		if(m_pastQueries[m_currentIndex] <= m_pastQueries[m_PQLowestIndex])
			m_PQLowestIndex = m_currentIndex;
		if(m_pastQueries[m_currentIndex] >= m_pastQueries[m_PQHighestIndex])
			m_PQHighestIndex = m_currentIndex;
	}

	//modify qps counts
	m_QPS = 1.0 / NANOSECONDS_TO_SECONDS(m_pastQueries[m_currentIndex].count());
	m_averageQPS = 1.0 / NANOSECONDS_TO_SECONDS(m_PQTotal.count() / ENGINECLOCK_INTERNAL_TIMESLOTS);
	m_highestQPS = 1.0 / NANOSECONDS_TO_SECONDS(m_pastQueries[m_PQLowestIndex].count()); //shortest time elapsed = highest qps
	m_lowestQPS = 1.0 / NANOSECONDS_TO_SECONDS(m_pastQueries[m_PQHighestIndex].count()); //longest time elapsed = lower qps

	//increment current index
	m_lastIndex = m_currentIndex;
	m_currentIndex = (m_currentIndex + 1) & ENGINECLOCK_INTERNAL_TIMESLOTS_MINUS_ONE;
	return m_pastQueries[m_lastIndex];
}

TimeSeconds CtrlEngineClock::GetSecondsElapsedModifiedForHitch() const
{
	if((m_averageQPS - m_QPS) / m_averageQPS > ENGINECLOCK_INTENAL_HITCH_VARIANCE) //hitch detected
		return 1.0 / m_averageQPS; //loss of accuracy doesn't matter if program is hitching
	return MICROSECONDS_TO_SECONDS(m_pastQueries[m_lastIndex].count());
}

TimeSeconds CtrlEngineClock::GetTrueSecondsElapsed() const
{
	return MICROSECONDS_TO_SECONDS(m_pastQueries[m_lastIndex].count());
}