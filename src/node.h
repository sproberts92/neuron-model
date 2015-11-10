#pragma once

#include <vector>
#include "rand_gen.h"

class Node
{
public:

	std::vector<double> pos;

	std::vector<Node*> next;

	Node(int d, const std::vector<double> p);

	bool get_value(void);
	void set_value(bool v);

	void clear_signal(void);
	void push_temp_next(void);
	bool pop_temp(double noise);

private:
	bool value;
	int dim;
	bool on;
	bool temp_value;
	static rand_gen<double> r_gen;
};
