#pragma once
#include "FilePathProcessing.h"

//uncomment to use full file path
#define LOG_USE_FILENAME 1

//uncomment to use a separate thread created specifically to log
//comment to use the thread requesting the log to actually log
#define LOG_USE_LOGGING_THREAD 1

//Every added type must have a corresponding LogMsgPrefix written in Logger.cpp
enum LogType
{
	LOGTYPE_NONE,
	LOGTYPE_FATAL,
	LOGTYPE_WARNING,
	LOGTYPE_SIZE			//Keep this as the last element!
};

//only queue the msgs, need to call SafeLog_PushAllPendingMessages for it to push
void SafeLog_QueueMessage(const LogType type, const FilePath& file, const LineNumber line, const std::string& log);
void SafeLog_ImmediatePushMessage(const LogType type, const FilePath& file, const LineNumber line, const std::string& log);
void SafeLog_PushAllPendingMessages();

//only queue the msgs, need to call SafeLog_PushAllPendingMessages for it to push
#define LOG_FATAL_QUEUE(type, log) SafeLog_QueueMessage(LogType::LOGTYPE_FATAL, __FILE__, __LINE__, log)
#define LOG_WARNING_QUEUE(type, log) SafeLog_QueueMessage(LogType::LOGTYPE_WARNING, __FILE__, __LINE__, log)
#define LOG_MSG_QUEUE(type, log) SafeLog_QueueMessage(LogType::LOGTYPE_NONE, __FILE__, __LINE__, log)
#define LOG_QUEUE(type, log) SafeLog_QueueMessage(type, __FILE__, __LINE__, log)

#define LOG_FATAL_PUSH(log) SafeLog_ImmediatePushMessage(LogType::LOGTYPE_FATAL, __FILE__, __LINE__, log)
#define LOG_WARNING_PUSH(log) SafeLog_ImmediatePushMessage(LogType::LOGTYPE_WARNING, __FILE__, __LINE__, log)
#define LOG_MSG_PUSH(log) SafeLog_ImmediatePushMessage(LogType::LOGTYPE_NONE, __FILE__, __LINE__, log)
#define LOG_PUSH(type, log) SafeLog_ImmediatePushMessage(type, __FILE__, __LINE__, log)