#include "node.h"

Node::Node(int d, const std::vector<double> p)
	: dim(d), pos(p), value(0), on(1), temp_value(0) {}

rand_gen<double> Node::r_gen = rand_gen<double>(0.0, 1.0);

bool Node::get_value(void) { return value; }
void Node::set_value(const bool v) { value = v; }

std::vector<double> Node::get_pos(void) { return pos; }

std::vector<Node*> Node::get_next(void) { return next; }
void Node::push_next(Node &n) { next.push_back(&n); }

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
