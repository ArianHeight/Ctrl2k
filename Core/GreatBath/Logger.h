#pragma once
#include <format>

#include "FilePathProcessing.h"

//uncomment to use a separate thread created specifically to log
//comment to use the thread requesting the log to actually log
#define LOG_USE_LOGGING_THREAD

namespace gbt
{

//Every added type must have a corresponding LogMsgPrefix written in Logger.cpp
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

enum LogPrefix : uint8_t
{
	LOGPREFIX_SHORT,
	LOGPREFIX_LONG,
	LOGPREFIX_SIZE			//Keep this as the last element!
};

//only queue the msgs, need to call SafeLog_PushAllPendingMessages for it to push
void SafeLog_QueueMessage(const LogLevel type, c_string file, const LineNumber line, std::string&& log);
void SafeLog_ImmediatePushMessage(const LogLevel type, c_string file, const LineNumber line, std::string&& log);
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