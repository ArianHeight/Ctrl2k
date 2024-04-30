#include <format>

#include "Core/GreatBath/Logger.h"
#include "Profiler.h"

using namespace rvl;

SunDialProfiler::SunDialProfiler(bool highPrecision, c_string file, const gbt::LineNumber line, c_string funcname, std::string msg)
	: m_highPrecisionMode(highPrecision), m_file(file), m_line(line), m_funcname(funcname), m_msg(std::move(msg)),
	m_start(std::chrono::steady_clock::now())
{}

SunDialProfiler::~SunDialProfiler()
{
	std::string suffix;
	long long timeElapsed;
	if(m_highPrecisionMode)
	{
		timeElapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - m_start).count();
		suffix = "ns";
	}
	else
	{
		timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_start).count();
		suffix = "ms";
	}

	// profiler is allowed to call safelog functions directly, it needs a deeper interface with the logger
	gbt::SafeLog_ImmediatePushMessage(gbt::LOGLEVEL_PROFILE, m_file, m_line, std::chrono::system_clock::now(), std::format("function {} took {}{}", m_funcname, timeElapsed, suffix));
}
