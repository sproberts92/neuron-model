#include "neuron.h"

neuron::neuron() {}

neuron::neuron(int d, std::vector<double> box) : dim(d)
{
	std::vector<double> p_temp = r_vec(d, box);
	base_soma = new node(d ,p_temp);

	grow_dir = r_u_vec(d);
	normalise_grow_dir();
}

neuron::neuron(const neuron &n)
	: dim(n.dim), base_soma(n.base_soma), grow_dir(n.grow_dir) { }

neuron& neuron::operator= (const neuron &n)
{
	dim = n.dim;
	base_soma = n.base_soma;
	grow_dir = n.grow_dir;

	return *this;
}

neuron::~neuron() {}

std::vector<double> neuron::r_vec(int dim, std::vector<double> b_box)
{
	std::vector<double> pos(2 * dim);
	static std::vector<rand_gen> r_gen;

	if(r_gen.empty())
	{
		for (int i = 0; i < dim; i++)				
			r_gen.push_back(rand_gen(b_box[i * 2], b_box[(i * 2) + 1]));
	}

	for (int i = 0; i < dim; i++)
		pos[i] = r_gen[i].get_rnum();

	return pos;
}

std::vector<double> neuron::r_u_vec(int dim)
{
	std::vector<double> pos(2 * dim);
	static std::vector<rand_gen> r_gen;
	if(r_gen.empty())
		r_gen.push_back(rand_gen(-1, 1));	

	for (int i = 0; i < dim; i++)
			pos[i] = r_gen[0].get_rnum();

	return pos;
}

void neuron::normalise_grow_dir(void)
{
	double length = 0.0;
	for (int i = 0; i < grow_dir.size(); i++)
		length += grow_dir[i] * grow_dir[i];

	for (int i = 0; i < grow_dir.size(); i++)
		grow_dir[i] /= sqrt(length);
}

node *neuron::find_shortest(neuron &target)
{
	double shortest_r = std::numeric_limits<double>::infinity();
	node *shortest_ptr;

	node *list_ptr;
	list_ptr = target.base_soma;

	if(list_ptr != 0)
		while(!list_ptr->next.empty())
		{
			double r = 0.0;
			for (int i = 0; i < dim; i++)
				r += (list_ptr->pos[i] - this->base_soma->pos[i])*(list_ptr->pos[i] - this->base_soma->pos[i]);

			if(abs(r) < shortest_r)
			{
				shortest_r = abs(r);
				shortest_ptr = list_ptr;
			}
			list_ptr = list_ptr->next[0];
		}

	/* CHECK UP ON BOUNDARY CONDITIONS*/

	return shortest_ptr;
}