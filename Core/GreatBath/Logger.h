#pragma once
#include <format>

#include "Core/Monument/Monument.h"
#include "FilePathProcessing.h"

//uncomment to use a separate thread created specifically to log
//comment to use the thread requesting the log to actually log
#define LOG_USE_LOGGING_THREAD

namespace gbt
{

//Every added level must have a corresponding LogMsgPrefix written in Logger.cpp
enum LogLevel : uint8_t
{
	LOGLEVEL_TRACE,
	LOGLEVEL_PROFILE,
	LOGLEVEL_MSG,
	LOGLEVEL_WARNING,
	LOGLEVEL_ERROR,
	LOGLEVEL_FATAL,
	LOGLEVEL_NONE_FLUSH,
	LOGLEVEL_NONE_SIZE			//Keep this as the last element!
};
compile_assert(1 << LOGLEVEL_NONE_SIZE <= UINT8_MAX);

enum LogLevelFlag : uint8_t
{
	LOGLEVELFLAG_TRACE = 1 << LOGLEVEL_TRACE,
	LOGLEVELFLAG_PROFILE = 1 << LOGLEVEL_PROFILE,
	LOGLEVELFLAG_MSG = 1 << LOGLEVEL_MSG,
	LOGLEVELFLAG_WARNING = 1 << LOGLEVEL_WARNING,
	LOGLEVELFLAG_ERROR = 1 << LOGLEVEL_ERROR,
	LOGLEVELFLAG_FATAL = 1 << LOGLEVEL_FATAL,
	LOGLEVELFLAG_NONE_FLUSH = 1 << LOGLEVEL_NONE_FLUSH // this flag should do nothing
};

enum LogPrefix : uint8_t
{
	LOGPREFIX_SHORT,
	LOGPREFIX_LONG,
	LOGPREFIX_NONE,
	LOGPREFIX_INVALID_SIZE //Keep this as the last element!
};

struct LoggingStreamSettings
{
	LogLevelFlag levelFlags; // flags for setting which type of msgs get logged
	LogPrefix usePrefix; // which prefix to add to msgs
	bool showTextColour : 1; // use ANSI escape characters to do coloured text in consoles
	bool showThreadId : 1; // print the thread id that logged the msg
	bool showFile : 1; // print the file name that logged the msg
	bool logFullPath : 1; // only takes effect if showing File, shows the full absolute file path
	bool showLineNumber : 1; // only takes effect if showing File, shows the line number of the log call

	//Everything is on by default
	LoggingStreamSettings() { mem_set_one(this, sizeof(LoggingStreamSettings)); usePrefix = LOGPREFIX_LONG; }
};

enum LogVerbosity
{
	LOGVERBOSITY_LOW,
	LOGVERBOSITY_MEDIUM,
	LOGVERBOSITY_HIGH,
	LOGVERBOSITY_FULL,
	LOGVERBOSITY_INVALID_COUNT
};

bool SafeLog_RegisterFile(const LoggingStreamSettings& settings, FilePath path, bool truncate);
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
void SafeLog_QueueMessage(const LogLevel level, c_string file, const LineNumber line, std::string&& log);
//DONOT call this directly use the logging macros!
void SafeLog_ImmediatePushMessage(const LogLevel level, c_string file, const LineNumber line, std::string&& log);
void SafeLog_PushAllPendingMessages();

//only queue the msgs, need to call SafeLog_PushAllPendingMessages for it to push
#define LOG_QUEUE(lvl, log, ...) gbt::SafeLog_QueueMessage(lvl, __FILE__, __LINE__, std::vformat(log, std::make_format_args(__VA_ARGS__)))
#define LOG_TRACE_QUEUE(log, ...) LOG_QUEUE(gbt::LogLevel::LOGLEVEL_TRACE, log, __VA_ARGS__)
#define LOG_MSG_QUEUE(log, ...) LOG_QUEUE(gbt::LogLevel::LOGLEVEL_MSG, log, __VA_ARGS__)
#define LOG_WARNING_QUEUE(log, ...) LOG_QUEUE(gbt::LogLevel::LOGLEVEL_WARNING, log, __VA_ARGS__)
#define LOG_ERROR_QUEUE(log, ...) LOG_QUEUE(gbt::LogLevel::LOGLEVEL_ERROR, log, __VA_ARGS__)
#define LOG_FATAL_QUEUE(log, ...) LOG_QUEUE(gbt::LogLevel::LOGLEVEL_FATAL, log, __VA_ARGS__)
#define LOG_FLUSH_QUEUE() gbt::SafeLog_QueueMessage(gbt::LogLevel::LOGLEVEL_NONE_FLUSH, "", 0, "")

#define LOG_PUSH(lvl, log, ...) gbt::SafeLog_ImmediatePushMessage(lvl, __FILE__, __LINE__, log, std::vformat(log, std::make_format_args(__VA_ARGS__)))
#define LOG_TRACE_PUSH(log, ...) LOG_PUSH(gbt::LogLevel::LOGLEVEL_TRACE, log, __VA_ARGS__)
#define LOG_MSG_PUSH(log, ...) LOG_PUSH(gbt::LogLevel::LOGLEVEL_MSG, log, __VA_ARGS__)
#define LOG_WARNING_PUSH(log, ...) LOG_PUSH(gbt::LogLevel::LOGLEVEL_WARNING, log, __VA_ARGS__)
#define LOG_ERROR_PUSH(log, ...) LOG_PUSH(gbt::LogLevel::LOGLEVEL_ERROR, log, __VA_ARGS__)
#define LOG_FATAL_PUSH(log, ...) LOG_PUSH(gbt::LogLevel::LOGLEVEL_FATAL, log, __VA_ARGS__)
#define LOG_FLUSH() gbt::SafeLog_ImmediatePushMessage(gbt::LogLevel::LOGLEVEL_NONE_FLUSH, "", 0, "")

}