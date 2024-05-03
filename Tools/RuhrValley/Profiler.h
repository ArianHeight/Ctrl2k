#pragma once
#include <chrono>
#include "Core/GreatBath/FilePathProcessing.h"

namespace rvl
{

// TODO move this enum into SunDial perhaps?
enum SunDialPrecision : uint8_t
{
	SUNDIALPRECISION_NANOSECONDS,
	SUNDIALPRECISION_MICROSECONDS,
	SUNDIALPRECISION_MILLISECONDS,
	SUNDIALPRECISION_SECONDS,
	SUNDIALPRECISION_INVALID_SIZE
};

// DONOT USE THIS CLASS DIRECTLY!
// use the macros instead
class SunDialProfilerScoped
{
private:
	std::chrono::steady_clock::time_point m_start;
	SunDialPrecision m_precision;
	c_string file;
	gbt::LineNumber m_line;
	c_string funcname;
	std::string m_msg;

public:
	SunDialProfilerScoped(SunDialPrecision precision, c_string file, const gbt::LineNumber line, c_string funcname, std::string msg = "");
	~SunDialProfilerScoped();

	SunDialProfilerScoped(const SunDialProfilerScoped& other) = delete;
	SunDialProfilerScoped& operator=(const SunDialProfilerScoped& other) = delete;
};

#define PROFILE_LOG_SCOPED(msg) auto _profiler_scoped = rvl::SunDialProfilerScoped(rvl::SunDialPrecision::SUNDIALPRECISION_MILLISECONDS, __FILE__, __LINE__, __FUNCTION__, msg)
#define PROFILE_LOG_SCOPED_PRECISION(precision, msg) auto _profiler_scoped = rvl::SunDialProfilerScoped(precision, __FILE__, __LINE__, __FUNCTION__, msg)

#define PROFILE_SCOPED() auto _profiler_scoped = rvl::SunDialProfilerScoped(rvl::SunDialPrecision::SUNDIALPRECISION_MILLISECONDS, __FILE__, __LINE__, __FUNCTION__)
#define PROFILE_SCOPED_PRECISION(precision) auto _profiler_scoped = rvl::SunDialProfilerScoped(precision, __FILE__, __LINE__, __FUNCTION__)

struct SunDialProfiler
{
	SunDialPrecision _precision;
	c_string _section_name;
	std::chrono::steady_clock::time_point _start;

	void _TimerEnd(c_string file, gbt::LineNumber line);

	SunDialProfiler(SunDialPrecision precision, c_string section_name, std::chrono::steady_clock::time_point start) : _precision(precision), _section_name(section_name), _start(start) {}
	SunDialProfiler(const SunDialProfiler& other) = delete;
	SunDialProfiler& operator=(const SunDialProfiler& other) = delete;
};

#define PROFILE_SECTION_START(name) auto _profiler_section_##name = rvl::SunDialProfiler(rvl::SunDialPrecision::SUNDIALPRECISION_MILLISECONDS, #name, std::chrono::steady_clock::now())
#define PROFILE_SECTION_START_PRECISION(name, precision) auto _profiler_section_##name = rvl::SunDialProfiler(precision, #name, std::chrono::steady_clock::now())

#define PROFILE_SECTION_END(name) _profiler_section_##name._TimerEnd(__FILE__, __LINE__)

}