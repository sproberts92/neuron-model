#include "node.h"

node::node(int d, const std::vector<double> p, bool v, int nb)
	: dim(d), pos(p), value(v), n_branches(nb) {}

node::node(const node &n)
	: dim(n.dim), pos(n.pos), value(n.value), n_branches(n.n_branches) {}

node& node::operator= (const node &n)
{
	dim = n.dim;
	value = n.value;
	pos = n.pos;

	n_branches = n.n_branches;

	return *this;
}

node::~node() {}