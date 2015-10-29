#pragma once

#include <vector>

class Node
{
public:
	int dim;
	bool value;
	std::vector<double> pos;

	std::vector<Node*> next;

	Node(int d, const std::vector<double> p, bool v = 0);
	Node(const Node &n);
	Node &operator= (const Node &n);
	~Node();
};