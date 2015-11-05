#include "brain.h"

Brain::Brain(int d, std::vector<double> b, int n, double l) : dim(d), bounds(b), n_neurons(n), schwann_l(l)
{
	neurons = new Neuron[n_neurons];

	/* Set up random number generator */
	if(r_gen.empty())
		r_gen.push_back(rand_gen(0, 1));
}

Brain::~Brain(void)
{
	delete[] neurons;
}

void Brain::place_neurons(void)
{
	for (int i = 0; i < n_neurons; i++)
		neurons[i] = Neuron(dim, bounds, &all_nodes);
}

void Brain::grow_axons(void)
{
	for (int i = 0; i < n_neurons; i++)
		grow_axon(neurons[i].base_soma, neurons[i].grow_dir);
}

void Brain::connect_network(void)
{
	for (int i = 0; i < n_neurons; i++)
	{
		for (int j = 0; j < n_neurons; j++)
		{
			if(i == j) continue;
					
			Node *shortest = neurons[i].find_shortest(neurons[j]);

			double r = 0.0;
			std::vector<double> vec_r;
			for (int k = 0; k < dim; k++)
			{
				vec_r.push_back(-shortest->pos[k] + neurons[i].base_soma->pos[k]);
				r += vec_r[k] * vec_r[k];
			}
		

			r = sqrt(abs(r));
		
			if(r != 0 && r_gen[0].get_rnum() < gaussian(r, 80))
			{
				for (int k = 0; k < dim; k++)
					vec_r[k] /= r;			
				
				Node *new_branch = branch_axon(shortest, vec_r);
				Node *dendrite_end;

				for (int i = 0; i < (int)(r/schwann_l + 1); i++)
					dendrite_end = grow_axon(new_branch, vec_r);
			
				dendrite_end->next.push_back(neurons[i].base_soma);
			}
		}
	}
}

double Brain::gaussian(double x, double c)
{
	return exp(-(x*x)/(2*c*c));
}

Node *Brain::grow_axon(Node *base, std::vector<double> g_dir)
{
	Node *list_ptr;
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

	Node *new_axon = new Node(dim, new_pos);
	all_nodes.push_back(new_axon);
	list_ptr->next.push_back(new_axon);

	return new_axon;
}

Node *Brain::branch_axon(Node *base, std::vector<double> g_dir)
{
	Node *list_ptr;
	list_ptr = base;

	std::vector<double> new_pos = g_dir;
	for (int i = 0; i < new_pos.size(); i++)
	{
		new_pos[i] = g_dir[i] * schwann_l + list_ptr->pos[i];

		// Impose periodic boundary conditions
		if(new_pos[i] < bounds[i * 2]) new_pos[i] += (bounds[i * 2 + 1] - bounds[i * 2]);
		if(new_pos[i] >= bounds[i * 2 + 1]) new_pos[i] -= (bounds[i * 2 + 1] - bounds[i * 2]);
	}

	Node *new_axon = new Node(dim, new_pos);
	all_nodes.push_back(new_axon);
	list_ptr->next.push_back(new_axon);

	return new_axon;
}

void Brain::print_network(std::ostringstream &fileName, bool no_signal)
{
	/* If no_signal == 1 then nodes which have no signal (value = 0) are
	 * printed also */

	std::ofstream out_stream;
	out_stream.open(fileName.str(), std::fstream::trunc);
	
	for(auto it_n = all_nodes.begin(); it_n != all_nodes.end(); ++it_n)
	{
		if(no_signal || (*it_n)->value)
		{
			for(auto it_d = (*it_n)->pos.begin(); it_d != (*it_n)->pos.end(); ++it_d)
					out_stream << *it_d << " ";
			out_stream << std::endl;
		}
	}

	out_stream.close();
}

void Brain::propagate_signal(void)
{
	/* Phase 0 - reset all counters */
	for(auto it_n = all_nodes.begin(); it_n != all_nodes.end(); ++it_n)
		(*it_n)->init_for_prop();

	/* Phase 1 - move to temp variable of next node(s) */
	for(auto it_n = all_nodes.begin(); it_n != all_nodes.end(); ++it_n)
		(*it_n)->push_temp_next();

	/* Phase 2 - move from temp variable to value variable */
	for(auto it_n = all_nodes.begin(); it_n != all_nodes.end(); ++it_n)
	{
		if(r_gen[0].get_rnum() > 0.05)
			(*it_n)->value = (*it_n)->temp_value;
		
		(*it_n)->temp_value = 0;
	}
}
