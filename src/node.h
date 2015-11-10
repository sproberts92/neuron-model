#pragma once

#include <vector>
#include "rand_gen.h"

class Node
{
public:
	bool value;
	bool on;

	std::vector<double> pos;

	std::vector<Node*> next;

	Node(int d, const std::vector<double> p);

	void clear_signal(void);
	void push_temp_next(void);
	bool pop_temp(double noise);

private:
	int dim;
	bool temp_value;
	static rand_gen<double> r_gen;
};
