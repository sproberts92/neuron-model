#include "node.h"

Node::Node(int d, const std::vector<double> p)
	: dim(d), pos(p), value(0), on(1), temp_value(0), num_incoming(0) {}

Node::Node(const Node &n)
	: dim(n.dim), pos(n.pos), value(n.value), on(n.on), temp_value(0), num_incoming(0) {}

Node& Node::operator= (const Node &n)
{
	dim = n.dim;
	value = n.value;
	on = n.on;
	pos = n.pos;

	// These are temporary and shouldn't be copied
	temp_value = 0;
	num_incoming = 0;

	return *this;
}

Node::~Node() {}
