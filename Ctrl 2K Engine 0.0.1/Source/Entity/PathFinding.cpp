#include <Ctrl 2kPCH.h>

/*

GraphNode

*/

GraphNode::GraphNode(glm::vec2 p) :
	m_pos(p)
{
	//cstr
}

void GraphNode::reset()
{
	this->open = true;
	this->f_cost = 0;
	this->h_cost = 0;
	this->g_cost = 0;
	this->m_lastNode = nullptr;
}

void GraphNode::updateCosts(GraphNode* lastNode, GraphNode *endNode)
{
	glm::ivec2 gDiff = glm::abs(this->m_pos - lastNode->m_pos);
	int moveCost = glm::abs(gDiff.x - gDiff.y) * 10; //estimate g_cost
	if (gDiff.x > gDiff.y)
	{
		moveCost += gDiff.y * 14;
	}
	else
	{
		moveCost += gDiff.x * 14;
	}
	this->updateCosts(lastNode, moveCost, endNode);
}

void GraphNode::updateCosts(GraphNode* lastNode, int moveCost, int hCost)
{
	this->g_cost = lastNode->g_cost + moveCost;
	this->h_cost = hCost;
	this->f_cost = this->g_cost + this->h_cost;

	this->m_lastNode = lastNode;
}

void GraphNode::updateCosts(GraphNode* lastNode, int moveCost, GraphNode *endNode)
{
	glm::ivec2 diff = glm::abs(endNode->m_pos - this->m_pos);
	this->h_cost = glm::abs(diff.x - diff.y) * 10; //estimates h_cost
	if (diff.x > diff.y)
	{
		this->h_cost += diff.y * 14;
	}
	else
	{
		this->h_cost += diff.x * 14;
	}

	this->updateCosts(lastNode, moveCost, this->h_cost);
}

void GraphNode::addNeighbour(GraphNode *node)
{
	this->m_neighbours.push_back(node);
}

void GraphNode::addLastNode(std::vector<GraphNode*> &path)
{
	if (this->m_lastNode != nullptr)
	{
		path.push_back(this->m_lastNode);
		this->m_lastNode->addLastNode(path);
	}
}

void GraphNode::pathfind(GraphNode* endNode, std::vector<GraphNode*> &path, std::vector<GraphNode*> &openNodes, std::vector<GraphNode*> &closedNodes)
{
	if (this == endNode) //finishes the pathFind
	{
		path.push_back(this);
		this->addLastNode(path);
		return;
	}

	if (!this->open) //closedNode guard
	{
		return;
	}

	//closes node and calcs openNode costs around
	this->open = false;
	closedNodes.push_back(this);
	for (int i = 0; i < this->m_neighbours.size(); i++)
	{
		if (this->m_neighbours[i]->open)
		{
			this->m_neighbours[i]->updateCosts(this, endNode);
			openNodes.push_back(this->m_neighbours[i]);
		}
	}

	for (int i = 0; i < openNodes.size(); i++) //clears nodes that have been closed
	{
		if (!openNodes[i]->open)
		{
			openNodes.erase(openNodes.begin() + i);
		}
	}

	//finds the next node to go to
	int lowestFCost = std::numeric_limits<int>::max();
	int lowestFCostIndex = 0;
	for (int i = 0; i < openNodes.size(); i++)
	{
		if (openNodes[i]->f_cost < lowestFCost)
		{
			lowestFCost = openNodes[i]->f_cost;
			lowestFCostIndex = i;
		}
		else if (openNodes[i]->f_cost == lowestFCost)
		{
			if (openNodes[i]->h_cost < openNodes[lowestFCostIndex]->h_cost)
			{
				lowestFCostIndex = i;
			}
		}
	}

	openNodes[lowestFCostIndex]->pathfind(endNode, path, openNodes, closedNodes); //recursive call
}

GraphNode::~GraphNode()
{
	//dstr
	for (int i = 0; i < this->m_neighbours.size(); i++)
	{
		this->m_neighbours[i] = nullptr;
	}
	this->m_neighbours.clear();
	this->m_lastNode = nullptr;
}

/*

end GraphNode

*/

/*

Pathfinding Module

*/

ASharpPathFinder::ASharpPathFinder(std::vector<GraphNode> nodes) :
	activeNodes(nodes) //should make a new copy
{
	//cstr
}

void ASharpPathFinder::reset() //resets all buffers
{
	if (this->m_done)
	{
		this->openNodes.clear();
		this->closeNodes.clear();
		this->path.clear();
		this->m_done = false;
		this->m_open = true;
	}
}

void ASharpPathFinder::requestPathfind(int startNodeIndex, int endNodeIndex)
{
	if (this->m_open)
	{
		this->m_open = false;
		this->m_done = false;

		this->activeNodes[startNodeIndex].pathfind(&(this->activeNodes[endNodeIndex]), this->path, this->openNodes, this->closeNodes);

		this->m_done = true;
	}
}

void ASharpPathFinder::fetchResults(std::vector<glm::vec2> &results)
{
	if (this->m_done)
	{
		for (int i = this->path.size() - 1; i >= 0; i--)
		{
			results.push_back(this->path[i]->m_pos);
		}

		this->reset();
	}
}

ASharpPathFinder::~ASharpPathFinder()
{
	//dstr
	this->m_open = false;
	this->m_done = false;
	this->openNodes.clear();
	this->closeNodes.clear();
	this->activeNodes.clear();
}

/*

end Pathfinding Module

*/