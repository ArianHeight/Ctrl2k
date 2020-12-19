#include <iostream>
#include <chrono>
#include <thread>
#include <stdlib.h>
#include <time.h>
#include <vector>

#include "Logger.h"

void logTest()
{
	int firstRep = rand() % 6 + 5; //5-10 reps
	int secondRep = rand() % 11 + 10; //10 - 20 reps
	for(int i = 0; i < firstRep; i++)
	{
		LOG_FATAL_PUSH("This is a log test msg #" + std::to_string(i));
		std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 50 + 1)); //wait between 1ms and 50ms
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 3000 + 2000)); //wait between 2s and 5s

	for(int i = 0; i < secondRep; i++)
	{
		LOG_FATAL_PUSH("This is a log test msg #" + std::to_string(i));
		std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 500 + 100)); //wait between 100ms and 0.5s
	}
}

void runLoggerTest()
{
	srand(time(NULL));

	std::vector<std::thread> testers;
	testers.reserve(5);

	for(int i = 0; i < 5; i++)
	{
		testers.push_back(std::thread(logTest));
	}


	std::this_thread::sleep_for(std::chrono::milliseconds(15000));

	std::cout << "Hello World" << std::endl;

	std::this_thread::sleep_for(std::chrono::milliseconds(0));

	LOG_MSG_PUSH("Everything is going fine");

	std::this_thread::sleep_for(std::chrono::milliseconds(0));

	LOG_FATAL_PUSH("We've hit a fatal error");

	for(int i = 0; i < 5; i++)
	{
		testers[i].join();
	}

	LOG_FATAL_PUSH("SHOULD PRINT NOW");
}

int main()
{
	runLoggerTest();

	return 0;
}