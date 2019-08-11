#pragma once

/*

A GameEvent that CoreEngine uses to keep track of stuff

*/

class GameEvent
{
public:
	GameEvent(EventType e, std::vector<GameObject*> &targets);
	GameEvent(EventType e);
	~GameEvent();

private:
	EventType m_type;
	std::vector<GameObject*> m_targets;

public:
	EventType getType();
	std::vector<GameObject*>& getTargets();
};

/*

end GameEvent

*/