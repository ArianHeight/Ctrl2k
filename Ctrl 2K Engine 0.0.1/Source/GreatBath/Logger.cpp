//In chage of logging all things
#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <assert.h>

#include "Logger.h"

static const std::string LogMsgPrefix[LogType::LOGTYPE_SIZE]
{
	"", 
	"", 
	"FATAL ERROR: ", 
	"WARNING: ", 
	""
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
	assert(data.type < LogType::LOGTYPE_SIZE && data.type >= 0);

	std::cout << LogMsgPrefix[data.type];
	switch(data.type)
	{
	case LogType::LOGTYPE_NONE:
		break;
	case LogType::LOGTYPE_FLUSH:
		std::cout << std::endl;
		return; //early return here to avoid double spacing
	case LogType::LOGTYPE_FATAL:
	case LogType::LOGTYPE_WARNING:
		std::cout << "In file " << data.file << " line " << data.line << " thread id " << data.creationId << ", ";
		break;
	default:
		std::cout << "In file " << data.file << " line " << data.line << ", ";
	}
	std::cout << data.log << '\n';
}

//WARNING: this is not mutex locked so not thread-safe
//YOU MUST USE THE LOG GUARD to use this function
static inline void Log_PushAllPendingMessages()
{
	while(!pendingLogs.empty())
	{
		Log_PushMessage(pendingLogs.front());
		pendingLogs.pop();
	}
}

#ifdef LOG_USE_LOGGING_THREAD 1
//both of these are with logGuard
static std::condition_variable sleepLogger;
static volatile bool terminateLogger = false;

static void Log_ContinuousLogMsg()
{
	std::unique_lock<std::mutex> logLock(logGuard, std::defer_lock);

	logLock.lock();
	while(!terminateLogger)
	{
		if(!pendingLogs.empty()) //there are msgs in queue
		{
			Log_PushMessage(pendingLogs.front()); //log msg
			pendingLogs.pop();
			logLock.unlock(); //unlock guard to allow more msgs to come in
			std::this_thread::yield(); //yield to other threads
			logLock.lock(); //hold lock again before continuing
		}
		else
			sleepLogger.wait(logLock); //queue is empty, sleep until needed
	}

	//push any unpushed msgs
	Log_PushAllPendingMessages();

	logLock.unlock(); //unlock before exit
}

//class is used for scoping the thread
static class LoggerThread
{
private:
	std::thread logger; //this is effectively static

public:
	//thread creation
	LoggerThread() : logger(std::thread(Log_ContinuousLogMsg)) {}

	//safe thread termination
	~LoggerThread()
	{
		{
			std::lock_guard<std::mutex> lock(logGuard);

			terminateLogger = true;
			sleepLogger.notify_all();
		}

		logger.join();
	}
};

static LoggerThread loggerThread;

#endif

void SafeLog_QueueMessage(const LogType type, const FilePath& file, const LineNumber line, const std::string& log)
{
	std::lock_guard<std::mutex> lock(logGuard);
#ifdef LOG_USE_FILENAME
	pendingLogs.push(LogBlock{ type, std::this_thread::get_id(), FilePath_GetFileName(file), line, log });
#else
	pendingLogs.push(LogBlock{ type, std::this_thread::get_id(), file, line, log });
#endif
}

void SafeLog_ImmediatePushMessage(const LogType type, const FilePath& file, const LineNumber line, const std::string& log)
{
	SafeLog_QueueMessage(type, file, line, log);
#ifdef LOG_USE_LOGGING_THREAD
	sleepLogger.notify_all();
#else
	SafeLog_PushAllPendingMessages();
#endif
}

void SafeLog_PushAllPendingMessages()
{
	std::lock_guard<std::mutex> lock(logGuard);
	
	Log_PushAllPendingMessages();
}
