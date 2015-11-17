#pragma once

#include <vector>
#include <valarray>
#include "rand_gen.h"

class Node
{
public:
	Node();
	Node(int d, const std::valarray<double> p);

	bool get_value(void);
	void set_value(const bool v);

	std::valarray<double> get_pos(void);
	std::vector<Node*> get_next(void);
	void push_next(Node &n);

	void clear_signal(void);
	void push_temp_next(void);
	bool pop_temp(double noise);

private:
	const int dim;
	
	bool value;
	bool on;
	bool buffer;
	
	const std::valarray<double> pos;
	std::vector<Node*> next;
};
