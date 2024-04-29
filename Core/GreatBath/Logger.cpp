//In chage of logging all things
#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <assert.h>

#include "Logger.h"

using namespace gbt;

enum LogPrefix : uint8_t
{
	LOGPREFIX_SHORT,
	LOGPREFIX_LONG,
	LOGPREFIX_INVALID_SIZE //Keep this as the last element!
};

static const char* LogMsgPrefix[LogPrefix::LOGPREFIX_INVALID_SIZE][LogLevel::LOGLEVEL_NONE_SIZE] =
{
{
	"[TRC]",
	"[PRF]",
	"[MSG]",
	"[WRN]",
	"[ERR]",
	"[FTL]",
	""
},
{
	"[TRACE]",
	"[PROFILE]",
	"",
	"[WARNING]",
	"[ERROR]",
	"[FATAL ERROR]",
	""
}
};

enum LogTextColourUsage : uint8_t
{
	LOGTEXTCOLOUR_DISABLED,
	LOGTEXTCOLOUR_ENABLED,
	LOGTEXTCOLOUR_INVALID_SIZE //Keep this as the last element!
};

static const char* LogTextColourPrefix[LogTextColourUsage::LOGTEXTCOLOUR_INVALID_SIZE][LogLevel::LOGLEVEL_NONE_SIZE] =
{
{
	"",
	"",
	"",
	"",
	"",
	"",
	""
},
{
	"\033[38;5;245m",
	"\033[38;5;250m",
	"\033[37m",
	"\033[33m",
	"\033[31m",
	"\033[35m",
	""
}
};

static const char* LogTextColourReset = "\033[0m";

struct LoggingStream
{
	LoggingStreamSettings settings;
	std::ostream* os;
};

struct LogBlock
{
	LogLevel level;
	std::thread::id creationId;
	FilePath file;
	LineNumber line;
	std::string log;
};

static std::mutex logGuard;
static std::queue<LogBlock> pendingLogs; // TODO maybe fix size this some day?
static std::vector<LoggingStream> outputs;

bool gbt::SafeLog_RegisterOutputStream(LogLevel lvl, LogVerbosity verbosity, std::ostream& os)
{
	LoggingStreamSettings settings;
	settings.levelFlags = (LogLevelFlag)(UINT8_MAX << lvl);
	switch(verbosity)
	{
	case LOGVERBOSITY_LOW:
		settings.showFile = false;
		settings.showThreadId = false;
	case LOGVERBOSITY_MEDIUM:
		settings.showLineNumber = false;
	case LOGVERBOSITY_HIGH:
		settings.logFullPath = false;
	}

	std::lock_guard<std::mutex> lock(logGuard);
	outputs.push_back({ settings, &os });
	return true;
}

bool gbt::SafeLog_RegisterOutputStream(LogLevel lvl, std::ostream& os)
{
	return SafeLog_RegisterOutputStream(lvl, LOGVERBOSITY_HIGH, os);
}

bool gbt::SafeLog_RegisterOutputStream(const LoggingStreamSettings& settings, std::ostream& os)
{
	std::lock_guard<std::mutex> lock(logGuard);
	outputs.push_back({ settings, &os });
	return true;
}

bool gbt::SafeLog_DeregisterOutputStream(const std::ostream& os)
{
	const std::ostream* const osptr = &os;
	std::lock_guard<std::mutex> lock(logGuard);
	auto begin = outputs.begin();
	auto end = outputs.end();
	auto it = std::find_if(begin, end, [&osptr](const LoggingStream& ls) { return ls.os == osptr; });
	if(it != end)
	{
		outputs.erase(it);
		return true;
	}
	return false;
}

//WARNING: not thread-safe
//YOU MUST USE THE LOG GUARD to use this function
static void Log_PushMessage(const LogBlock& data)
{
	assert(data.level < LogLevel::LOGLEVEL_NONE_SIZE && data.level >= 0);

	LogLevelFlag levelFlag = (LogLevelFlag)(1 << data.level);
	for(LoggingStream& lstream : outputs)
	{
		std::ostream& os = *(lstream.os);

		if(data.level == LogLevel::LOGLEVEL_NONE_FLUSH)
		{
			os.flush();
			continue;
		}
		else if(!(lstream.settings.levelFlags & levelFlag))
		{
			continue;
		}

		bool hasPrefix = LogMsgPrefix[lstream.settings.useLongPrefix][data.level][0] != '\0';
		os << LogTextColourPrefix[lstream.settings.showTextColour][data.level] << LogMsgPrefix[lstream.settings.useLongPrefix][data.level];
		if(lstream.settings.showFile)
		{
			hasPrefix = true;
			os << "<";
			if(lstream.settings.logFullPath)
				os << data.file;
			else
				os << data.file.fileNameView();
			if(lstream.settings.showLineNumber)
				os << " line " << data.line;
			os << ">";
		}
		if(lstream.settings.showThreadId)
		{
			hasPrefix = true;
			os << "[thread " << data.creationId << "]";
		}
		if(hasPrefix)
			os << ": ";
		os << data.log << '\n';
		if(lstream.settings.showTextColour)
			os << LogTextColourReset;
	}
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

#ifdef LOG_USE_LOGGING_THREAD
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
class LoggerThread
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

void gbt::SafeLog_QueueMessage(const LogLevel level, c_string file, const LineNumber line, std::string&& log)
{
	std::lock_guard<std::mutex> lock(logGuard);
	pendingLogs.push(LogBlock{ level, std::this_thread::get_id(), file, line, std::move(log) });
}

void gbt::SafeLog_ImmediatePushMessage(const LogLevel level, c_string file, const LineNumber line, std::string&& log)
{
	SafeLog_QueueMessage(level, file, line, std::move(log));
#ifdef LOG_USE_LOGGING_THREAD
	sleepLogger.notify_all();
#else
	SafeLog_PushAllPendingMessages();
#endif
}

void gbt::SafeLog_PushAllPendingMessages()
{
	std::lock_guard<std::mutex> lock(logGuard);
	
	Log_PushAllPendingMessages();
}