#include <Ctrl 2kPCH.h>

//helper function declaration
template <typename T>
bool inVector(T in, std::vector<T>& theList)
{
	int i = theList.size();
	for (int index = 0; index < i; index++)
	{
		if (in == theList[index])
		{
			return true;
		}
	}
	return false;
}

/*

game object

*/

GameObject::GameObject()
{
	//cstr
	this->initComponents(); //intializes vital components engine requires
	this->m_renderQueue.push_back("");
}

void GameObject::addComponent(std::string &name, Component *component)
{
	component->setGameObject(this);
	if (component->getType() == VISUAL_DATA) //this is temp and for debug!!
	{
		this->m_renderQueue[0] = name;
	}
	this->m_components.insert(std::pair<std::string, Component*>(name, component));
}

void GameObject::initComponents()
{
	std::string temp = ""; //temporary var for names

	//transformation
	temp = "Position";
	this->addComponent(temp, new Transformation());

	//last transformation
	temp = "Last Position";
	this->addComponent(temp, new Transformation());

	//main renderer
	temp = "Main Renderer";
	this->addComponent(temp, new Renderer());
}

void GameObject::removeFromCam()
{
	if (this->renderCam != nullptr) //camera guard
	{
		this->renderCam->setTarget(nullptr);
	}
}

void GameObject::removeFromHud()
{
	if (this->displayingHud != nullptr) //hud guard
	{
		this->displayingHud->setTarget(nullptr);
	}
}

void GameObject::linkParent(GameObject *obj)
{
	this->m_parent = obj;
}

void GameObject::linkGame(Game *g)
{
	this->game = g;
}

void GameObject::setLinkCam(Camera *c)
{
	this->renderCam = c;
}

bool GameObject::isCamLinked()
{
	if (this->renderCam != nullptr) //linked
	{
		return true;
	}
	return false;
}

void GameObject::setLinkHud(GUI *h)
{
	this->displayingHud = h;
}

bool GameObject::isHudLinked()
{
	if (this->displayingHud != nullptr) //linked
	{
		return true;
	}
	return false;
}

void GameObject::addChild(GameObject *obj)
{
	obj->linkParent(this);
	obj->linkGame(this->game);
	this->m_children.push_back(obj);
}

void GameObject::clearChildrenPtr(int lvl) //make sure to not forget to use the lvl!!
{
	if (lvl > 0)
	{
		for (int i = 0; i < this->m_children.size(); i++)
		{
			this->m_children[i]->clearChildrenPtr(lvl - 1); //recursive call to clear
			this->m_children[i] = nullptr; //sets the pointers to nullptr
		}
		this->m_children.clear();
	}
}

void GameObject::update() 
{
	//pseudo-code
	for (int i = 0; i < this->m_children.size(); i++)
	{
		if (this->m_children[i]->getDelete()) //goes through the queue and deletes objects that needs deleting
		{
			delete this->m_children[i];
			this->m_children[i] = nullptr;
			this->m_children.erase(this->m_children.begin() + i);
		}
		else
		{
			this->m_children[i]->update(); //updates if does not need deleting
		}
	}

	this->m_inputs.clear(); //clears all previous inputs
}

void GameObject::render() 
{
	if (this->game != nullptr) //nullptr guard, does the actual rendering
	{
		RenderEngine &rEngine = this->game->getEngine().getRenderEngine(); //reference to render engine being used
		Component* c = this->m_components.at("Main Renderer");
		rEngine.render(c); //renders
	}

	//debug will move to update later
	if (this->m_renderQueue.size() > 0)
	{
		try
		{
			this->m_components.at(this->m_renderQueue[0])->getTimerRef() += this->game->getEngine().getTime(); //only render first thing in queue
		}
		catch (std::out_of_range)
		{
			//do nothing
		}
	}

	for (int i = 0; i < this->m_children.size(); i++)
	{
		this->m_children[i]->render(); //render calls on all children
	}
}

void GameObject::inputCommand(std::string &str)
{
	this->m_inputs.push_back(str);
}

void GameObject::inputCommand(std::vector<std::string>* in)
{
	for (int i = 0; i < in->size(); i++)
	{
		this->m_inputs.push_back(in->at(i));
	}
}

void GameObject::deleteGO()
{
	this->m_delete = true;
}

void GameObject::addProperty(std::string &str)
{
	if (!inVector(str, this->m_properties))
	{
		this->m_properties.push_back(str);
	}
}

bool GameObject::hasProperty(std::string &pro)
{
	return inVector(pro, this->m_properties);
}

void GameObject::clearAllProperties()
{
	this->m_properties.clear();
}

//virtuals
bool GameObject::doDamage(GameObject *target)
{
	//does nothing
	return false;
}

//accessor
std::string GameObject::getName() //default getName
{
	return "";
}

bool GameObject::getDelete()
{
	return this->m_delete;
}

Component* GameObject::getComponent(std::string &name)
{
	Component *returnValue = nullptr;
	try
	{
		returnValue = this->m_components.at(name);
	}
	catch (std::out_of_range)
	{
		return nullptr;
	}

	return returnValue;
}

int GameObject::checkForComponent(std::string &name)
{
	return this->m_components.count(name);
}

std::vector<std::string>* GameObject::getRenderQueue()
{
	return &(this->m_renderQueue);
}

void GameObject::getAllComponentByType(std::vector<Component*> &output, ComponentType type)
{
	for (auto it = this->m_components.begin(); it != this->m_components.end(); it++) //iteration
	{
		if (it->second->getType() == type) //check for type match
		{
			output.push_back(it->second);
		}
	}
}

void GameObject::getAllComponentByTypeB(std::vector<Component*> &output, ComponentType type)
{
	this->getAllComponentByType(output, type); //returns its own types

	for (int i = 0; i < this->m_children.size(); i++) //returns the child types
	{
		this->m_children[i]->getAllComponentByTypeB(output, type);
	}
}

GameObject* GameObject::getChild(std::string &name)
{
	for (int i = 0; i < this->m_children.size(); i++) //iterates through children
	{
		if (this->m_children[i]->getName() == name) //finds and returns a match for name
		{
			return this->m_children[i];
		}
	}

	return nullptr;
}

GameObject::~GameObject()
{
	//clean up
	for (auto it = this->m_components.begin(); it != this->m_components.end(); it++)
	{
		if (it->second != nullptr)
		{
			delete it->second;
			it->second = nullptr;
		}
	}
	this->m_components.clear();

	for (int i = 0; i < this->m_children.size(); i++)
	{
		if (this->m_children[i] != nullptr)
		{
			delete this->m_children[i];
			this->m_children[i] = nullptr;
		}
	}
	this->m_children.clear();

	this->removeFromCam();
	this->renderCam = nullptr;
	this->removeFromHud();
	this->displayingHud = nullptr;
}

/*

end game object

*/