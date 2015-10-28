#pragma once

#include <vector>

class node
{
public:
	int dim;
	bool value;
	std::vector<double> pos;

	std::vector<node*> next;

	node(int d, const std::vector<double> p, bool v = 0);
	node(const node &n);
	node &operator= (const node &n);
	~node();
};