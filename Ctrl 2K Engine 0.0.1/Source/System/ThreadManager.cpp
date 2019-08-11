#include <Ctrl 2kPCH.h>

/*

method declarations for ThreadManager

*/

ThreadManager::ThreadManager()
{
	//empty cstr
}

void ThreadManager::joinThread(std::thread::id id)
{
	for (ProgramThread* t : this->m_threads) //iterates through until id matches
	{
		if (t->id() == id)
		{
			t->join();
			return;
		}
	}
}

void ThreadManager::detachThread(std::thread::id id)
{
	for (ProgramThread* t : this->m_threads) //iterates through until id matches
	{
		if (t->id() == id)
		{
			t->detach();
			return;
		}
	}
}

void ThreadManager::deleteThread(std::thread::id id)
{
	ProgramThread* t; //pointer for iteration
	for (int i = 0; i < this->m_threads.size(); i++) //not for each cause you need the index
	{
		t = this->m_threads[i];
		if (t->id() == id)
		{
			delete t;
			t = nullptr; //m_threads[i] is now deleted
			this->m_threads[i] = nullptr; //sets ptr in list to nullptr
			this->m_threads.erase(m_threads.begin() + i); //deletes the ptr
			return;
		}
	}
}

std::thread::id ThreadManager::addThread(std::thread *t, bool autoJoin)
{
	//adds new thread to list
	ProgramThread *thread = new ProgramThread(t, autoJoin);
	this->m_threads.push_back(thread);
	return thread->id();
}

int ThreadManager::numThreads()
{
	return this->m_threads.size();
}

ThreadManager::~ThreadManager()
{
	//dstr
	for (int i = 0; i < this->m_threads.size(); i++)
	{
		if (this->m_threads[i] != nullptr) //nullptr guard
		{
			delete this->m_threads[i];
			this->m_threads[i] = nullptr; //m_threads[i] is now deleted
		}
	}

	this->m_threads.clear(); //deletes all ptrs
}

/*

end ThreadManager

*/