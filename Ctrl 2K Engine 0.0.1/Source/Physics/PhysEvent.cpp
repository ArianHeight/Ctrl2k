#include <Ctrl 2kPCH.h>

/*

PhysEvent

*/

//cstrs will put data into the right places in accordance to what the two colliding objects are and give the correct collision type
PhysEvent::PhysEvent(Component *one, Component *two) :
	hbOne(one), hbTwo(two), tOne(one->getPhysicsObject()->getType()), tTwo(two->getPhysicsObject()->getType())
{
	//cstr
}

PhysEvent::~PhysEvent()
{
	//dstr does nothing because no ownership
}

/*

end PhysEvent

*/