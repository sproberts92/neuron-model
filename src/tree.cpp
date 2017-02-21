#include "tree.hpp"

Tree::Tree(std::valarray<std::pair<double, double>> b,
	int cr, std::vector<Node*> &a, std::vector<Synapse*> &s)
	: bounds(b), all(&a), all_s(&s)
{
	root = new Neuron(r_vec(bounds), cr);
	all->push_back(root);

	// Generate and normalise a vector in a random direction along which the
	// axon will be grown.
	grow_dir = r_vec(unit_box(bounds.size()));
	normalise(grow_dir);
}

Neuron *Tree::get_root(void){ return root; }
std::valarray<double> Tree::get_grow_dir(void){ return grow_dir; }

double Tree::find_shortest(const Tree &target, Node **shortest_ptr,
	std::valarray<double> &vec_r, user_config_t &cf)
{
	Node *list_ptr = target.root;

	double shortest_r = std::numeric_limits<double>::infinity();

	// Move along the axon originating from target.root
	while(!list_ptr->get_next().empty())
	{
		// Because of the periodic boundary conditions,  if a particular target
		// node is more than box_l/2 away from the root of this tree we need to
		// wrap around and get distance as if it was wrapped around.   This  is
		// achieved by creating a temp node, shifting it into the adjacent cell
		// and then measuring the distance to that instead.
		auto temp = list_ptr->get_pos() - root->get_pos();

		for (int i = 0; i < size(temp); ++i)
		{
			auto box_l = cf.bounds[2*i + 1] - cf.bounds[2 * i];

			// Impose periodic boundary conditions. Isn't it beautiful?
			temp[i] -= ((temp[i] > box_l/2) - (temp[i] < -box_l/2)) * box_l;
		}

		double r = (temp * temp).sum();

		// Compare r^2 values, don't need to waste time with sqrt.
		if(r < shortest_r)
		{
			shortest_r = r;
			*shortest_ptr = list_ptr;
			vec_r = temp;
		}

		list_ptr = list_ptr->get_next().front();
	}

	return sqrt(shortest_r);
}

std::valarray<double> Tree::r_vec(std::valarray<std::pair<double, double>> box)
{
	std::valarray<double> vec(box.size());
	static rand_gen<double> r_gen(0, 1);

	// Generate a random number for each element of the vector then scale and
	// translate appropriately such that if lies in the box.
	for (int i = 0; i < vec.size(); ++i)
		vec[i] = (box[i].second - box[i].first)
			* (r_gen.get_rnum() + box[i].first);

	return vec;
}

std::valarray<std::pair<double, double>> Tree::unit_box(size_t d)
{
	// To do: replace this entirely with a compile-time template.
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
	// The new position is calculated relative to the position of add_at, if
	// it lies outside the system box then it needs to be wrapped to the other
	// side of the box.
	auto new_pos = rel_pos + add_at->get_pos();
	impose_bc(new_pos);

	Node *new_axon = new Node(new_pos);
	add_at->push_next(*new_axon);
	all->push_back(new_axon);

	return new_axon;
}

Synapse *Tree::add_synapse(Node *add_at, std::valarray<double> rel_pos, int t)
{
	// To do: The functionality here is so close to Tree::add_node, it should
	// be possible to combine them to reduce duplication.
	auto new_pos = rel_pos + add_at->get_pos();
	impose_bc(new_pos);

	Synapse *new_axon = new Synapse(new_pos, t);
	add_at->push_next(*new_axon);
	all->push_back(new_axon);
	all_s->push_back(new_axon);

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
		if(p[i] < bounds[i].first)  p[i] += (bounds[i].second - bounds[i].first);
		if(p[i] > bounds[i].second) p[i] -= (bounds[i].second - bounds[i].first);
	}
}

Node *Tree::grow_branch(Tree &target, user_config_t &cf)
{
	// Encapsulates the process of selecting the point to grow the branch
	// (dendrite) to, deciding whether a branch will actually be grown (with
	// Gaussian probability) and then growing the branch.

	Node *shortest = nullptr;

	std::valarray<double> vec_r;
	double r = find_shortest(target, &shortest, vec_r, cf);

	// Decide whether the branch will be created, short circuits to avoid
	// actually drawing the random variable and calculating the Gaussian if
	// it's not necessary.
	static rand_gen<double> r_gen = rand_gen<double>(0, 1);
	if(r != 0 && cf.link_fwhm_param != 0
		&& r_gen.get_rnum() < gaussian(r, cf.link_fwhm_param))
	{
		vec_r = cf.schwann_l * vec_r / r;
		Synapse *synapse = add_synapse(shortest, vec_r, cf.target_age);
		Node *dendrite_head = synapse;

		for (int i = 0; i < (int)(r / cf.schwann_l - 1); ++i)
			dendrite_head = add_node(dendrite_head, vec_r);

		dendrite_head->push_next(*root);

		return dendrite_head;
	}
	else return nullptr;
}

double Tree::gaussian(double x, double c)
{
	return exp(-(x * x)/(2 * c * c));
}
