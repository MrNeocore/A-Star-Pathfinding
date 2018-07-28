#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>

#define MAP_WIDTH 20
#define MAP_HEIGHT 20
#define NUM_BLOCKED 100
#define NUM_SLOW 100
#define DIAG 1

const float COST_SLOW_CELL = 5.0f;
const float COST_NORMAL_CELL = 1.0f;

enum node_type
{
	EMPTY,
	WALL,
	PATH
};

class Node {
public:
	int x;
	int y;
	float cost;
	node_type type;
	int g = 100000; //  std::numeric_limits<int>::infinity(); -> Not working
	int h;
	Node* cameFrom = NULL;
	const int f() const { return g + h; }

	Node::Node(int set_x, int set_y, node_type cost_type, float set_cost);
};

int ManhattanDist(Node &n1, Node &n2);
int DiagDist(Node &n1, Node &n2);
int DistanceHeuristic(Node &n1, Node &n2);
bool NodeIn(Node *node, std::vector<Node>& nodes);
void ReconstructPath(Node *goal);
void ShowMap(std::vector<Node*> &cells);