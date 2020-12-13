//In chage of logging all things
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <assert.h>

#include "Logger.h"

static const std::string LogMsgPrefix[LogType::SIZE]
{
	"", 
	"FATAL ERROR: ", 
	"WARNING: "
};

struct LogBlock
{
	LogType type;
	std::thread::id creationId;
	FilePath file;
	LineNumber line;
	std::string log;
};

static std::mutex logGuard;
static std::queue<LogBlock> pendingLogs;

//WARNING: not thread-safe
//YOU MUST USE THE LOG GUARD to use this function
static void Log_PushMessage(const LogBlock& data)
{
	static std::string file;
#ifdef LOG_USEFILENAME
	file = FilePath_GetFileName(data.file);
#else
	file = data.file;
#endif

	assert(data.type < LogType::SIZE && data.type >= 0);

	std::cout << LogMsgPrefix[data.type];
	switch(data.type)
	{
	case LogType::FATAL:
		std::cout << "In file " << file << " line " << data.line << " thread id " << data.creationId << ", ";
		break;
	default:
		std::cout << "In file " << file << " line " << data.line << ", ";
	}
	std::cout << data.log << '\n';
}

void SafeLog_QueueMessage(const LogType type, const FilePath& file, const LineNumber line, const std::string& log)
{
	std::lock_guard<std::mutex> lock(logGuard);

	pendingLogs.push(LogBlock{ type, std::this_thread::get_id(), file, line, log });
}

//WARNING: this is not mutex locked so not thread-safe
//YOU MUST USE THE LOG GUARD to use this function
static inline void Log_PushAllPendingMessages()
{
	while(!pendingLogs.empty())
	{
		Log_PushMessage(pendingLogs.back());
		pendingLogs.pop();
	}
}

void SafeLog_ImmediatePushMessage(const LogType type, const FilePath& file, const LineNumber line, const std::string& log)
{
	std::lock_guard<std::mutex> lock(logGuard);

	Log_PushAllPendingMessages();
	Log_PushMessage(LogBlock{ type, std::this_thread::get_id(), file, line, log });
}

void SafeLog_PushAllPendingMessages()
{
	std::lock_guard<std::mutex> lock(logGuard);
	
	Log_PushAllPendingMessages();
}
