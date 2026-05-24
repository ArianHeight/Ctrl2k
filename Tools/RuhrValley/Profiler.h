#pragma once
#include <chrono>
#include "Core/GreatBath/FilePathProcessing.h"
#include "Core/OracleBone/obn.h"

namespace rvl
{

// move this enum into SunDial perhaps?
enum class TimePrecision : uint8_t
{
	NANOSECONDS,
	MICROSECONDS,
	MILLISECONDS,
	SECONDS,
	INVALID_SIZE
};

// DONOT USE THIS CLASS DIRECTLY!
// use the macros instead
class ProfilerScoped : public uncopyable, public unmoveable
{
private:
	std::chrono::steady_clock::time_point m_start;
	TimePrecision m_precision;
	c_string file;
	gbt::LineNumber m_line;
	c_string funcname;
	obn::small_string128 m_msg;

public:
	ProfilerScoped(TimePrecision precision, c_string file, const gbt::LineNumber line, c_string funcname, obn::view_string msg = "");
	~ProfilerScoped();
};

#define PROFILE_LOG_SCOPED(msg) auto _profiler_scoped = rvl::ProfilerScoped(rvl::TimePrecision::MILLISECONDS, __FILE__, __LINE__, __FUNCTION__, msg)
#define PROFILE_LOG_SCOPED_PRECISION(precision, msg) auto _profiler_scoped = rvl::ProfilerScoped(precision, __FILE__, __LINE__, __FUNCTION__, msg)

#define PROFILE_SCOPED() auto _profiler_scoped = rvl::ProfilerScoped(rvl::TimePrecision::MILLISECONDS, __FILE__, __LINE__, __FUNCTION__)
#define PROFILE_SCOPED_PRECISION(precision) auto _profiler_scoped = rvl::ProfilerScoped(precision, __FILE__, __LINE__, __FUNCTION__)

struct Profiler : public uncopyable, public unmoveable
{
	TimePrecision m_precision;
	c_string m_section_name;
	std::chrono::steady_clock::time_point m_start;

	void TimerEnd(c_string file, gbt::LineNumber line);

	Profiler(TimePrecision precision, c_string section_name, std::chrono::steady_clock::time_point start) : m_precision(precision), m_section_name(section_name), m_start(start) {}
};

#define PROFILE_SECTION_START(name) auto _profiler_section_##name = rvl::Profiler(rvl::TimePrecision::MILLISECONDS, #name, std::chrono::steady_clock::now())
#define PROFILE_SECTION_START_PRECISION(name, precision) auto _profiler_section_##name = rvl::Profiler(precision, #name, std::chrono::steady_clock::now())

#define PROFILE_SECTION_END(name) _profiler_section_##name.TimerEnd(__FILE__, __LINE__)

}