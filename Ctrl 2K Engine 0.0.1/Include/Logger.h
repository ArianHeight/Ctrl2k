#pragma once
#include "FilePathProcessing.h"

//uncomment to use full file path
#define LOG_USEFILENAME 1

//Every added type must have a corresponding LogMsgPrefix written in Logger.cpp
enum LogType
{
	NONE,
	FATAL,
	WARNING,
	SIZE			//Keep this as the last element!
};

//only queue the msgs, need to call SafeLog_PushAllPendingMessages for it to push
void SafeLog_QueueMessage(const LogType type, const FilePath& file, const LineNumber line, const std::string& log);
void SafeLog_ImmediatePushMessage(const LogType type, const FilePath& file, const LineNumber line, const std::string& log);
void SafeLog_PushAllPendingMessages();

//only queue the msgs, need to call SafeLog_PushAllPendingMessages for it to push
#define LOG_FATAL_QUEUE(type, log) SafeLog_QueueMessage(LogType::FATAL, __FILE__, __LINE__, log)
#define LOG_WARNING_QUEUE(type, log) SafeLog_QueueMessage(LogType::WARNING, __FILE__, __LINE__, log)
#define LOG_MSG_QUEUE(type, log) SafeLog_QueueMessage(LogType::NONE, __FILE__, __LINE__, log)
#define LOG_QUEUE(type, log) SafeLog_QueueMessage(type, __FILE__, __LINE__, log)

#define LOG_FATAL_PUSH(log) SafeLog_ImmediatePushMessage(LogType::FATAL, __FILE__, __LINE__, log)
#define LOG_WARNING_PUSH(log) SafeLog_ImmediatePushMessage(LogType::WARNING, __FILE__, __LINE__, log)
#define LOG_MSG_PUSH(log) SafeLog_ImmediatePushMessage(LogType::NONE, __FILE__, __LINE__, log)
#define LOG_PUSH(type, log) SafeLog_ImmediatePushMessage(type, __FILE__, __LINE__, log)