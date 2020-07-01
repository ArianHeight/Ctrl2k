#include <Ctrl 2kPCH.h>

//helper function. allows for equality testing with room for absolute error
bool epsilonEqual(double inOne, double inTwo, double epsilon)
{
	if (abs(inTwo - inOne) <= epsilon)
	{
		return true;
	}
	return false;
}

//helper function. allows for a special cross product formula needed for line-line
float specialCross(glm::vec2 one, glm::vec2 two)
{
	return ((one.x * two.y) - (one.y * two.x));
}

//helper function
template <typename T>
const bool inVector(const T& in, const std::vector<T>& theList);

/*
main physics engine
*/

//cstr. Sets necessary values and reserves spots for collisions and such
PhysEngine::PhysEngine(Game* game, Map *tile, double &timeRef) :
	m_game(game), playgroundTile(tile), m_timeRef(timeRef)
{
	this->m_possibleCollisions.reserve(64);
	this->m_collidingAABB.reserve(32);
}

//call this method everytime the map changes
void PhysEngine::mapChange(Map *tile)
{
	this->playgroundTile = tile;
}

void PhysEngine::compileHitboxList()
{
	this->m_hitboxList.clear(); //clears buffer
	Entity *object; //testing values
	DamageArea *dmg; //testing value

	//fills collision list
	this->m_game->getAllPOs(this->m_hitboxList);

	//resets all POs
	for (int index = 0; index < this->m_hitboxList.size(); index++)
	{
		this->m_hitboxList[index]->getPhysicsObject()->updateHitbox();
	}
}

//background collision response
void PhysEngine::backgroundCollision()
{
	//testing values
	Manifest output;
	Component* position; //Transformation
	Component* hitbox; //physcomponent
	std::string cPos = "Position"; //current position
	std::string lPos = "Last Position"; //last position

	//against background
	int size = this->m_backgroundColliding.size();
	for (int i = 0; i < size; i++) //processes the list of physcomponents colliding with the map
	{
		hitbox = this->m_backgroundColliding[i];
		position = hitbox->getGameObjectPtr()->getComponent(0); //gets displacement
		if (position->getDisplacement() != hitbox->getGameObjectPtr()->getComponent(1)->getDisplacement()) //only updates if moved
		//if (true)
		{
			//updates test values
			output = this->playgroundTile->m_collisionMap->mapCollisionResponse(position, hitbox); //response
			if (!isnan(output.normal.x) && !isnan(output.normal.y)) //nan check
			{
				position->changeDisp(output.normal * output.magnitude); //response goes to entity in a manifest
			}
		}
	}
}

//builds a list of background collisions and possibly colliding objects
void PhysEngine::buildCollisionList(bool &mapChange, int &newMap)
{
	if (this->playgroundTile == nullptr) //temp
	{
		return; //early exit if no map
	} //temp

	//clears buffers
	this->m_backgroundColliding.clear();
	this->m_possibleCollisions.clear();
	this->m_collisionEvents.clear();
	this->m_collidingAABB.clear();

	//testing values
	Component *object;
	Component *transform;
	std::string pos = "Position";
	std::string tempName = "CollisionOn";
	glm::vec2 testPos;
	glm::vec2 spawnPos;
	float testDepth;
	mapChange = false;

	//fills collision list
	this->compileHitboxList();

	for (int index = 0; index < this->m_hitboxList.size(); index++) //iterates through all entities
	{
		//updates testing values
		object = this->m_hitboxList[index];
		transform = object->getGameObjectPtr()->getComponent(0);
		testPos = transform->getDisplacement();

		//hitbox against hitbox checking
		for (int sIndex = index + 1; sIndex < this->m_hitboxList.size(); sIndex++)
		{
			if (this->collision(object, this->m_hitboxList[sIndex])) //check for collision
			{
				//std::cout << "touching" << '\n';
				this->m_collisionEvents.push_back(PhysEvent(this->m_hitboxList[index], this->m_hitboxList[sIndex]));
			}
		}

		//for background testing
		if (this->playgroundTile->m_collisionMap->mapCollisionCheck(transform, object))
		{
			tempName = "CollisionOn";
			if (object->getGameObjectPtr()->hasProperty(tempName))
			{
				this->m_backgroundColliding.push_back(object);
			}

			tempName = "DeleteOnCollision";
			if (object->getGameObjectPtr()->hasProperty(tempName))
			{
				object->getGameObjectPtr()->deleteGO();
			}

			tempName = "ParticleSpawnOnCollision";
			if (object->getGameObjectPtr()->hasProperty(tempName))
			{
				spawnPos = object->getGameObjectPtr()->getComponent(0)->getDisplacement();
				//this->m_game->getEngine().getParticleEngine().addEffect(new ExplosiveFX(spawnPos, 0.2, -0.00001, 0.1f, 9));
				this->m_game->getEngine().getParticleEngine().addEffect(new SparkFX(spawnPos, 0.1, object->getGameObjectPtr()->getComponent(3)->getDisplacement(), 5));
			}
		}
		else
		{
			//object->clearCollidingSide();
		}

		//player only, check against connector hitboxes
		if (object->getGameObjectPtr()->isCamLinked())
		{
			const std::vector<Component*>& connectors = this->playgroundTile->getAllActiveComponents(CONNECTOR);
			for (int i = 0; i < connectors.size(); i++)
			{
				if (this->aabbCheck(testPos, connectors[i]->getSelfHB()))
				{
					mapChange = true; //global queue map change
					newMap = this->m_game->getEngine().getAssets().getMM().search(connectors[i]->getTarget());
					this->playerMapChange(transform, connectors[i]);
				}
			}
		}/*
		else //temp for ai testing
		{
			object->updateAggro(this->objectList[0]);
		}*/
	}
}

//entity hitbox collision check
void PhysEngine::updateCollidingObjects()
{
	PhysEvent *e; //test value

	//does collision response on all collision events
	for (int i = 0; i < this->m_collisionEvents.size(); i++)
	{
		e = &(this->m_collisionEvents[i]);
		this->collisionResponse(e);
	}
}

//main method called by idle() every frame
void PhysEngine::updateObjects()
{
	if (this->playgroundTile == nullptr)
	{
		return; //early exit if no map
	}

	this->updateCollidingObjects();
	this->backgroundCollision();
}

//math
float PhysEngine::getDistanceFromVelocity(float initVelocity, float downAcceleration, double time)
{
	return (initVelocity + float(0.5f * downAcceleration * time * time));
}

float PhysEngine::getFVelocityFromDistance(float initVelocity, float downAcceleration, float distance)
{
	return glm::sqrt(initVelocity * initVelocity + 2.0f * downAcceleration * distance) * (glm::abs(initVelocity) / initVelocity);
}

glm::vec2 PhysEngine::getDistanceFromVelocity(glm::vec2 initVelocity, glm::vec2 downAcceleration, double time)
{
	return (initVelocity + (0.5f * downAcceleration * float(time * time)));
}

glm::vec2 PhysEngine::getFVelocityFromDistance(glm::vec2 initVelocity, glm::vec2 downAcceleration, glm::vec2 distance)
{
	return glm::sqrt(initVelocity * initVelocity + 2.0f * downAcceleration * distance) * (glm::abs(initVelocity) / initVelocity);
}


//for particles
void PhysEngine::updateParticle(Particle *p)
{
	glm::vec2 dist = this->getDistanceFromVelocity(p->getVelocity(), p->getAcceleration(), this->m_timeRef);
	glm::vec2 vel = this->getFVelocityFromDistance(p->getVelocity(), p->getAcceleration(), dist);

	p->addPos(dist);
	p->setVelocity(vel);
	p->updateTime(this->m_timeRef);
}

void PhysEngine::updateParticleFX(ParticleEffect *vfx)
{
	std::vector<Particle*> &particles = vfx->getBufferRef();
	
	//iteration
	for (int i = 0; i < particles.size(); i++)
	{
		this->updateParticle(particles[i]);
	}
}

void PhysEngine::spawnExplosiveParticleFx(PhysEvent *e, bool invert)
{
	glm::vec2 position;
	//std::string name = "Position";
	if (!invert)
	{
		position = e->hbOne->getGameObjectPtr()->getComponent(0)->getDisplacement();
	}
	else
	{
		position = e->hbTwo->getGameObjectPtr()->getComponent(0)->getDisplacement();
	}

	this->m_game->getEngine().getParticleEngine().addEffect(new ExplosiveFX(position, 0.1, -0.00001, 0.1f, 9));
}

void PhysEngine::spawnSparkParticleFx(PhysEvent *e, bool invert)
{
	glm::vec2 position;
	glm::vec2 velocity;
	std::string name = "Position";
	std::string nameTwo = "Velocity";
	if (!invert)
	{
		position = e->hbOne->getGameObjectPtr()->getComponent(0)->getDisplacement();
		velocity = e->hbOne->getGameObjectPtr()->getComponent(3)->getDisplacement();
	}
	else
	{
		position = e->hbTwo->getGameObjectPtr()->getComponent(0)->getDisplacement();
		velocity = e->hbTwo->getGameObjectPtr()->getComponent(3)->getDisplacement();
	}

	this->m_game->getEngine().getParticleEngine().addEffect(new SparkFX(position, 0.1, velocity, 5));
}

/*
//damage response
void PhysEngine::dmgDmgResponse(PhysEvent *e)
{
	Entity *one = e->m_entities[0];
	DamageArea *two = e->m_dmg[0];
	std::vector<Entity*> &immuneEntities = two->getImmuneRef();

	for (Entity* immuneEntity : immuneEntities) //makes sure that the attack wasn't created by the intersecting entity
	{
		if (immuneEntity == one)
		{
			return; //early return colliding entity is immune
		}
	}

	if (two->getHitboxPtr() != nullptr) //means dmgarea has an aabb hitbox
	{
		two->doDamage(one);
	}
}
/*
//collision response
void PhysEngine::entityEntityResponse(PhysEvent *e)
{
	PhysObject *one = e->m_entities[0]->getMainBodyPtr(); //first object in collision
	PhysObject *two = e->m_entities[1]->getMainBodyPtr(); //second obj in collision

	glm::vec2 topLeft = one->getHB()->tl - two->getHB()->br; //one - two
	glm::vec2 bottomRight = one->getHB()->br - two->getHB()->tl; //one - two
	glm::vec2 move; //how much two will move in wp
	
	if (glm::abs(topLeft.x) < glm::abs(bottomRight.x)) //least amount of movement horizontally
	{
		move.x = topLeft.x;
	}
	else
	{
		move.x = bottomRight.x;
	}
	if (glm::abs(topLeft.y) < glm::abs(bottomRight.y)) //least amount of movement needed vertically
	{
		move.y = topLeft.y;
	}
	else
	{
		move.y = bottomRight.y;
	}
	if (glm::abs(move.x) < glm::abs(move.y)) //move up or move down
	{
		move.y = 0;
	}
	else
	{
		move.x = 0;
	}

	float weightPercentageOne = two->m_mass / (one->m_mass + two->m_mass); //percentage of two's move weight to one's move weight
	if (two->m_mass >= 9999.99)
	{
		weightPercentageOne = 1.0f;
	}
	if (one->m_mass >= 9999.99)
	{
		weightPercentageOne = 0.0f;
	}
	e->m_entities[0]->engineMove(glm::vec3(-move * weightPercentageOne, 0.0f)); //one moves opposite direction
	e->m_entities[1]->engineMove(glm::vec3(move * (1.0f - weightPercentageOne), 0.0f)); //two moves
}
*/
//quick axis-aligned bounding box check with a point
bool PhysEngine::aabbCheck(glm::vec2 &wpOne, FourPoints *wpTwo)
{
	if (wpOne.x > wpTwo->tl.x && wpOne.x < wpTwo->tr.x)
	{
		if (wpOne.y < wpTwo->tl.y && wpOne.y > wpTwo->bl.y)
		{
			return true;
		}
	}
	return false;
}

//quick axis-aligned bounding box check
bool PhysEngine::aabbCheck(FourPoints *wpOne, FourPoints *wpTwo)
{
	if (wpOne->tl.x < wpTwo->tr.x && wpOne->tr.x > wpTwo->tl.x) //x axis overlap
	{
		if (wpOne->tl.y > wpTwo->bl.y && wpOne->bl.y < wpTwo->tl.y) //y axis overlaps
		{
			return true; //both axis overlap
		}
	}
	return false;
}

//quick circle check
bool PhysEngine::circleCheck(glm::vec2 &point, glm::vec2& origin, float radius)
{
	float distanceX = origin.x - point.x;
	float distanceY = origin.y - point.y;
	float distance = glm::sqrt((distanceX * distanceX) + (distanceY * distanceY)); //gets distance between two points

	if (distance <= radius) //point is in cicle's range
	{
		return true;
	}

	return false;
}

//quick circle check
bool PhysEngine::circleCheck(glm::vec2 &originOne, float radiusOne, glm::vec2& originTwo, float radiusTwo)
{
	return this->circleCheck(originOne, originTwo, radiusOne + radiusTwo); //does the same thing
}

//checks for a line to line intersection
bool PhysEngine::lineIntersectCheck(DamageArea *lineOne, DamageArea *lineTwo)
{
	/*
	bool aabbIn = this->aabbCheck(&(lineOne->m_approxHitbox), &(lineTwo->m_approxHitbox));

	if (!aabbIn) //bounding boxes not in
	{
		return false;
	}*/
	/*
	glm::vec2 q = lineOne->m_origin; //origin 1
	glm::vec2 r = lineOne->m_endPoint - q; //length 1
	glm::vec2 t = lineTwo->m_origin; //origin 2
	glm::vec2 u = lineTwo->m_endPoint - t; //length 2
	float s = 0; //scalar representing a point on the line 1
	float v = 0; //point on line 2 *point = t + vu

	float qtr = specialCross((q - t), r); //v = (q-t)xr/(uxr)
	float qtu = specialCross((q - t), u); //s = (q-t)xu/(uxr)
	float ur = specialCross(u, r);

	if (!epsilonEqual(ur, 0.0, 0.01)) //no divides by 0
	{
		s = qtu / ur;
		v = qtr / ur;
		if ((0.0f <= s && s <= 1.0f) && (0.0f <= v && v <= 1.0f))
		{
			return true;
		}
	}
	else if (epsilonEqual(qtu, 0.0, 0.01)) //collinear testing
	{
		float tOne = glm::dot((q - t), u) / glm::dot(u, u); //vector form scalar of q in terms of t + vu. tOne.x should = tOne.y
		float tTwo = tOne + (glm::dot(r, u) / glm::dot(u, u)); //q + r in terms of t + vu

		if ((0.0f <= tOne && tOne <= 1.0f) || (0.0f <= tTwo && tTwo <= 1.0f)) //collinear and joint
		{
			return true;
		}
	}

	return false;
	*/
	/*
	glm::vec2 pointLine; //lineOne (origin is 0)
	glm::vec3 yPointLine; //cross product to find rotation
	glm::vec2 pointOne; //lineTwo's origin's position (lineOne's origin as 0)
	glm::vec2 pointTwo; //lineTwo's endpoint position (lineOne's origin as 0)
	glm::mat2 rotation; //rotation of lineOne

	pointLine = lineOne->m_endPoint - lineOne->m_origin;
	pointOne = lineTwo->m_origin - lineOne->m_origin;
	pointTwo = lineTwo->m_endPoint - lineOne->m_origin;

	yPointLine = glm::cross(glm::vec3(pointLine.x, pointLine.y, 0.0), glm::vec3(0.6, 0.0, 0.8));
	rotation = glm::mat2(pointLine, glm::vec2(yPointLine.x, yPointLine.y));
	//rotates all 3 points
	pointOne = rotation * pointOne; 
	pointTwo = rotation * pointTwo;
	pointLine = rotation * pointLine;
	aabbIn = (pointOne.y * pointTwo.y < 0) || (epsilonEqual(pointOne.y * pointTwo.y, 0.0, 0.05) && (0 < pointOne.x < pointLine.x || 0 < pointTwo.x < pointLine.x)); //lineOne is now on the x axis, lineTwo has been rotated, if lineTwo's two points are both to one side of the axis, then they are not touching
	if (!aabbIn) //check one
	{
		return false;
	}

	//same thing as above, but lineOne and lineTwo are reversed
	pointLine = lineTwo->m_endPoint - lineTwo->m_origin;
	pointOne = lineOne->m_origin - lineTwo->m_origin;
	pointTwo = lineOne->m_endPoint - lineTwo->m_origin;

	yPointLine = glm::cross(glm::vec3(pointLine.x, pointLine.y, 0.0), glm::vec3(0.6, 0.0, 0.8));
	rotation = glm::mat2(pointLine, glm::vec2(yPointLine.x, yPointLine.y));
	pointOne = rotation * pointOne;
	pointTwo = rotation * pointTwo;
	pointLine = rotation * pointLine;
	aabbIn = (pointOne.y * pointTwo.y < 0) || (epsilonEqual(pointOne.y * pointTwo.y, 0.0, 0.05) && (0 < pointOne.x < pointLine.x || 0 < pointTwo.x < pointLine.x));
	if (!aabbIn) //check two
	{
		return false;
	}

	return true;*/
	return false;
}

void PhysEngine::playerMapChange(Component *position, Component *connector)
{
	glm::vec2 playerPos = position->getDisplacement();
	glm::vec2 camRelative = this->m_game->getEngine().getRenderEngine().getActiveCam()->getPos() - playerPos;
	FourPoints currentArea = *(connector->getSelfHB());
	FourPoints targetArea = *(connector->getTargetHB());

	//player position relative to "teleport box"
	float magCurrent = sqrt(pow(currentArea.tr.x - currentArea.bl.x, 2) + pow(currentArea.tr.y - currentArea.bl.y, 2));
	float magTarget = sqrt(pow(targetArea.tr.x - targetArea.bl.x, 2) + pow(targetArea.tr.y - targetArea.bl.y, 2));
	float ratio = 1.0f;
	if (magCurrent != 0)
	{
		ratio = magTarget / magCurrent;
	}
	glm::vec2 blDiff = playerPos - currentArea.bl;
	glm::vec2 trDiff = currentArea.tr - playerPos;

	//puts player in a relative position in the connected teleport box in the new active map
	if (trDiff.x <= blDiff.x)
	{
		if (trDiff.x <= trDiff.y && trDiff.x <= blDiff.y) //right closer, exit left
		{
			position->setDisp(glm::vec2(targetArea.bl.x, targetArea.bl.y + (blDiff.y * ratio)));
		}
		else if (trDiff.y <= blDiff.y) //top closer, exit bottom
		{
			position->setDisp(glm::vec2(targetArea.bl.x + (blDiff.x * ratio), targetArea.bl.y));
		}
		else //bottom closer, exit top
		{
			position->setDisp(glm::vec2(targetArea.bl.x + (blDiff.x * ratio), targetArea.tr.y));
		}
	}
	else
	{
		if (blDiff.x <= trDiff.y && blDiff.x <= blDiff.y) //left closer, exit right
		{
			position->setDisp(glm::vec2(targetArea.tr.x, targetArea.bl.y + (blDiff.y * ratio)));
		}
		else if (trDiff.y <= blDiff.y) //top closer, exit bottom
		{
			position->setDisp(glm::vec2(targetArea.bl.x + (blDiff.x * ratio), targetArea.bl.y));
		}
		else //bottom closer, exit top
		{
			position->setDisp(glm::vec2(targetArea.bl.x + (blDiff.x * ratio), targetArea.tr.y));
		}
	}

	//puts camera in the correct relative position to the player in the new map
	this->m_game->getEngine().getRenderEngine().getActiveCam()->setPos(position->getDisplacement() + camRelative);
}

bool PhysEngine::collision(Component *one, Component *two)
{
	//Physics Object
	PhysObject *oneO = one->getPhysicsObject();
	PhysObject *twoO = two->getPhysicsObject();

	//for type
	PhysType tOne = oneO->getType();
	PhysType tTwo = twoO->getType();

	//position
	//std::string temp = "Position"; //temporary string for storing names
	Component *oneT = one->getGameObjectPtr()->getComponent(0);
	Component *twoT = two->getGameObjectPtr()->getComponent(0);

	if (tOne == RECTANGLE && tTwo == tOne) //aabb against aabb, also fastest algorithmn
	{
		//temp values
		FourPoints oneFP = *(oneO->getHB()) * oneT->getSize() + oneT->getDisplacement();
		FourPoints twoFP = *(twoO->getHB()) * twoT->getSize() + twoT->getDisplacement();

		return this->aabbCheck(&oneFP, &twoFP);
	}
	else if (tOne == CIRCLE && tTwo == tOne) //circle against circle, basic distance calculation
	{
		return this->circleCheck(oneO->getCenter() + oneT->getDisplacement(), oneO->getRadius() * oneT->getSize(), 
			twoO->getCenter() + twoT->getDisplacement(), twoO->getRadius() * twoT->getSize());
	}
	else if (tOne == FAN && tTwo == tOne) //fan against fan, circle test plus angle
	{
		//research how to do this
		if (!this->circleCheck(oneO->getCenter() + oneT->getDisplacement(), oneO->getRadius() * oneT->getSize(),
			twoO->getCenter() + twoT->getDisplacement(), twoO->getRadius() * twoT->getSize())) //does not collide if doesn't even pass circle test
		{
			return false;
		}

		//angle check
	}

	return false; //default return value
}

bool PhysEngine::dmgResponse(PhysEvent *e, bool invert)
{
	//setup
	std::string name = "Damage";
	GameObject *goOne = e->hbOne->getGameObjectPtr();
	GameObject *goTwo = e->hbTwo->getGameObjectPtr();

	if (!invert) //hbOne is the damage area
	{
		return goOne->doDamage(e->hbTwo->getGameObjectPtr());
	}
	else //hbOne is the damage area
	{
		return goTwo->doDamage(e->hbOne->getGameObjectPtr());
	}
}

void PhysEngine::voidResponse(PhysEvent *e, bool invert)
{
	if (!invert)
	{
		e->hbOne->getGameObjectPtr()->deleteGO();
	}
	else
	{
		e->hbTwo->getGameObjectPtr()->deleteGO();
	}
}

void PhysEngine::aabbResponse(PhysEvent *e)
{
	//hitbox
	PhysObject *one = e->hbOne->getPhysicsObject(); //for object one
	PhysObject *two = e->hbTwo->getPhysicsObject(); //for object two

	//early return no move
	if (one->getMass() == two->getMass() && one->getMass() == -250.0f)
	{
		return; //infinite mass no move
	}

	//position
	//std::string temp = "Position"; //temporary string for storing names
	Component *oneT = e->hbOne->getGameObjectPtr()->getComponent(0);
	Component *twoT = e->hbTwo->getGameObjectPtr()->getComponent(0);

	//last pos
	//temp = "Last Position";
	Component *oneLT = e->hbOne->getGameObjectPtr()->getComponent(1);
	Component *twoLT = e->hbTwo->getGameObjectPtr()->getComponent(1);

	//needed for calculations, one and two FP are hitboxes scaled to size and in world position
	FourPoints *fPPtr = one->getHB();
	FourPoints oneFP = FourPoints{ fPPtr->tl, fPPtr->tr, fPPtr->bl, fPPtr->br } * oneT->getSize() + oneT->getDisplacement(); //copy data and manipulate
	fPPtr = two->getHB();
	FourPoints twoFP = FourPoints{ fPPtr->tl, fPPtr->tr, fPPtr->bl, fPPtr->br } * twoT->getSize() + twoT->getDisplacement(); //copy data and manipulate
	fPPtr = nullptr; //no ownership

	//for checking distances
	glm::vec2 topLeft = oneFP.tl - twoFP.br; //one - two
	glm::vec2 bottomRight = oneFP.br - twoFP.tl; //one - two
	glm::vec2 move; //how much two will move in wp

	if (glm::abs(topLeft.x) < glm::abs(bottomRight.x)) //least amount of movement horizontally
	{
		move.x = topLeft.x;
	}
	else
	{
		move.x = bottomRight.x;
	}
	if (glm::abs(topLeft.y) < glm::abs(bottomRight.y)) //least amount of movement needed vertically
	{
		move.y = topLeft.y;
	}
	else
	{
		move.y = bottomRight.y;
	}
	if (glm::abs(move.x) < glm::abs(move.y)) //move up or move down
	{
		move.y = 0;
	}
	else
	{
		move.x = 0;
	}

	float weightPercentageOne = two->getMass() / (one->getMass() + two->getMass()); //percentage of two's move weight to one's move weight
	//applies weight percentages
	if (two->m_mass == -250.0f)
	{
		weightPercentageOne = 0.0f;
	}
	if (one->m_mass == -250.0f)
	{
		weightPercentageOne = 1.0f;
	}

	oneT->changeDisp(-move * weightPercentageOne); //one moves negative direction
	twoT->changeDisp(move * (1.0f - weightPercentageOne)); //two moves opposite percentage of one
}

void PhysEngine::circleResponse(PhysEvent *e)
{
	//objects
	PhysObject *one = e->hbOne->getPhysicsObject(); //for object one
	PhysObject *two = e->hbTwo->getPhysicsObject(); //for object two

	//early return no move
	if (one->getMass() == two->getMass() && one->getMass() == -250.0f)
	{
		return; //infinite mass no move
	}

	//for transformation
	//std::string tempName = "Position";
	Component *oneT = e->hbOne->getGameObjectPtr()->getComponent(0);
	Component *twoT = e->hbTwo->getGameObjectPtr()->getComponent(0);

	glm::vec2 distance = two->getCenter() + twoT->getDisplacement() - one->getCenter() - oneT->getDisplacement(); //distance from one to two
	//float absDistance = glm::sqrt(distance.x * distance.x + distance.y * distance.y); //pythagorean

	float weightPercentageOne = two->getMass() / (one->getMass() + two->getMass()); //percentage of two's move weight to one's move weight
	//applies weight percentages
	if (two->m_mass == -250.0f)
	{
		weightPercentageOne = 1.0f;
	}
	if (one->m_mass >= -250.0f)
	{
		weightPercentageOne = 0.0f;
	}

	oneT->changeDisp(-distance * weightPercentageOne); //one moves negative direction
	twoT->changeDisp(distance * (1.0f - weightPercentageOne)); //two moves opposite percentage of one
}

void PhysEngine::collisionResponse(PhysEvent *e)
{
	//setup
	std::string name = "";
	bool collide = true;
	GameObject *objOne = nullptr;
	GameObject *objTwo = nullptr;

	//finds the dmgValue component
	objOne = e->hbOne->getGameObjectPtr();
	objTwo = e->hbTwo->getGameObjectPtr();

	name = "DoesDamage";
	if (objOne->hasProperty(name))
	{
		collide = this->dmgResponse(e);
	}
	if (objTwo->hasProperty(name))
	{
		collide = collide && this->dmgResponse(e, true);
	}

	if (!collide) //no collision early return
	{
		return;
	}

	name = "DeleteOnCollision";
	if (objOne->hasProperty(name))
	{
		this->voidResponse(e);
	}
	if (objTwo->hasProperty(name))
	{
		this->voidResponse(e, true);
	}

	name = "ParticleSpawnOnCollision";
	if (objOne->hasProperty(name))
	{
		this->spawnSparkParticleFx(e);
	}
	if (objTwo->hasProperty(name))
	{
		this->spawnSparkParticleFx(e, true);
	}

	name = "CollisionOn";
	if (objOne->hasProperty(name) && objTwo->hasProperty(name))
	{
		if (e->tOne == RECTANGLE && e->tTwo == RECTANGLE) //aabb
		{
			this->aabbResponse(e);
		}
	}

}

PhysEngine::~PhysEngine()
{
	this->m_hitboxList.clear(); //weak_ptrs don't matter
	this->playgroundTile = nullptr; //tiles will clean themselves up, not physics engine's responsibiility
}

/*
end main physics engine
*/