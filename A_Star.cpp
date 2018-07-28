#include "A_star.h"

using namespace std;

bool operator==(const Node &n1, const Node &n2)
{
	return ((n1.x == n2.x) && (n1.y == n2.y));
}

int DistanceHeuristic(Node &n1, Node &n2)
{
	if (DIAG)
		return DiagDist(n1, n2);
	else
		return ManhattanDist(n1, n2);
}

int ManhattanDist(Node &n1, Node &n2)
{
	return abs(n1.x - n2.x) + abs(n1.y - n2.y);
}

int DiagDist(Node &n1, Node &n2)
{
	return max(abs(n1.x - n2.x), abs(n1.y - n2.y));
}

bool NodeIn(Node *node, vector<Node*>& nodes)
{
	bool found = false;

	for (size_t i = 0; i < nodes.size(); i++)
		if (node == nodes[i])
			found = true;

	return found;
}

void ReconstructPath(Node* goal)
{
	Node* curNode = goal;
	int count = 0;

	while (curNode->cameFrom != NULL)
	{
		count++;
		curNode->type = PATH;
		curNode = curNode->cameFrom;
	}
	cout << "Path lenght : " << count << endl;
}

bool compareNodes(Node const *n1, Node const *n2)
{
	return (n1->f() > n2->f());
}

void ShowMap(vector<Node*> &cells)
{
	for (size_t y = 0; y < MAP_HEIGHT; y++) {
		for (size_t x = 0; x < MAP_WIDTH; x++)
		{
			//vector<Node*>::iterator tmp = find_if(cells.begin(), cells.end(), [&x, &y](const Node* node) {return node->x == x && node->y == y; });
			for (size_t i = 0; i < cells.size(); i++)
			{
				if (cells[i]->x == x && cells[i]->y == y)
				{
					Node *node = cells[i];

					if (node->type == EMPTY)
					{
						if (node->cost == COST_NORMAL_CELL)
							cout << ". ";
						else if(node->cost == COST_SLOW_CELL)
							cout << "~ ";
					}
						
					else if (node->type == WALL)
						cout << "X ";

					else if (node->type == PATH)
					{
						if (node->cost == COST_NORMAL_CELL)
							cout << "| ";
						else if (node->cost == COST_SLOW_CELL)
							cout << "||";
					}
				}
			}
		}
		cout << endl;
	}
}

Node::Node(int set_x, int set_y, node_type set_type = EMPTY, float set_cost = 1.0f) : x(set_x), y(set_y), type(set_type), cost(set_cost) {}

vector<Node*> GetNeighbours(Node *node, vector<Node*> &nodes)
{
	vector<Node*> neighbours;
	
	for (Node* adj : nodes)
	{
		if (adj->type != WALL)
		{
			if ((node->x != MAP_WIDTH) && (adj->x == ((node->x)+1)) && (adj->y == node->y)) // Node to the right
				neighbours.push_back(adj);

			else if ((node->x != 0) && (adj->x == ((node->x)-1)) && (adj->y == node->y)) // Node to the left
				neighbours.push_back(adj);
		
			else if ((node->y != MAP_HEIGHT) && (adj->y == ((node->y)+1)) && (adj->x == node->x)) // Node above
				neighbours.push_back(adj);

			else if ((node->y != 0) && (adj->y == ((node->y)-1)) && (adj->x == node->x)) // Node below
				neighbours.push_back(adj);

			if (DIAG)
			{
				if((adj->x == ((node->x) + 1)) && (adj->y == node->y - 1)) // Top right
					if((node->x != MAP_WIDTH && node->y != 0)) // Not already at top right
						neighbours.push_back(adj);

				if ((adj->x == ((node->x) - 1)) && (adj->y == node->y - 1)) // Top left
					if ((node->x != 0 && node->y != 0)) // Not already at top left
						neighbours.push_back(adj);

				if ((adj->x == ((node->x) - 1)) && (adj->y == node->y + 1)) // Bottom left
					if ((node->x != 0 && node->y != MAP_HEIGHT)) // Not already at bottom left
						neighbours.push_back(adj);

				if ((adj->x == ((node->x) + 1)) && (adj->y == node->y + 1)) // Bottom right
					if ((node->x != MAP_HEIGHT && node->y != MAP_HEIGHT)) // Not already at bottom right
						neighbours.push_back(adj);
			}
 		}
	}

	return neighbours;
}

bool Astar(vector<Node*> &nodes, Node &start, Node &goal)
{
	vector<Node*> to_visit; // Would be great to use priority_queue, but can't search in it (out of the box)...
	vector<Node*> visited; // Could use unordered_set for faster exist checks

	start.g = 0;
	start.h = DistanceHeuristic(start, goal);
	to_visit.push_back(&start);
	start.type = PATH; 

	bool found_path = false;

	while (!to_visit.empty())
	{
		std::sort(to_visit.begin(), to_visit.end(), compareNodes);

		Node* visiting = to_visit.back();
		
		if (visiting == &goal)
			found_path = true;

		to_visit.pop_back(); // Effectively remove the current node from the queue
		visited.push_back(visiting);
		vector<Node*> neighbours = GetNeighbours(visiting, nodes);

		for (size_t i = 0; i < neighbours.size(); i++)
		{
			if (NodeIn(neighbours[i], visited))
				continue;

			if (!NodeIn(neighbours[i], to_visit))
				to_visit.push_back(neighbours[i]);

			int test_g = visiting->g + neighbours[i]->cost; // or more simply -> DistanceHeuristic(*visiting, *neighbours[i]); (basically : 1 -> Cost between 2 cells)
			
			if (test_g >= neighbours[i]->g)
				continue;

			else
			{
				neighbours[i]->cameFrom = visiting;
				neighbours[i]->g = test_g;
				neighbours[i]->h = DistanceHeuristic(*neighbours[i], goal);
			}
		}
	}

	return found_path;
}

int main()
{
	vector<Node*> nodes;
	Node* startNode = new Node(0, 0);
	Node* endNode = new Node(MAP_WIDTH - 1, MAP_HEIGHT - 1); // For consistency purposes... it's probably is possible to do with less pointers, but this wouldn't work without at first and I don't have time to try again.

	for (size_t x = 0; x < MAP_HEIGHT; x++)
	{
		for (size_t y = 0; y < MAP_WIDTH; y++)
		{
			if ((x != startNode->x || y != startNode->y) && (x != endNode->x || y != endNode->y))
				nodes.push_back(new Node(x, y, EMPTY));
		}
	}

	for (size_t i = 0; i < NUM_BLOCKED; i++)
	{
		int randomIdx = rand() % (MAP_WIDTH * MAP_HEIGHT -2);
		nodes[randomIdx]->type = WALL;
	}

	for (size_t i = 0; i < NUM_SLOW; i++)
	{
		int randomIdx;
		
		do
		{
			randomIdx = rand() % (MAP_WIDTH * MAP_HEIGHT - 2);
		} while (nodes[randomIdx]->type != EMPTY && nodes[randomIdx]->cost == COST_SLOW_CELL);

		nodes[randomIdx]->cost = COST_SLOW_CELL;
	}

	nodes.push_back(startNode);
	nodes.push_back(endNode);

	ShowMap(nodes);
	
	bool found = Astar(nodes, *startNode, *endNode); 

	if (found)
	{
		cout << "Path found !" << endl;
		ReconstructPath(endNode);
		ShowMap(nodes);
	}
	else
		cout << "ERROR : Path not found" << endl;

	for (size_t i = 0; i < nodes.size(); i++)
		delete nodes.at(i);

	nodes.clear();

	return 0;
}