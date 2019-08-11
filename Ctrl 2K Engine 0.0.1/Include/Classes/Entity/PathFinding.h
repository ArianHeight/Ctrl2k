#pragma once

/*

GraphNode

*/

//pre-declaration
class ASharpPathFinder;

class GraphNode
{
public:
	GraphNode(glm::vec2 p = glm::vec2(0.0f));
	~GraphNode();

	friend class ASharpPathFinder;

private:
	GraphNode *m_lastNode = nullptr; //no ownership
	std::vector<GraphNode*> m_neighbours; //no ownership
	glm::vec2 m_pos; //position in wp
	int g_cost; //distance cost
	int h_cost; //estimated distance from end node
	int f_cost; //g cost + h cost
	bool open = true; //whether the node is open or not

public:
	void reset(); //resets all values except for neighbours

	void addNeighbour(GraphNode *node);
	void addLastNode(std::vector<GraphNode*> &path);

	void updateCosts(GraphNode* lastNode, GraphNode *endNode);
	void updateCosts(GraphNode* lastNode, int moveCost, int hCost);
	void updateCosts(GraphNode* lastNode, int moveCost, GraphNode *endNode);

	void pathfind(GraphNode* endNode, std::vector<GraphNode*> &path, std::vector<GraphNode*> &openNodes, std::vector<GraphNode*> &closedNodes); //pathfinds from this node to endnode
};

/*

end GraphNode

*/

/*

Pathfinding Module
this is dynamic class, operates on thread destroy on run or reset
note that activeNodes will be copied instead of reffed

*/

class ASharpPathFinder
{
public:
	ASharpPathFinder(std::vector<GraphNode> nodes);
	~ASharpPathFinder();

private:
	std::vector<GraphNode> activeNodes; //all nav nodes active in current mesh.
	std::vector<GraphNode*> openNodes; //openNodes
	std::vector<GraphNode*> closeNodes; //closeNodes
	std::vector<GraphNode*> path; //output vector
	glm::vec2 request; //a queue for requests
	bool m_open{true};
	bool m_done{false};

public:
	void reset();
	void requestPathfind(int startNodeIndex, int endNodeIndex);
	void fetchResults(std::vector<glm::vec2> &results);
};

/*

end Pathfinding Module

*/