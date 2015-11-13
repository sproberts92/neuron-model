#include "brain.h"

Brain::Brain(int d, std::vector<double> b, int n, double l) : dim(d), bounds(b), n_neurons(n), schwann_l(l)
{
	neurons = new Neuron[n_neurons];

	r_gen = rand_gen<double>(0, 1);
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
		std::cout << 100 * i / n_neurons << "%\r";

		for (int j = 0; j < n_neurons; j++)
		{
			if(i == j) continue;
					
			Node *shortest = neurons[i].find_shortest(neurons[j]);

			double r = 0.0;
			std::vector<double> vec_r;
			for (int k = 0; k < dim; k++)
			{
				vec_r.push_back(-shortest->get_pos()[k] + neurons[i].base_soma->get_pos()[k]);
				r += vec_r[k] * vec_r[k];
			}
		

			r = sqrt(abs(r));
		
			if(r != 0)// && r_gen.get_rnum() < gaussian(r, 60))
			{
				for (int k = 0; k < dim; k++)
					vec_r[k] /= r;			
				
				Node *synapse = branch_axon(shortest, vec_r);
				all_synapses.push_back(synapse);
				all_synapses_state.push_back(1);
				synapses.insert(std::map<Node*, int>::value_type(synapse, 0));

				Node *dendrite_end;
				for (int i = 0; i < (int)(r/schwann_l + 1); i++)
					dendrite_end = grow_axon(synapse, vec_r);
			
				dendrite_end->push_next(neurons[i].base_soma);
			}
		}
	}
	std::cout << "100%\n" << std::endl;
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
		while(!list_ptr->get_next().empty())
			list_ptr = list_ptr->get_next()[0];

	std::vector<double> new_pos = g_dir;
	for (int i = 0; i < new_pos.size(); i++){
		new_pos[i] = g_dir[i] * schwann_l + list_ptr->get_pos()[i];

		// Impose periodic boundary conditions
		if(new_pos[i] < bounds[i * 2]) new_pos[i] += (bounds[i * 2 + 1] - bounds[i * 2]);
		if(new_pos[i] >= bounds[i * 2 + 1]) new_pos[i] -= (bounds[i * 2 + 1] - bounds[i * 2]);
	}

	Node *new_axon = new Node(dim, new_pos);
	all_nodes.push_back(new_axon);
	list_ptr->push_next(new_axon);

	return new_axon;
}

Node *Brain::branch_axon(Node *base, std::vector<double> g_dir)
{
	Node *list_ptr;
	list_ptr = base;

	std::vector<double> new_pos = g_dir;
	for (int i = 0; i < new_pos.size(); i++)
	{
		new_pos[i] = g_dir[i] * schwann_l + list_ptr->get_pos()[i];

		// Impose periodic boundary conditions
		if(new_pos[i] < bounds[i * 2]) new_pos[i] += (bounds[i * 2 + 1] - bounds[i * 2]);
		if(new_pos[i] >= bounds[i * 2 + 1]) new_pos[i] -= (bounds[i * 2 + 1] - bounds[i * 2]);
	}

	Node *new_axon = new Node(dim, new_pos);
	all_nodes.push_back(new_axon);
	list_ptr->push_next(new_axon);

	return new_axon;
}

void Brain::print_network(std::ostringstream &fileName, bool no_signal)
{
	/* If no_signal == 1 then nodes which have no signal (value = 0) are
	 * printed also */

	std::ofstream out_stream;
	out_stream.open(fileName.str(), std::fstream::trunc);
	
	for(auto it_n : all_nodes)
	{
		if(no_signal || it_n->get_value())
		{
			for(auto it_d : it_n->get_pos())
					out_stream << it_d << " ";
			out_stream << std::endl;
		}
	}

	out_stream.close();
}

void Brain::propagate_signal(double thresh)
{
	/* Phase 1 - move to temp variable of next node(s) */
	for(auto it_n : all_nodes)
		it_n->push_temp_next();

	/* Phase 2 - move from temp variable to value variable */
	for(auto it_n : all_nodes)
		it_n->pop_temp(thresh);
}

void Brain::clear_signals(void)
{
	/* Phase 0 - reset all counters */
	for(auto it_n : all_nodes)
		it_n->clear_signal();
}

void Brain::depth_first_path_search(Node *node, Node *root, std::vector<Node*> path)
{
	path.push_back(node);

	for(auto it_n : node->get_next())
	{
		if(it_n == root){
			std::cout << "Loop found, length: " << path.size() - 1 << std::endl;
			return;
		}
		else if(!(std::find(path.begin(), path.end(), it_n) != path.end()))
			depth_first_path_search(it_n, root, path);
	}

	path.pop_back();

	/* Possible optimisation: only add nodes that have branches multiple paths incoming
	 * to the path. This would require an extra varaible storing the number of incoming
	 * paths and for this to be calculated by filling the whole network with 1s and 
	 * running one propagation step */
}
