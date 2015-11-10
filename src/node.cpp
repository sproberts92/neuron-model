#include "node.h"

Node::Node(int d, const std::vector<double> p)
	: dim(d), pos(p), value(0), on(1), temp_value(0) {}
Node::Node(const Node &n)
	: dim(n.dim), pos(n.pos), value(n.value), on(n.on), temp_value(0) {}

// Initialise static random generator object
rand_gen<double> Node::r_gen = rand_gen<double>(0.0, 1.0);

Node& Node::operator= (const Node &n)
{
	dim = n.dim;
	value = n.value;
	on = n.on;
	pos = n.pos;

	// Temporary and shouldn't be copied
	temp_value = 0;

	return *this;
}

Node::~Node() {}

void Node::clear_signal(void)
{
	value = 0;
	temp_value = 0;
}

void Node::push_temp_next(void)
{
	for(auto it_next : next)
		it_next->temp_value = it_next->temp_value | value;

	value = 0;
}

bool Node::pop_temp(double thresh)
{
	bool success = false;
	if(on && (thresh == 0.0 || (thresh != 1.0 && r_gen.get_rnum() > thresh)))
	{
		value = temp_value;
		success = true;
	}

	temp_value = 0;

	return success;
}
