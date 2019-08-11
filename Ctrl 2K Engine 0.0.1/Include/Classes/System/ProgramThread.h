#pragma once

/*

Wrapper class around std::thread

*/

class ProgramThread
{
public:
	ProgramThread(std::thread *t, bool joinWhenTerminated = true); //autoJoin is on, strongly recommended to not detach threads due to unaccounted behaviour when dealing with detached threads and memory
	~ProgramThread();

private:
	std::thread *m_thread;
	bool autoJoin; //autoJoins when class is deleted if true

public:
	bool checkJoined(); //checks if you can join the thread to main thread
	void join(); //joins thread
	void detach(); //detaches thread
	std::thread::id id(); //returns the id of thread
};

/*

end ProgramThread class

*/