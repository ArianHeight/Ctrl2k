#pragma once

/*

Managing class for ProgramThread objects

*/

class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();

private:
	std::vector<ProgramThread*> m_threads;

public:
	void joinThread(std::thread::id id); //joins certain thread
	void detachThread(std::thread::id id); //detaches certain thread
	void deleteThread(std::thread::id id); //deletes certain thread
	std::thread::id addThread(std::thread *t, bool autoJoin = true); //adds thread to list
	int numThreads(); //returns number of active threads
};

/*

end ThreadManager

*/