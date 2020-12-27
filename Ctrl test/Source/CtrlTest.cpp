#include <iostream>
#include <chrono>
#include <thread>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <assert.h>

#include "Logger.h"
#include "EngineClock.h"
#include "Profiler.h"

#define CTRLTEST_ERROR std::this_thread::sleep_for(std::chrono::seconds(1)); assert(false);

void logTest()
{
	int firstRep = rand() % 6 + 5; //5-10 reps
	int secondRep = rand() % 11 + 5; //5 - 15 reps
	for(int i = 0; i < firstRep; i++)
	{
		LOG_WARNING_PUSH("This is a log test msg #" + std::to_string(i));
		std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 50 + 1)); //wait between 1ms and 50ms
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 3000)); //wait between 0s and 3s

	for(int i = 0; i < secondRep; i++)
	{
		LOG_MSG_PUSH("This is a log test msg #" + std::to_string(i));
		std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 490 + 10)); //wait between 10ms and 0.5s
	}
}

void runLoggerTest()
{
	srand(time(NULL));

	std::vector<std::thread> testers;
	testers.reserve(5);

	LOG_MSG_QUEUE("Queued msg");
	LOG_WARNING_QUEUE("Queued warning");
	LOG_FATAL_QUEUE("Queued error");
	LOG_QUEUE(LogType::LOGTYPE_NONE, "Another queued msg");
	LOG_FLUSH_QUEUE();
	SafeLog_PushAllPendingMessages();
	LOG_MSG_PUSH("All Queued Msgs Pushed");

	for(int i = 0; i < 5; i++)
	{
		testers.push_back(std::thread(logTest));
	}


	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	LOG_MSG_PUSH("Everything is going fine");

	std::this_thread::sleep_for(std::chrono::milliseconds(0));

	LOG_FATAL_PUSH("We've hit a fatal error");

	for(int i = 0; i < 5; i++)
	{
		testers[i].join();
	}

	LOG_WARNING_PUSH("If program has printed anything at this point, logger tests successful");
	LOG_FLUSH();
}

template <typename T>
bool IsAccurate(const T& value, const T& comparator, const T& deltaEpsilon, const double& percentEpsilon)
{
	if(abs(value - comparator) <= deltaEpsilon || abs(value - comparator) <= abs(comparator * percentEpsilon))
		return true;
	return false;
}

void LifeTimeClockTest(const double percentAccuracy)
{
	long time = 0;
	long testTime = 1e9;
	long deltaAccuracy = 50000;
	{
		auto timer = MiniLifeTimeClock(time);
		std::this_thread::sleep_for(std::chrono::nanoseconds(testTime));
	}
	LOG_MSG_PUSH("MiniLifeTimeClock test time " + std::to_string(testTime) + ", recorded time " + std::to_string(time));
	if(!IsAccurate(time, testTime, deltaAccuracy, percentAccuracy))
	{
		LOG_FATAL_PUSH("MiniLifeTimeClock test failed: time mismatch");
		CTRLTEST_ERROR
	}

	testTime = 1e6;
	deltaAccuracy = 30000;
	{
		auto timer = ShortLifeTimeClock(time);
		std::this_thread::sleep_for(std::chrono::microseconds(testTime));
	}
	LOG_MSG_PUSH("ShortLifeTimeClock test time " + std::to_string(testTime) + ", recorded time " + std::to_string(time));
	if(!IsAccurate(time, testTime, deltaAccuracy, percentAccuracy))
	{
		LOG_FATAL_PUSH("ShortLifeTimeClock test failed: time mismatch");
		CTRLTEST_ERROR
	}

	testTime = 1e3;
	deltaAccuracy = 30;
	{
		auto timer = MedLifeTimeClock(time);
		std::this_thread::sleep_for(std::chrono::milliseconds(testTime));
	}
	LOG_MSG_PUSH("MedLifeTimeClock test time " + std::to_string(testTime) + ", recorded time " + std::to_string(time));
	if(!IsAccurate(time, testTime, deltaAccuracy, percentAccuracy))
	{
		LOG_FATAL_PUSH("MedLifeTimeClock test failed: time mismatch");
		CTRLTEST_ERROR
	}

	testTime = 5;
	deltaAccuracy = 1;
	{
		auto timer = LongLifeTimeClock(time);
		std::this_thread::sleep_for(std::chrono::seconds(testTime));
	}
	LOG_MSG_PUSH("LongLifeTimeClock test time " + std::to_string(testTime) + ", recorded time " + std::to_string(time));
	if(!IsAccurate(time, testTime, deltaAccuracy, percentAccuracy))
	{
		LOG_FATAL_PUSH("LongLifeTimeClock test failed: time mismatch");
		CTRLTEST_ERROR
	}

	LOG_MSG_PUSH("LifeTimeClock tests successful");
	LOG_FLUSH();
}

void runEngineBaseClockTest(double accuracy)
{
	const int fps = 10;
	const long usElapsed = 1e6 / fps;
	EngineBaseClock c;
	for(int i = 0; i < 2 * ENGINECLOCK_INTERNAL_TIMESLOTS; i++)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(usElapsed));
		if(!IsAccurate(c.QueryTime(), usElapsed, 0L, accuracy))
		{
			LOG_FATAL_PUSH("EngineBaseClock " + std::to_string(fps) + "fps test failed time expected: " + 
				std::to_string(usElapsed) + "us, time recorded " + std::to_string(c.GetTimeElapsed()) + "us");
			CTRLTEST_ERROR
		}
	}
	LOG_MSG_PUSH("EngineBaseClock " + std::to_string(fps) + "fps test successful with accuracy " + std::to_string(accuracy));
	LOG_FLUSH();
}

void runCtrlEngineClockTest(double accuracy)
{
	const int fps = 10;
	const long usElapsed = 1e6 / fps;
	CtrlEngineClock c;

	for(int i = 0; i < 2 * ENGINECLOCK_INTERNAL_TIMESLOTS; i++)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(usElapsed));
		if(!IsAccurate(c.QueryTime(), usElapsed, 0L, accuracy) )
		{
			LOG_FATAL_PUSH("CtrlEngineClock " + std::to_string(fps) + "fps test failed time expected: " +
				std::to_string(usElapsed) + "us, time recorded " + std::to_string(c.GetTimeElapsed()) + "us");
			CTRLTEST_ERROR
		}
		else if(!IsAccurate(c.getQPS(), (double)fps, 0.0, accuracy))
		{
			LOG_FATAL_PUSH("CtrlEngineClock " + std::to_string(fps) + 
				" fps test failed true fps:" + std::to_string(c.getQPS()) + 
				" low fps:" + std::to_string(c.getLowQPS()) + 
				" high fps:" + std::to_string(c.getHighQPS()) + 
				" avg fps:" + std::to_string(c.getAvgQPS()));
			CTRLTEST_ERROR
		}
	}

	std::this_thread::sleep_for(std::chrono::microseconds((long) (usElapsed / ((1.0 - accuracy) * (1.0 - ENGINECLOCK_INTENAL_HITCH_VARIANCE)))));
	c.QueryTime();
	if(c.GetTrueSecondsElapsed() <= c.GetSecondsElapsedModifiedForHitch())
	{
		LOG_FATAL_PUSH("CtrlEngineClock " + std::to_string(fps) + "fps test failed true time elapsed: " +
			std::to_string(c.GetTrueSecondsElapsed()) + "s, modified time elapsed: " + std::to_string(c.GetSecondsElapsedModifiedForHitch()) + "s");
		CTRLTEST_ERROR
	}

	LOG_MSG_PUSH("CtrlEngineClock " + std::to_string(fps) + "fps test successful with accuracy " + std::to_string(accuracy));
	LOG_FLUSH();
}

void runProfilerTest()
{
	const int sleepTimeSeconds = 1;
	{
		TOOLS_PROFILE();
		TOOLS_MULTIPROFILE(profiler1);

		std::this_thread::sleep_for(std::chrono::seconds(sleepTimeSeconds));
		LOG_MSG_PUSH("Profiler test sleeping for " + std::to_string(sleepTimeSeconds) + "s, recorded times below");
	}

	{
		TOOLS_PROFILE_LOG("Profile msg 1");
		TOOLS_MULTIPROFILE_LOG(profiler1, "Profile msg 2");

		std::this_thread::sleep_for(std::chrono::seconds(sleepTimeSeconds));
		LOG_MSG_PUSH("Profiler log test sleeping for " + std::to_string(sleepTimeSeconds) + "s, recorded times below");
	}

	{
		TOOLS_HP_PROFILE();
		TOOLS_HP_MULTIPROFILE(profiler1);

		std::this_thread::sleep_for(std::chrono::seconds(sleepTimeSeconds));
		LOG_MSG_PUSH("High precision profile test sleeping for " + std::to_string(sleepTimeSeconds) + "s, recorded times below");
	}

	{
		TOOLS_HP_PROFILE_LOG("HP profile msg 1");
		TOOLS_HP_MULTIPROFILE_LOG(profiler1, "HP profile msg 2");

		std::this_thread::sleep_for(std::chrono::seconds(sleepTimeSeconds));
		LOG_MSG_PUSH("High precision profiler log test sleeping for " + std::to_string(sleepTimeSeconds) + "s, recorded times below");
	}

	LOG_MSG_PUSH("If all 12 msgs are pushed above, then profiler tests successful");
	LOG_FLUSH();
}

int main()
{
	runLoggerTest();
	LifeTimeClockTest(0.2);
	runEngineBaseClockTest(0.2);
	runCtrlEngineClockTest(0.2);
	runProfilerTest();

	return 0;
}