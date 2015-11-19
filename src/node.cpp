#include "node.h"

Node::Node(const std::valarray<double> p)
	: pos(p), value(0), on(1), buffer(0) {}

bool Node::get_value(void) { return value; }
void Node::set_value(const bool v) { value = v; }

std::valarray<double> Node::get_pos(void) { return pos; }

std::vector<Node*> Node::get_next(void) { return next; }
void Node::push_next(Node &n) { next.push_back(&n); }

void Node::clear_signal(void)
{
	value = 0;
	buffer = 0;
}

void Node::push_temp_next(void)
{
	for(auto it_next : next)
		it_next->buffer = it_next->buffer | value;

	value = 0;
}

bool Node::pop_temp(double thresh)
{
	auto r_gen = rand_gen<double>(0.0, 1.0);

	bool success = false;
	if(on && (thresh == 0.0 || (thresh != 1.0 && r_gen.get_rnum() > thresh)))
	{
		value = buffer;
		success = true;
	}

	buffer = 0;

	return success;
}
