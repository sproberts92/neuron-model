#pragma once

#include <vector>
#include "rand_gen.h"

class Node
{
public:
	Node();
	Node(int d, const std::vector<double> p);

	bool get_value(void);
	void set_value(const bool v);

	std::vector<double> get_pos(void);
	std::vector<Node*> get_next(void);
	void push_next(Node &n);

	void clear_signal(void);
	void push_temp_next(void);
	bool pop_temp(double noise);
	bool value;

private:
	const int dim;
	
	bool on;
	bool temp_value;
	
	static rand_gen<double> r_gen;
	const std::vector<double> pos;
	std::vector<Node*> next;
};
