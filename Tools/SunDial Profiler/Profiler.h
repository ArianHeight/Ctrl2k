#pragma once
#include <chrono>
#include "Core/GreatBath/FilePathProcessing.h"

class SunDialProfiler
{
private:
	std::chrono::steady_clock::time_point m_start;
	bool m_highPrecisionMode;
	c_string m_file;
	gbt::LineNumber m_line;
	std::string m_funcname;
	std::string m_msg;
	std::string m_timeElapsed;

public:
	SunDialProfiler(bool highPrecision, c_string file, const gbt::LineNumber line, const std::string& funcname, const std::string& msg = "");
	~SunDialProfiler();
};

#define TOOLS_PROFILE_LOG(msg) auto profiler = SunDialProfiler(false, __FILE__, __LINE__, __FUNCTION__, msg)
#define TOOLS_PROFILE() auto profiler = SunDialProfiler(false, __FILE__, __LINE__, __FUNCTION__)
//use when you need to give custome profiler names to have multiple profiles happening in the same scope
#define TOOLS_MULTIPROFILE_LOG(profiler_name, msg) auto profiler_name = SunDialProfiler(false, __FILE__, __LINE__, __FUNCTION__, msg)
//use when you need to give custome profiler names to have multiple profiles happening in the same scope
#define TOOLS_MULTIPROFILE(profiler_name) auto profiler_name = SunDialProfiler(false, __FILE__, __LINE__, __FUNCTION__)

//HP = high precision(eg nanosecond based log instead of millisecond)
#define TOOLS_HP_PROFILE_LOG(msg) auto profiler = SunDialProfiler(true, __FILE__, __LINE__, __FUNCTION__, msg)
//HP = high precision(eg nanosecond based log instead of millisecond)
#define TOOLS_HP_PROFILE() auto profiler = SunDialProfiler(true, __FILE__, __LINE__, __FUNCTION__)
//use when you need to give custome profiler names to have multiple profiles happening in the same scope
#define TOOLS_HP_MULTIPROFILE_LOG(profiler_name, msg) auto profiler_name = SunDialProfiler(true, __FILE__, __LINE__, __FUNCTION__, msg)
//use when you need to give custome profiler names to have multiple profiles happening in the same scope
#define TOOLS_HP_MULTIPROFILE(profiler_name) auto profiler_name = SunDialProfiler(true, __FILE__, __LINE__, __FUNCTION__)
