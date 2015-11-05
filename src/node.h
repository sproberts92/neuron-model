#pragma once

#include <vector>

class Node
{
public:
	bool value;
	bool on;
	bool temp_value;
	int num_incoming;

	std::vector<double> pos;

	std::vector<Node*> next;

	Node(int d, const std::vector<double> p);
	Node(const Node &n);
	Node &operator= (const Node &n);
	~Node();

	void init_for_prop(void);
	void push_temp_next(void);

private:
	int dim;	
};