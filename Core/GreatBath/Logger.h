#pragma once
#include "FilePathProcessing.h"

//uncomment to use full file path
#define LOG_USE_FILENAME

//uncomment to use a separate thread created specifically to log
//comment to use the thread requesting the log to actually log
#define LOG_USE_LOGGING_THREAD

namespace gbt
{

//Every added type must have a corresponding LogMsgPrefix written in Logger.cpp
enum LogType : uint8_t
{
	LOGTYPE_TRACE,
	LOGTYPE_PROFILE,
	LOGTYPE_MSG,
	LOGTYPE_WARNING,
	LOGTYPE_ERROR,
	LOGTYPE_FATAL,
	LOGTYPE_NONE_FLUSH,
	LOGTYPE_NONE_SIZE			//Keep this as the last element!
};

//only queue the msgs, need to call SafeLog_PushAllPendingMessages for it to push
void SafeLog_QueueMessage(const LogType type, const FilePath& file, const LineNumber line, const std::string& log);
void SafeLog_ImmediatePushMessage(const LogType type, const FilePath& file, const LineNumber line, const std::string& log);
void SafeLog_PushAllPendingMessages();

//only queue the msgs, need to call SafeLog_PushAllPendingMessages for it to push
#define LOG_TRACE_QUEUE(log) gbt::SafeLog_QueueMessage(gbt::LogType::LOGTYPE_TRACE, __FILE__, __LINE__, log)
#define LOG_MSG_QUEUE(log) gbt::SafeLog_QueueMessage(gbt::LogType::LOGTYPE_MSG, __FILE__, __LINE__, log)
#define LOG_WARNING_QUEUE(log) gbt::SafeLog_QueueMessage(gbt::LogType::LOGTYPE_WARNING, __FILE__, __LINE__, log)
#define LOG_ERROR_QUEUE(log) gbt::SafeLog_QueueMessage(gbt::LogType::LOGTYPE_ERROR, __FILE__, __LINE__, log)
#define LOG_FATAL_QUEUE(log) gbt::SafeLog_QueueMessage(gbt::LogType::LOGTYPE_FATAL, __FILE__, __LINE__, log)
#define LOG_FLUSH_QUEUE() gbt::SafeLog_QueueMessage(gbt::LogType::LOGTYPE_NONE_FLUSH, "", 0, "")
#define LOG_QUEUE(type, log) gbt::SafeLog_QueueMessage(type, __FILE__, __LINE__, log)

#define LOG_TRACE_PUSH(log) gbt::SafeLog_ImmediatePushMessage(gbt::LogType::LOGTYPE_TRACE, __FILE__, __LINE__, log)
#define LOG_MSG_PUSH(log) gbt::SafeLog_ImmediatePushMessage(gbt::LogType::LOGTYPE_MSG, __FILE__, __LINE__, log)
#define LOG_WARNING_PUSH(log) gbt::SafeLog_ImmediatePushMessage(gbt::LogType::LOGTYPE_WARNING, __FILE__, __LINE__, log)
#define LOG_ERROR_PUSH(log) gbt::SafeLog_ImmediatePushMessage(gbt::LogType::LOGTYPE_ERROR, __FILE__, __LINE__, log)
#define LOG_FATAL_PUSH(log) gbt::SafeLog_ImmediatePushMessage(gbt::LogType::LOGTYPE_FATAL, __FILE__, __LINE__, log)
#define LOG_FLUSH() gbt::SafeLog_ImmediatePushMessage(gbt::LogType::LOGTYPE_NONE_FLUSH, "", 0, "")
#define LOG_PUSH(type, log) gbt::SafeLog_ImmediatePushMessage(type, __FILE__, __LINE__, log)

}