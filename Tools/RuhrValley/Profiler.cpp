#include <format>
#include <vector>

#include "Core/GreatBath/Logger.h"
#include "Core/Petra/rqm.h"
#include "Profiler.h"

using namespace rvl;

// TODO maybe move this with the TimePrecision enum?
static const char* timeSuffixes[(size_t)TimePrecision::INVALID_SIZE] =
{
	"ns",
	"us",
	"ms",
	"s"
};

static const char* msgPrefix[NUM_BOOLEAN_STATES] =
{
	", ",
	""
};

// will scope check precision
static inline long long GetTimeElapsed(const std::chrono::steady_clock::time_point& start, const std::chrono::steady_clock::time_point& end, TimePrecision& precision)
{
	switch(precision)
	{
	case TimePrecision::NANOSECONDS:
		return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
	case TimePrecision::MICROSECONDS:
		return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	case TimePrecision::MILLISECONDS:
		return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	case TimePrecision::SECONDS:
	default:
		precision = TimePrecision::SECONDS;
	}
	return std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
}

ProfilerScoped::ProfilerScoped(TimePrecision highPrecision, c_string file, const gbt::LineNumber line, c_string funcname, obn::view_string msg)
	: m_precision(highPrecision), m_file(file), m_line(line), m_funcname(funcname), m_msg(msg),
	m_start(std::chrono::steady_clock::now())
{}

ProfilerScoped::~ProfilerScoped()
{
	const long long timeElapsed = GetTimeElapsed(m_start, std::chrono::steady_clock::now(), m_precision);

	// profiler is allowed to call safelog functions directly, it needs a deeper interface with the logger
	gbt::SafeLog_ImmediatePushMessage(gbt::LOGLEVEL_PROFILE, m_file, m_line, std::chrono::system_clock::now(),
		std::format("{}() took {}{}{}{}", m_funcname, timeElapsed, timeSuffixes[(size_t)m_precision], msgPrefix[m_msg.empty() ? 1 : 0], m_msg));
}

void Profiler::TimerEnd(c_string file, gbt::LineNumber line)
{
	const long long timeElapsed = GetTimeElapsed(m_start, std::chrono::steady_clock::now(), m_precision);

	// profiler is allowed to call safelog functions directly, it needs a deeper interface with the logger
	gbt::SafeLog_ImmediatePushMessage(gbt::LOGLEVEL_PROFILE, file, line, std::chrono::system_clock::now(),
		std::format("Section {} took {}{}", m_section_name, timeElapsed, timeSuffixes[(size_t)m_precision]));
}

BenchMarker::ScopedTimer::ScopedTimer(BenchMarker& parent) : m_parent(parent), m_start(std::chrono::steady_clock::now())
{
}

BenchMarker::ScopedTimer::~ScopedTimer()
{
	m_parent.AddTime(GetTimeElapsed(m_start, std::chrono::steady_clock::now(), m_parent.m_precision));
}

void BenchMarker::AddTime(const long long time)
{
	if(m_runTimes.size() == m_runTimes.capacity())
	{
		m_overflowed = true;
		m_runTimes[m_roundRobinIndex] = time;

		++m_roundRobinIndex;
		if(m_roundRobinIndex >= m_runTimes.size())
		{
			m_roundRobinIndex = 0;
		}
	}
	else
	{
		m_runTimes.push_back(time);
	}
}

BenchMarker::ScopedTimer BenchMarker::GenerateTimer()
{
	return BenchMarker::ScopedTimer(*this);
}

void BenchMarker::LogResults()
{
	if(m_runTimes.empty())
		return;

	long long total = 0;
	long long best = m_runTimes[0];
	long long worst = m_runTimes[0];
	for(size_t i = 0; i < m_runTimes.size(); i++)
	{
		total += m_runTimes[i];
		if(m_runTimes[i] > worst)
		{
			worst = m_runTimes[i];
		}
		if(m_runTimes[i] < best)
		{
			best = m_runTimes[i];
		}
	}

	long double mean = total / m_runTimes.size();

	long double deviation = 0;
	for(size_t i = 0; i < m_runTimes.size(); i++)
	{
		deviation += (m_runTimes[i] - mean) * (m_runTimes[i] - mean);
	}
	long double standard_deviation = rqm::sqrt(deviation / m_runTimes.size());

	// TODO do not use this sort, use a faster one
	selection_sort(m_runTimes.data(), m_runTimes.size());
	long long median = m_runTimes[m_runTimes.size() / 2];

	const char* timeSuffix = timeSuffixes[(size_t)m_precision];

	// profiler is allowed to call safelog functions directly, it needs a deeper interface with the logger
	gbt::SafeLog_QueueMessage(gbt::LOGLEVEL_PROFILE, m_file, m_line, std::chrono::system_clock::now(),
		std::format("{}() BenchMark({}{} runs) | Total: {}{}, Best: {}{}, Worst: {}{}, Mean: {:.3g}{}, Median: {}{}, StDev: {:.3g}",
			m_funcname, m_overflowed ? "overflowed " : "", m_runTimes.size(),
			total, timeSuffix, best, timeSuffix, worst, timeSuffix, mean, timeSuffix, median, timeSuffix, standard_deviation));

	m_runTimes.clear();
	m_overflowed = false;
	m_roundRobinIndex = 0;
}

// TODO use a fixed sized vector or custom vector
// TODO make thread-safe
static std::vector<BenchMarker*> s_benchMarkers;

BenchMarker::BenchMarker(TimePrecision precision, c_string file, const gbt::LineNumber line, c_string funcname)
	: m_precision(precision), m_file(file), m_line(line), m_funcname(funcname)
{
	s_benchMarkers.push_back(this);
}

void rvl::LogAllBenchMarkResults()
{
	for(size_t i = 0; i < s_benchMarkers.size(); i++)
	{
		s_benchMarkers[i]->LogResults();
	}
}
