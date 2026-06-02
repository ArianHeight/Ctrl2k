#pragma once
#include <chrono>
#include "Core/GreatBath/FilePathProcessing.h"
#include "Core/OracleBone/obn.h"
#include "Core/AncestralHall/ahl.h"

/*

TODO everything in this file and the cpp should be disabled on full release mode - we don't want profiling code left over
in the final game.

*/

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
	c_string m_file;
	gbt::LineNumber m_line;
	c_string m_funcname;
	obn::small_string128 m_msg;

public:
	ProfilerScoped() = delete;
	ProfilerScoped(TimePrecision precision, c_string file, const gbt::LineNumber line, c_string funcname, obn::view_string msg = "");
	~ProfilerScoped();
};

#define PROFILE_LOG_SCOPED_PRECISION(msg, precision) auto _profiler_scoped = rvl::ProfilerScoped(precision, __FILE__, __LINE__, __FUNCTION__, msg)
#define PROFILE_LOG_SCOPED(msg) PROFILE_LOG_SCOPED_PRECISION(msg, rvl::TimePrecision::MILLISECONDS)

#define PROFILE_SCOPED_PRECISION(precision) auto _profiler_scoped = rvl::ProfilerScoped(precision, __FILE__, __LINE__, __FUNCTION__)
#define PROFILE_SCOPED() PROFILE_SCOPED_PRECISION(rvl::TimePrecision::MILLISECONDS)

// DONOT USE THIS CLASS DIRECTLY!
// use the macros instead
class Profiler : public uncopyable, public unmoveable
{
private:
	TimePrecision m_precision;
	c_string m_section_name;
	std::chrono::steady_clock::time_point m_start;

public:
	void TimerEnd(c_string file, gbt::LineNumber line);

	Profiler() = delete;
	Profiler(TimePrecision precision, c_string section_name, std::chrono::steady_clock::time_point start) : m_precision(precision), m_section_name(section_name), m_start(start) {}
};

#define PROFILE_SECTION_START_PRECISION(name, precision) auto _profiler_section_##name = rvl::Profiler(precision, #name, std::chrono::steady_clock::now())
#define PROFILE_SECTION_START(name) PROFILE_SECTION_START_PRECISION(name, rvl::TimePrecision::MILLISECONDS)

#define PROFILE_SECTION_END(name) _profiler_section_##name.TimerEnd(__FILE__, __LINE__)

constexpr size_t BENCHMARKER_MAX_STORAGE = 64;

// DONOT USE THIS CLASS DIRECTLY!
// use the macros instead
// TODO maybe this needs to be thread-safe?
class BenchMarker : public uncopyable, public unmoveable
{
public:
	class ScopedTimer
	{
	private:
		BenchMarker& m_parent;
		std::chrono::steady_clock::time_point m_start;

	public:
		ScopedTimer() = delete;
		ScopedTimer(BenchMarker& parent);
		~ScopedTimer();
	};

private:
	TimePrecision m_precision;
	bool m_overflowed = false;
	ahl::stack_vector<long long, BENCHMARKER_MAX_STORAGE> m_runTimes;
	size_t m_roundRobinIndex = 0;
	c_string m_file;
	gbt::LineNumber m_line;
	c_string m_funcname;

	void AddTime(const long long time);

public:
	ScopedTimer GenerateTimer();
	void LogResults();

	BenchMarker() = delete;
	BenchMarker(TimePrecision precision, c_string file, const gbt::LineNumber line, c_string funcname);
};

// DONOT USE THIS CLASS DIRECTLY!
// use the macros instead
void LogAllBenchMarkResults();

#define BENCHMARK_SCOPED_PRECISION(precision) \
static rvl::BenchMarker _benchMarker(precision, __FILE__, __LINE__, __FUNCTION__);\
rvl::BenchMarker::ScopedTimer _benchMarkTimer = _benchMarker.GenerateTimer()

#define BENCHMARK_SCOPED() BENCHMARK_SCOPED_PRECISION(rvl::TimePrecision::MILLISECONDS)

// Log all the results from all registered benchmarks
#define BENCHMARK_LOG_RESULTS() rvl::LogAllBenchMarkResults()

}