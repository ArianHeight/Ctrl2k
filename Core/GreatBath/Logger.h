#pragma once
#include <chrono>
#include <format>

#include "Core/Monument/Monument.h"
#include "FilePathProcessing.h"

//uncomment to use a separate thread created specifically to log
//comment to use the thread requesting the log to actually log
#define LOG_USE_LOGGING_THREAD

#ifdef ERROR
compile_assert_msg(false, "This file has \"ERROR\" in a few enum declarations that is being overriden by the ERROR macro"
	"(probably from wingdi.h), please include this file before the ERROR macro is defined or undef the ERROR macro.");
#endif

namespace gbt
{

//Every added level must have a corresponding LogMsgPrefix written in Logger.cpp
ENUM_SCOPED_TYPED(LogLevel, uint8_t,
	PROFILE,
	TRACE,
	MSG,
	WARNING,
	ERROR,
	FATAL,
	NONE_FLUSH,
	NONE_SIZE //keep as the last element!
);
compile_assert((1 << LogLevel::NONE_SIZE) <= UINT8_MAX);

enum class LogLevelFlag : uint8_t
{
	NONE = 0,
	PROFILE = 1 << LogLevel::PROFILE,
	TRACE = 1 << LogLevel::TRACE,
	MSG = 1 << LogLevel::MSG,
	WARNING = 1 << LogLevel::WARNING,
	ERROR = 1 << LogLevel::ERROR,
	FATAL = 1 << LogLevel::FATAL,
	FLUSH = 1 << LogLevel::NONE_FLUSH, // this flag should do nothing

	ALL = UINT8_MAX
};
ENUM_CLASS_OP_GEN(LogLevelFlag);
compile_assert(sizeof(LogLevelFlag) == sizeof(uint8_t));

enum class LogPrefix : uint8_t
{
	SHORT,
	LONG,
	NONE,
	INVALID_SIZE //Keep this as the last element!
};

enum class LogTime : uint8_t
{
	NONE, // don't log time
	HMS, // HH:mm:ss
	HMS_S, // HH:mm:ss.ssss
	FULL, // YYYY-MM-DD HH:mm:ss.ssss
	INVALID_SIZE //Keep this as the last element!
};

struct LoggingStreamSettings
{
	LogLevelFlag levelFlags = LogLevelFlag::ALL; // flags for setting which type of msgs get logged
	LogPrefix usePrefix = LogPrefix::LONG; // which prefix to add to msgs
	LogTime useLogTime = LogTime::FULL; // how should time be logged
	bool showTextColour : 1 = true; // use ANSI escape characters to do coloured text in consoles
	bool showThreadId : 1 = true; // print the thread id that logged the msg
	bool showFile : 1 = true; // print the file name that logged the msg
	bool logFullPath : 1 = true; // only takes effect if showing File, shows the full absolute file path
	bool showLineNumber : 1 = true; // only takes effect if showing File, shows the line number of the log call

	//Everything is on by default
	LoggingStreamSettings() = default;
};

enum class LogVerbosity : uint8_t
{
	LOW,
	MEDIUM,
	HIGH,
	FULL,
	INVALID_COUNT
};

bool SafeLog_RegisterFile(const LoggingStreamSettings& settings, const FilePath& path, bool truncate);
bool SafeLog_RegisterFile(LogLevel lvl, LogVerbosity verbosity, const FilePath& path);
bool SafeLog_RegisterFile(LogLevel lvl, const FilePath& path);
bool SafeLog_RegisterFile(const FilePath& path);
bool SafeLog_DeregisterFile(const FilePath& path);
//Please ensure the ostream does not get deleted for the entire duration of the program
bool SafeLog_RegisterOutputStream(LogLevel lvl, LogVerbosity verbosity, std::ostream& os);
//Please ensure the ostream does not get deleted for the entire duration of the program
bool SafeLog_RegisterOutputStream(LogLevel lvl, std::ostream& os);
//Please ensure the ostream does not get deleted for the entire duration of the program
bool SafeLog_RegisterOutputStream(const LoggingStreamSettings& settings, std::ostream& os);
bool SafeLog_DeregisterOutputStream(const std::ostream& os);

/*

You must register a logging output stream before being able to see logs

*/
#define REGISTER_FILE_FOR_LOGGING_CUSTOM_NO_TRUNCATE(settings, path) gbt::SafeLog_RegisterFile(settings, path, false)
#define REGISTER_FILE_FOR_LOGGING_CUSTOM(settings, path) gbt::SafeLog_RegisterFile(settings, path, true)
#define REGISTER_FILE_FOR_LOGGING_VERBOSITY(lvl, verbosity, path) gbt::SafeLog_RegisterFile(lvl, verbosity, path)
#define REGISTER_FILE_FOR_LOGGING_LEVEL(lvl, path) gbt::SafeLog_RegisterFile(lvl, path)
#define REGISTER_FILE_FOR_LOGGING(path) gbt::SafeLog_RegisterFile(path)
#define REGISTER_LOGGING_STREAM_VERBOSITY(lvl, verbosity, os) gbt::SafeLog_RegisterOutputStream(lvl, verbosity, os)
#define REGISTER_LOGGING_STREAM(setting, os) gbt::SafeLog_RegisterOutputStream(setting, os)
#define DEREGISTER_FILE_FOR_LOGGING(path) gbt::SafeLog_DeregisterFile(path)
#define DEREGISTER_LOGGING_STREAM(os) gbt::SafeLog_DeregisterOutputStream(os)

//DONOT call this directly use the logging macros!
//only queue the msgs, need to call SafeLog_PushAllPendingMessages for it to push
void SafeLog_QueueMessage(const LogLevel level, c_string file, const LineNumber line, const std::chrono::system_clock::time_point time, std::string&& log);
//DONOT call this directly use the logging macros!
void SafeLog_ImmediatePushMessage(const LogLevel level, c_string file, const LineNumber line, const std::chrono::system_clock::time_point time, std::string&& log);
void SafeLog_PushAllPendingMessages();

//only queue the msgs, need to call SafeLog_PushAllPendingMessages for it to push
#define LOG_QUEUE(lvl, log, ...) gbt::SafeLog_QueueMessage(lvl, __FILE__, __LINE__, std::chrono::system_clock::now(), std::vformat(log, std::make_format_args(__VA_ARGS__)))
#define LOG_TRACE_QUEUE(log, ...) LOG_QUEUE(gbt::LogLevel::TRACE, log, __VA_ARGS__)
#define LOG_MSG_QUEUE(log, ...) LOG_QUEUE(gbt::LogLevel::MSG, log, __VA_ARGS__)
#define LOG_WARNING_QUEUE(log, ...) LOG_QUEUE(gbt::LogLevel::WARNING, log, __VA_ARGS__)
#define LOG_ERROR_QUEUE(log, ...) LOG_QUEUE(gbt::LogLevel::ERROR, log, __VA_ARGS__)
#define LOG_FATAL_QUEUE(log, ...) LOG_QUEUE(gbt::LogLevel::FATAL, log, __VA_ARGS__)
#define LOG_FLUSH_QUEUE() gbt::SafeLog_QueueMessage(gbt::LogLevel::NONE_FLUSH, "", 0, std::chrono::system_clock::now(), "")

#define LOG_PUSH(lvl, log, ...) gbt::SafeLog_ImmediatePushMessage(lvl, __FILE__, __LINE__, std::chrono::system_clock::now(), std::vformat(log, std::make_format_args(__VA_ARGS__)))
#define LOG_TRACE_PUSH(log, ...) LOG_PUSH(gbt::LogLevel::TRACE, log, __VA_ARGS__)
#define LOG_MSG_PUSH(log, ...) LOG_PUSH(gbt::LogLevel::MSG, log, __VA_ARGS__)
#define LOG_WARNING_PUSH(log, ...) LOG_PUSH(gbt::LogLevel::WARNING, log, __VA_ARGS__)
#define LOG_ERROR_PUSH(log, ...) LOG_PUSH(gbt::LogLevel::ERROR, log, __VA_ARGS__)
#define LOG_FATAL_PUSH(log, ...) LOG_PUSH(gbt::LogLevel::FATAL, log, __VA_ARGS__)
#define LOG_FLUSH() gbt::SafeLog_ImmediatePushMessage(gbt::LogLevel::NONE_FLUSH, "", 0, std::chrono::system_clock::now(), "")

}