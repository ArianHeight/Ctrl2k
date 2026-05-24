#include <format>

#include "Core/GreatBath/Logger.h"
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
	: m_precision(highPrecision), file(file), m_line(line), funcname(funcname), m_msg(msg),
	m_start(std::chrono::steady_clock::now())
{}

ProfilerScoped::~ProfilerScoped()
{
	const long long timeElapsed = GetTimeElapsed(m_start, std::chrono::steady_clock::now(), m_precision);

	// profiler is allowed to call safelog functions directly, it needs a deeper interface with the logger
	gbt::SafeLog_ImmediatePushMessage(gbt::LOGLEVEL_PROFILE, file, m_line, std::chrono::system_clock::now(), std::format("{}() took {}{}{}{}", funcname, timeElapsed, timeSuffixes[(size_t)m_precision], msgPrefix[m_msg.empty() ? 1 : 0], m_msg));
}

void Profiler::TimerEnd(c_string file, gbt::LineNumber line)
{
	const long long timeElapsed = GetTimeElapsed(m_start, std::chrono::steady_clock::now(), m_precision);

	// profiler is allowed to call safelog functions directly, it needs a deeper interface with the logger
	gbt::SafeLog_ImmediatePushMessage(gbt::LOGLEVEL_PROFILE, file, line, std::chrono::system_clock::now(), std::format("Section {} took {}{}", m_section_name, timeElapsed, timeSuffixes[(size_t)m_precision]));
}
