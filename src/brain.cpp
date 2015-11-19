#include "brain.h"

void Brain::create_network(user_config_t &config)
{
	auto bounds = convert_bounds(config);
	place_neurons(config.n_neurons, bounds);

	std::cout << "Neuron layout complete. Growing axons..." << std::endl;

	for (int i = 0; i < config.growth_iter; i++)
	{
		std::cout << 100 * i / config.growth_iter << "%\r";

		grow_axons(config.schwann_l);	
		print_network(file_name(config.growth, i), 1);
	}
	std::cout << "100%\n" << std::endl;

	std::cout << "Axon growth complete. Growing dendrites..." << std::endl;
	connect_network(config.schwann_l);
}

void Brain::find_loops(void)
{
	std::vector<Node*> path;
	depth_first_path_search(*neurons.front().get_root(), *neurons.front().get_root(), path);
}

void Brain::clear_signals(void)
{
	/* Phase 0 - reset all counters */
	for(auto it_n : all_nodes)
		it_n->clear_signal();
}

void Brain::insert_signal(int neuron_index)
{
	neurons[neuron_index].get_root()->set_value(1);
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

void Brain::print_network(std::ostringstream &fileName, bool no_signal)
{
	/* If no_signal == 1 then nodes which have no signal (value = 0) are
	 * printed also */
	std::ofstream out_stream;
	out_stream.open(fileName.str(), std::fstream::trunc);

	for(auto it_n : all_nodes)
		if(no_signal || it_n->get_value())
		{
			for(auto it_d : it_n->get_pos())
				out_stream << it_d << " ";
			out_stream << std::endl;
		}

	out_stream.close();
}

int Brain::network_size(void)
{
	return static_cast<int>(all_nodes.size());
}

void Brain::place_neurons(int n, std::valarray<std::pair<double, double>> bounds)
{
	for (int i = 0; i < n; i++)
		neurons.push_back(Tree::Tree(bounds, all_nodes));
}

void Brain::grow_axons(double l)
{
	for(auto i : neurons)
		i.grow_axon(l);
}


void Brain::connect_network(double l)
{
	// rand_gen<double> r_gen = rand_gen<double>(0, 1);

	int p = 0;
	for(auto i : neurons)
	{
		std::cout << 100 * p++ / neurons.size() << "%\r";
		
		for(auto j : neurons)
			if(i.get_root() == j.get_root()) continue;
			else if(1)// && r_gen.get_rnum() < gaussian(r, 60))
				i.grow_branch(j, l);
	}	

	std::cout << "100%\n" << std::endl;
}

void Brain::depth_first_path_search(Node &node, Node &root, std::vector<Node*> path)
{
	path.push_back(&node);

	for(auto it_n : node.get_next())
	{
		if(it_n == &root){
			std::cout << "Loop found, length: " << path.size() - 1 << std::endl;
			return;
		}
		else if(!(std::find(path.begin(), path.end(), it_n) != path.end()))
			depth_first_path_search(*it_n, root, path);
	}

	path.pop_back();

	/* Possible optimisation: only add nodes that have branches multiple paths incoming
	 * to the path. This would require an extra varaible storing the number of incoming
	 * paths and for this to be calculated by filling the whole network with 1s and 
	 * running one propagation step */
}

double Brain::gaussian(double x, double c)
{
	return exp(-(x * x)/(2 * c * c));
}
