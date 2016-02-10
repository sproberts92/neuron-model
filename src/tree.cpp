#include "tree.hpp"

Tree::Tree(std::valarray<std::pair<double, double>> b, std::vector<Node*> &a)
	: bounds(b), all(&a)
{
	root = new Neuron(r_vec(bounds), 2);
	all->push_back(root);

	grow_dir = r_vec(unit_box(bounds.size()));
	normalise(grow_dir);
}

Neuron *Tree::get_root(void)
{
	return root;
}

std::valarray<double> Tree::get_grow_dir(void)
{
	return grow_dir;
}

double Tree::find_shortest(const Tree &target, Node **shortest_ptr)
{
	Node *list_ptr = target.root;
	double shortest_r = std::numeric_limits<double>::infinity();

	while(!list_ptr->get_next().empty())
	{
		auto temp = list_ptr->get_pos() - root->get_pos();
		double r = (temp * temp).sum();

		/* Compare r^2 values, don't need to waste time with sqrt */
		if(r < shortest_r)
		{
			shortest_r = r;
			*shortest_ptr = list_ptr;
		}

		list_ptr = list_ptr->get_next().front();
	}

	/* CHECK UP ON BOUNDARY CONDITIONS*/
	return sqrt(shortest_r);
}

std::valarray<double> Tree::r_vec(std::valarray<std::pair<double, double>> box)
{
	std::valarray<double> vec(box.size());
	static rand_gen<double> r_gen(0, 1);

	for (int i = 0; i < vec.size(); ++i)
		vec[i] = ((box[i].second - box[i].first) * r_gen.get_rnum() + box[i].first);

	return vec;
}

std::valarray<std::pair<double, double>> Tree::unit_box(size_t d)
{
	std::valarray<std::pair<double, double>> box(d);
	box = std::make_pair(-1, 1);

	return box;
}

void Tree::normalise(std::valarray<double> &v)
{
	v /= sqrt((v * v).sum());
}

Node *Tree::add_node(Node *add_at, std::valarray<double> rel_pos)
{
	auto new_pos = rel_pos + add_at->get_pos();

	impose_bc(new_pos);

	Node *new_axon = new Node(new_pos);
	add_at->push_next(*new_axon);
	all->push_back(new_axon);

	return new_axon;
}

void Tree::grow_axon(double l)
{
	Node *list_ptr = root;

	while(!list_ptr->get_next().empty())
		list_ptr = list_ptr->get_next().front();

	add_node(list_ptr, l * grow_dir);
}

void Tree::impose_bc(std::valarray<double> &p)
{
	for (int i = 0; i < p.size(); i++)
	{
		// Impose periodic boundary conditions
		if(p[i] < bounds[i].first) p[i] += (bounds[i].second - bounds[i].first);
		if(p[i] >= bounds[i].second) p[i] -= (bounds[i].second - bounds[i].first);
	}
}

Node *Tree::grow_branch(Tree &target, double l, double c)
{
	Node *shortest = nullptr;
	double r = find_shortest(target, &shortest);

	static rand_gen<double> r_gen = rand_gen<double>(0, 1);
	/* TO DO: replace static with smart-pointer that can be gc'd later*/
	/* TO DO: read constants in gaussian from config.cfg */

	if(r != 0 && c != 0 && r_gen.get_rnum() < gaussian(r, c))
	{
		auto vec_r = l * (root->get_pos() - shortest->get_pos()) / r;
		Node *synapse = add_node(shortest, vec_r);

		Node *dendrite_head = synapse;
		for (int i = 0; i < (int)(r / l - 1); ++i)
			dendrite_head = add_node(dendrite_head, vec_r);

		dendrite_head->push_next(*root);

		return synapse;
	}
	else return nullptr;
}

double Tree::gaussian(double x, double c)
{
	return exp(-(x * x)/(2 * c * c));
}
