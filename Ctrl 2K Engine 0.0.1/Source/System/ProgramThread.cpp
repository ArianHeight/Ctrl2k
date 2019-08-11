#include <Ctrl 2kPCH.h>

/*

method definitions for ProgramThread wrapper

*/

ProgramThread::ProgramThread(std::thread *t, bool joinWhenTerminated) : m_thread(t), autoJoin(joinWhenTerminated)
{
	//cstr
}

bool ProgramThread::checkJoined()
{
	return this->m_thread->joinable();
}

void ProgramThread::join()
{
	if (this->m_thread->joinable()) //join guard
	{
		this->m_thread->join();
	}
}

void ProgramThread::detach()
{
	if (this->m_thread->joinable()) //guard
	{
		this->m_thread->detach();
	}
}

std::thread::id ProgramThread::id()
{
	return this->m_thread->get_id();
}

ProgramThread::~ProgramThread()
{
	//dstr
	if (this->m_thread != nullptr) //nullptr guard
	{
		if (autoJoin) //joins if autojoin was on
		{
			this->join();
		}
		else
		{
			this->detach();
		}

		delete this->m_thread; //safely deallocates the memory from heap
		this->m_thread = nullptr;
	}
}

/*

end ProgramThread

*/