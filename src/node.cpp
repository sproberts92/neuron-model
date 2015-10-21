#include "node.h"

node::node(int d, const std::vector<double> p, bool v)
	: dim(d), pos(p), value(v) {}

node::node(const node &n)
	: dim(n.dim), pos(n.pos), value(n.value) {}

node& node::operator= (const node &n)
{
	dim = n.dim;
	value = n.value;
	pos = n.pos;

	return *this;
}

node::~node() {}