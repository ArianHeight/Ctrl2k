#include <Ctrl 2kPCH.h>

/*

A GameEvent that CoreEngine uses to keep track of stuff

*/

GameEvent::GameEvent(EventType e, std::vector<GameObject*> &targets) :
	m_type(e), m_targets(targets)
{
	//cstr

}

GameEvent::GameEvent(EventType e) :
	m_type(e)
{
	//cstr
}

//accessors
EventType GameEvent::getType()
{
	return this->m_type;
}

std::vector<GameObject*>& GameEvent::getTargets()
{
	return this->m_targets;
}

GameEvent::~GameEvent()
{
	//dstr
	for (int i = 0; i < this->m_targets.size(); i++)
	{
		this->m_targets[i] = nullptr;
	}
	this->m_targets.clear();
}

/*

end GameEvent

*/