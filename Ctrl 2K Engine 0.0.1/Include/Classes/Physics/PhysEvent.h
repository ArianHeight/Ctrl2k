#pragma once

/*

and event for the physengine

*/

class PhysEngine;

class PhysEvent
{
	friend class PhysEngine;

public:
	PhysEvent(Component *one, Component *two);
	~PhysEvent();
	
private:
	PhysType tOne;
	PhysType tTwo;

	Component *hbOne;
	Component *hbTwo;
};

/*

end PhysEvent

*/