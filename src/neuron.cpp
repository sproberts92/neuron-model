#include "neuron.h"

Neuron::Neuron() {}

Neuron::Neuron(int d, std::vector<double> box, std::vector<Node*> *an) : dim(d)
{
	all_nodes = an;
	std::vector<double> p_temp = r_vec(d, box);

	base_soma = new Node(d ,p_temp);

	all_nodes->push_back(base_soma);

	grow_dir = r_u_vec(d);
	normalise_grow_dir();
}

Neuron::Neuron(const Neuron &n)
	: dim(n.dim), base_soma(n.base_soma), grow_dir(n.grow_dir) { }

Neuron& Neuron::operator= (const Neuron &n)
{
	dim = n.dim;
	base_soma = n.base_soma;
	grow_dir = n.grow_dir;

	return *this;
}

Neuron::~Neuron() {}

std::vector<double> Neuron::r_vec(int dim, std::vector<double> b_box)
{
	std::vector<double> pos(dim);
	static std::vector<rand_gen <double>> r_gen;

	if(r_gen.empty())
	{
		for (int i = 0; i < dim; i++)				
			r_gen.push_back(rand_gen<double>(b_box[i * 2], b_box[(i * 2) + 1]));
	}

	for (int i = 0; i < dim; i++)
		pos[i] = r_gen[i].get_rnum();

	return pos;
}

std::vector<double> Neuron::r_u_vec(int dim)
{
	std::vector<double> pos(dim);
	static std::vector<rand_gen <double>> r_gen;
	if(r_gen.empty())
		r_gen.push_back(rand_gen<double>(-1, 1));	

	for (int i = 0; i < dim; i++)
			pos[i] = r_gen[0].get_rnum();

	return pos;
}

void Neuron::normalise_grow_dir(void)
{
	double length = 0.0;
	for (int i = 0; i < grow_dir.size(); i++)
		length += grow_dir[i] * grow_dir[i];

	for (int i = 0; i < grow_dir.size(); i++)
		grow_dir[i] /= sqrt(length);
}

Node *Neuron::find_shortest(Neuron &target)
{
	double shortest_r = std::numeric_limits<double>::infinity();
	Node *shortest_ptr;

	Node *list_ptr;
	list_ptr = target.base_soma;

	// for (int i = 0; i < 5; i++)
		// if(!list_ptr->next.empty()) list_ptr = list_ptr->next[0];

	if(list_ptr != 0)
		while(!list_ptr->get_next().empty())
		{
			double r = 0.0;
			for (int i = 0; i < dim; i++)
				r += (list_ptr->get_pos()[i] - this->base_soma->get_pos()[i])*(list_ptr->get_pos()[i] - this->base_soma->get_pos()[i]);

			if(abs(r) < shortest_r)
			{
				shortest_r = abs(r);
				shortest_ptr = list_ptr;
			}
			list_ptr = list_ptr->get_next()[0];
		}

	/* CHECK UP ON BOUNDARY CONDITIONS*/

	return shortest_ptr;
}