#include "Logger.h"
#include "Profiler.h"

SunDialProfiler::SunDialProfiler(
	bool highPrecision, 
	const FilePath& file, 
	const LineNumber line, 
	const std::string& funcname, 
	const std::string& msg) : 
	m_highPrecisionMode(highPrecision), 
	m_file(file), 
	m_line(line), 
	m_funcname(funcname), 
	m_msg(msg), 
	m_timeElapsed(""), 
	m_start(std::chrono::steady_clock::now())
{}

#define SD_PROFILER_MSG_TEMPLATE "in function " + m_funcname + " profiler took " + m_timeElapsed + " to go out of scope"

SunDialProfiler::~SunDialProfiler()
{
	if(m_highPrecisionMode)
		m_timeElapsed = std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - m_start).count()) + "ns";
	else
		m_timeElapsed = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_start).count()) + "ms";

	if(m_msg == "")
		m_msg = SD_PROFILER_MSG_TEMPLATE;
	else
		m_msg = SD_PROFILER_MSG_TEMPLATE + ", " + m_msg;

	SafeLog_ImmediatePushMessage(LogType::LOGTYPE_PROFILE, m_file, m_line, m_msg);
}