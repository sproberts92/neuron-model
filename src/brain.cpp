#include "brain.h"

brain::brain(int d, std::vector<double> b, int n, double l) : dim(d), bounds(b), n_neurons(n), schwann_l(l)
{
	neurons = new neuron[n_neurons];
}

brain::~brain(void)
{
	delete[] neurons;
}

void brain::place_neurons(void)
{
	for (int i = 0; i < n_neurons; i++)
		neurons[i] = neuron(dim, bounds);
}

void brain::grow_axons(void)
{
	for (int i = 0; i < n_neurons; i++)
		grow_axon(neurons[i].base_soma, neurons[i].grow_dir);
}

void brain::connect_network(void)
{
	for (int i = 0; i < n_neurons; i++)
	{
		for (int j = 0; j < n_neurons; j++)
		{
			if(i == j) continue;
					
			node *shortest = neurons[i].find_shortest(neurons[j]);

			double r = 0.0;
			std::vector<double> vec_r;
			for (int k = 0; k < dim; k++)
			{
				vec_r.push_back(-shortest->pos[k] + neurons[i].base_soma->pos[k]);
				r += vec_r[k] * vec_r[k];
			}
		

			r = sqrt(abs(r));

			if(r != 0)
			{
				for (int k = 0; k < dim; k++)
					vec_r[k] /= r;			
				
				node *new_branch = branch_axon(shortest, vec_r);
				node *dendrite_end;

				for (int i = 0; i < (int)(r/schwann_l + 1); i++)
					dendrite_end = grow_axon(new_branch, vec_r);
			
				// dendrite_end->next.push_back(neurons[i].base_soma);
			}
		}
	}
}

node *brain::grow_axon(node *base, std::vector<double> g_dir)
{
	node *list_ptr;
	list_ptr = base;

	if(list_ptr != 0)
		while(!list_ptr->next.empty())
			list_ptr = list_ptr->next[0];

	std::vector<double> new_pos = g_dir;
	for (int i = 0; i < new_pos.size(); i++){
		new_pos[i] = g_dir[i] * schwann_l + list_ptr->pos[i];

		// Impose periodic boundary conditions
		if(new_pos[i] < bounds[i * 2]) new_pos[i] += (bounds[i * 2 + 1] - bounds[i * 2]);
		if(new_pos[i] >= bounds[i * 2 + 1]) new_pos[i] -= (bounds[i * 2 + 1] - bounds[i * 2]);
	}

	node *new_axon = new node(dim, new_pos);
	list_ptr->next.push_back(new_axon);

	return new_axon;
}

node *brain::branch_axon(node *base, std::vector<double> g_dir)
{
	node *list_ptr;
	list_ptr = base;

	std::vector<double> new_pos = g_dir;
	for (int i = 0; i < new_pos.size(); i++)
	{
		new_pos[i] = g_dir[i] * schwann_l + list_ptr->pos[i];

		// Impose periodic boundary conditions
		if(new_pos[i] < bounds[i * 2]) new_pos[i] += (bounds[i * 2 + 1] - bounds[i * 2]);
		if(new_pos[i] >= bounds[i * 2 + 1]) new_pos[i] -= (bounds[i * 2 + 1] - bounds[i * 2]);
	}

	node *new_axon = new node(dim, new_pos);
	list_ptr->next.push_back(new_axon);

	return new_axon;
}
