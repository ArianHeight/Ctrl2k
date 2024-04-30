#pragma once
#include <chrono>
#include "Core/GreatBath/FilePathProcessing.h"

namespace rvl
{

// DONOT USE THIS CLASS DIRECTLY!
// use the macros instead
class SunDialProfiler
{
private:
	std::chrono::steady_clock::time_point m_start;
	bool m_highPrecisionMode;
	c_string m_file;
	gbt::LineNumber m_line;
	c_string m_funcname;
	std::string m_msg;

public:
	SunDialProfiler(bool highPrecision, c_string file, const gbt::LineNumber line, c_string funcname, std::string msg = "");
	~SunDialProfiler();
};

#define TOOLS_PROFILE_LOG(msg) auto _profiler = rvl::SunDialProfiler(false, __FILE__, __LINE__, __FUNCTION__, msg)
#define TOOLS_PROFILE() auto _profiler = rvl::SunDialProfiler(false, __FILE__, __LINE__, __FUNCTION__)
//use when you need to give custome profiler names to have multiple profiles happening in the same scope
#define TOOLS_MULTIPROFILE_LOG(profiler_name, msg) auto profiler_name = rvl::SunDialProfiler(false, __FILE__, __LINE__, __FUNCTION__, msg)
//use when you need to give custome profiler names to have multiple profiles happening in the same scope
#define TOOLS_MULTIPROFILE(profiler_name) auto profiler_name = rvl::SunDialProfiler(false, __FILE__, __LINE__, __FUNCTION__)

//HP = high precision(eg nanosecond based log instead of millisecond)
#define TOOLS_HP_PROFILE_LOG(msg) auto _profiler = rvl::SunDialProfiler(true, __FILE__, __LINE__, __FUNCTION__, msg)
//HP = high precision(eg nanosecond based log instead of millisecond)
#define TOOLS_HP_PROFILE() auto _profiler = rvl::SunDialProfiler(true, __FILE__, __LINE__, __FUNCTION__)
//use when you need to give custome profiler names to have multiple profiles happening in the same scope
#define TOOLS_HP_MULTIPROFILE_LOG(profiler_name, msg) auto profiler_name = rvl::SunDialProfiler(true, __FILE__, __LINE__, __FUNCTION__, msg)
//use when you need to give custome profiler names to have multiple profiles happening in the same scope
#define TOOLS_HP_MULTIPROFILE(profiler_name) auto profiler_name = rvl::SunDialProfiler(true, __FILE__, __LINE__, __FUNCTION__)


}