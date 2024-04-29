//In chage of logging all things
#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <list>
#include <fstream>

#include "Logger.h"

using namespace gbt;

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
},
{
	"",
	"",
	"",
	"",
	"",
	"",
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

// TODO add log time
struct LogBlock
{
	LogLevel level;
	std::thread::id creationId;
	FilePath file;
	LineNumber line;
	std::string log;
};

struct LogFile
{
	FilePath path;
	std::ofstream file;
};

static std::mutex logGuard;
static std::queue<LogBlock> pendingLogs; // TODO maybe fix size this some day?
static std::vector<LoggingStream> outputs;
static std::list<LogFile> logFiles; // linked list, can't use vector here because we need reference stability

static inline void ApplyLogLevelToSettings(LogLevel lvl, LoggingStreamSettings& settings)
{
	settings.levelFlags = (LogLevelFlag)(UINT8_MAX << lvl);
}

static inline void ApplyVerbosityToSettings(LogVerbosity verbosity, LoggingStreamSettings& settings)
{
	switch(verbosity)
	{
	case LOGVERBOSITY_LOW:
		settings.showFile = false;
		settings.showThreadId = false;
	case LOGVERBOSITY_MEDIUM:
		settings.showLineNumber = false;
	case LOGVERBOSITY_HIGH:
		settings.logFullPath = false;
	default:
		break;
	}
}

bool gbt::SafeLog_RegisterFile(const LoggingStreamSettings& settings, FilePath path, bool truncate)
{
	std::ofstream file(path.path(), truncate ? std::ios::out | std::ios::trunc : std::ios::out);
	std::lock_guard<std::mutex> lock(logGuard);
	if(!file.is_open())
	{
		pendingLogs.push(LogBlock{ LOGLEVEL_ERROR, std::this_thread::get_id(), __FILE__, (size_t)__LINE__, path.path() + " could not be opened, could not register to logging system" });
		return false;
	}

	LogFile& lf = logFiles.emplace_back(std::move(LogFile{ std::move(path), std::move(file) }));
	outputs.push_back({ settings, &(lf.file) });
	return true;
}

bool gbt::SafeLog_RegisterFile(LogLevel lvl, LogVerbosity verbosity, const FilePath& path)
{
	LoggingStreamSettings settings;
	ApplyLogLevelToSettings(lvl, settings);
	ApplyVerbosityToSettings(verbosity, settings);
	settings.showTextColour = false;
	settings.logFullPath = false;
	settings.usePrefix = LOGPREFIX_SHORT;

	return SafeLog_RegisterFile(settings, path, true);
}

bool gbt::SafeLog_RegisterFile(LogLevel lvl, const FilePath& path)
{
	return SafeLog_RegisterFile(lvl, LOGVERBOSITY_FULL, path);
}

bool gbt::SafeLog_RegisterFile(const FilePath& path)
{
	return SafeLog_RegisterFile(LOGLEVEL_TRACE, LOGVERBOSITY_FULL, path);
}

bool gbt::SafeLog_RegisterOutputStream(LogLevel lvl, LogVerbosity verbosity, std::ostream& os)
{
	LoggingStreamSettings settings;
	ApplyLogLevelToSettings(lvl, settings);
	ApplyVerbosityToSettings(verbosity, settings);

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

//WARNING: not thread-safe
//YOU MUST USE THE LOG GUARD to use this function
static bool UnsafeLog_DeregisterOutputStream(const std::ostream& os)
{
	const std::ostream* const osptr = &os;
	auto begin = outputs.begin();
	auto end = outputs.end();
	auto it = std::find_if(begin, end, [&osptr](const LoggingStream& ls) { return ls.os == osptr; });
	if(it == end)
	{
		pendingLogs.push(LogBlock{ LOGLEVEL_ERROR, std::this_thread::get_id(), __FILE__, (size_t)__LINE__, "Output stream not found, could not deregister from logging system" });
		return false;
	}
	it->os->flush();
	outputs.erase(it);
	return true;
}

bool gbt::SafeLog_DeregisterFile(const FilePath& path)
{
	std::lock_guard<std::mutex> lock(logGuard);
	auto begin = logFiles.begin();
	auto end = logFiles.end();
	auto it = std::find_if(begin, end, [&path](const LogFile& lf) { return lf.path == path; });
	if(it == end)
	{
		pendingLogs.push(LogBlock{ LOGLEVEL_ERROR, std::this_thread::get_id(), __FILE__, (size_t)__LINE__, path.path() + " not found, could not deregister from logging system" });
		return false;
	}
	bool success = UnsafeLog_DeregisterOutputStream(it->file);
	logFiles.erase(it);
	return success;
}

bool gbt::SafeLog_DeregisterOutputStream(const std::ostream& os)
{
	std::lock_guard<std::mutex> lock(logGuard);
	return UnsafeLog_DeregisterOutputStream(os);
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

		bool hasPrefix = LogMsgPrefix[lstream.settings.usePrefix][data.level][0] != '\0';
		os << LogTextColourPrefix[lstream.settings.showTextColour][data.level] << LogMsgPrefix[lstream.settings.usePrefix][data.level];
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