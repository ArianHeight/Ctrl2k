#include <format>

#include "Core/GreatBath/Logger.h"
#include "Profiler.h"

using namespace rvl;

// TODO maybe move this with the SunDialPrecision enum?
static const char* timeSuffixes[SunDialPrecision::SUNDIALPRECISION_INVALID_SIZE] =
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
static inline long long GetTimeElapsed(const std::chrono::steady_clock::time_point& start, const std::chrono::steady_clock::time_point& end, SunDialPrecision& precision)
{
	switch(precision)
	{
	case SUNDIALPRECISION_NANOSECONDS:
		return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
	case SUNDIALPRECISION_MICROSECONDS:
		return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	case SUNDIALPRECISION_MILLISECONDS:
		return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	case SUNDIALPRECISION_SECONDS:
	default:
		precision = SUNDIALPRECISION_SECONDS;
	}
	return std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
}

SunDialProfilerScoped::SunDialProfilerScoped(SunDialPrecision highPrecision, c_string file, const gbt::LineNumber line, c_string funcname, std::string msg)
	: m_precision(highPrecision), file(file), m_line(line), funcname(funcname), m_msg(std::move(msg)),
	m_start(std::chrono::steady_clock::now())
{}

SunDialProfilerScoped::~SunDialProfilerScoped()
{
	const long long timeElapsed = GetTimeElapsed(m_start, std::chrono::steady_clock::now(), m_precision);

	// profiler is allowed to call safelog functions directly, it needs a deeper interface with the logger
	gbt::SafeLog_ImmediatePushMessage(gbt::LOGLEVEL_PROFILE, file, m_line, std::chrono::system_clock::now(), std::format("{}() took {}{}{}{}", funcname, timeElapsed, timeSuffixes[m_precision], msgPrefix[m_msg.empty()], m_msg));
}

void SunDialProfiler::_TimerEnd(c_string file, gbt::LineNumber line)
{
	const long long timeElapsed = GetTimeElapsed(_start, std::chrono::steady_clock::now(), _precision);

	// profiler is allowed to call safelog functions directly, it needs a deeper interface with the logger
	gbt::SafeLog_ImmediatePushMessage(gbt::LOGLEVEL_PROFILE, file, line, std::chrono::system_clock::now(), std::format("Section {} took {}{}", _section_name, timeElapsed, timeSuffixes[_precision]));
}
