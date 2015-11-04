#pragma once

#include <vector>

class Node
{
public:
	bool value;
	bool temp_value;
	int num_incoming;

	std::vector<double> pos;

	std::vector<Node*> next;

	Node(int d, const std::vector<double> p);
	Node(const Node &n);
	Node &operator= (const Node &n);
	~Node();

private:
	int dim;	
};