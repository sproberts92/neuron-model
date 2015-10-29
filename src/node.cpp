#include "node.h"

Node::Node(int d, const std::vector<double> p, bool v)
	: dim(d), pos(p), value(v) {}

Node::Node(const Node &n)
	: dim(n.dim), pos(n.pos), value(n.value) {}

Node& Node::operator= (const Node &n)
{
	dim = n.dim;
	value = n.value;
	pos = n.pos;

	return *this;
}

Node::~Node() {}