#include "neuron.h"

neuron::neuron() {}

neuron::neuron(int d, std::vector<double> box) : dim(d)
{
	std::vector<double> p_temp = r_vec(d, box);
	base_soma = new node(d ,p_temp);

	static std::vector<double> unit_box;
	if(unit_box.empty())
		for (int i = 0; i < dim; i++)
		{
			unit_box.push_back(-1.0);
			unit_box.push_back(1.0);
		}

	grow_dir = r_vec(d, unit_box);
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
	// static std::vector<rand_gen> r_gen;
	std::vector<rand_gen> r_gen;
	
	/* NEED A BETTER WAY TO HANDLE THIS
	 * CURRENTLY IT IS POINTLESS BECUASE THE STATIC VECTOR IS
	 * CLEARED EVERY LOOP. MAYBE PUT TWO SEPARATE r_gen VARIABLES
	 * OR MAYBE EVEN AN ARRAY OF THEM. IT STILL WORKS, JUST SLOW */
	// static std::vector<double> safe_box;
	// if(b_box != safe_box) r_gen.clear();

	if(r_gen.empty())
	{
		for (int i = 0; i < dim; i++)				
			r_gen.push_back(rand_gen(b_box[i * 2], b_box[(i * 2) + 1]));
		// safe_box = b_box;
	}

	for (int i = 0; i < dim; i++){
		pos[i] = r_gen[i].get_rnum();
	}

	return pos;
}

void neuron::grow_axon(double rate, std::vector<double> &box)
{
	node *list_ptr;
	list_ptr = this->base_soma;

	if(list_ptr != 0)
		while(!list_ptr->next.empty())
			list_ptr = list_ptr->next[0];

	std::vector<double> new_pos = grow_dir;
	for (int i = 0; i < new_pos.size(); i++){
		new_pos[i] = grow_dir[i] * rate + list_ptr->pos[i];
		if(new_pos[i] < box[i * 2]) new_pos[i] += (box[i * 2 + 1] - box[i * 2]);
		if(new_pos[i] >= box[i * 2 + 1]) new_pos[i] -= (box[i * 2 + 1] - box[i * 2]);
	}

	node *new_axon = new node(dim, new_pos);
	list_ptr->next.push_back(new_axon);
}

void neuron::print_axon(std::ofstream &out_stream)
{
	node *list_ptr;
	list_ptr = this->base_soma;

	if(list_ptr != 0)
		while(!list_ptr->next.empty())
		{
			for (int i = 0; i < dim; i++)
				out_stream << list_ptr->pos[i] << " ";
			out_stream << std::endl;
			list_ptr = list_ptr->next[0];
		}
}

void neuron::normalise_grow_dir(void)
{
	double length = 0.0;
	for (int i = 0; i < grow_dir.size(); i++)
		length += grow_dir[i] * grow_dir[i];

	for (int i = 0; i < grow_dir.size(); i++)
		grow_dir[i] /= sqrt(length);
}