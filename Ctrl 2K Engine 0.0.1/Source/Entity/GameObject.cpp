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
	this->m_activeComponents = std::vector<std::vector<Component*>>(8);
	this->initComponents(); //intializes vital components engine requires
}

int GameObject::addComponent(Component *component, bool active)
{
	component->setGameObject(this);
	this->m_components.push_back(component);
	if (active) //this is temp and for debug!!
	{
		this->m_activeComponents[component->getType()].push_back(component);
	}

	return this->m_components.size() - 1;
}

void GameObject::initComponents()
{
	std::string temp = ""; //temporary var for names

	//transformation 0
	this->addComponent(new Transformation(), true);

	//last transformation 1
	this->addComponent(new Transformation(), true);

	//main renderer 2
	this->addComponent(new Renderer(), true);
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
		Component* c = this->m_components[2];
		rEngine.render(c); //renders
	}

	//debug will move to update later
	if (this->m_activeComponents[VISUAL_DATA].size() > 0)
	{
		try
		{
			this->m_activeComponents[VISUAL_DATA][0]->getTimerRef() += this->game->getEngine().getTime(); //only render first thing in queue
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

Component* GameObject::getComponent(int index)
{
	Component *returnValue = nullptr;
	try
	{
		returnValue = this->m_components[index];
	}
	catch (std::out_of_range)
	{
		return nullptr;
	}

	return returnValue;
}

int GameObject::checkForComponent(int index)
{
	return index < this->m_components.size() && index >= 0;
}

std::vector<Component*>& GameObject::getAllActiveComponents(ComponentType type) {
	return this->m_activeComponents[type];
}

void GameObject::getAllActiveComponentsC(std::vector<Component*> &output, ComponentType type) {
	std::vector<Component*>& activeComponents = this->getAllActiveComponents(type);

	for (auto it = activeComponents.begin(); it != activeComponents.end(); it++) { //iteration
		output.push_back(*it);
	}
	for (auto it = this->m_children.begin(); it != this->m_children.end(); it++) { //returns the child types
		(*it)->getAllActiveComponentsC(output, type);
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
	this->m_activeComponents.clear();
	for (auto it = this->m_components.begin(); it != this->m_components.end(); it++) { //clears all components
		delete (*it);
		*it = nullptr;
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