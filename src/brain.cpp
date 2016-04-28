#include "brain.hpp"

Brain::Brain(void)
{
	live_nodes = new std::deque<Node*>;
	live_nodes_s = new std::set<Node*>;
}

void Brain::create_network(user_config_t &config)
{
	auto bounds = convert_bounds(config);
	place_neurons(config.n_neurons, bounds);

	std::cout << "Neuron layout complete. Growing axons..." << std::endl;

	for (int i = 0; i < config.growth_iter; i++)
	{
		std::cout << 100 * i / config.growth_iter << "%\r";

		grow_axons(config.schwann_l);
		print_network(file_name(config.growth, i), 0, 0);
	}
	std::cout << "100%\n" << std::endl;

	std::cout << "Axon growth complete. Growing dendrites..." << std::endl;
	connect_network(config.schwann_l, config.link_fwhm_param, config.target_age);
}

Brain::~Brain()
{
	for (auto i : all_nodes)
		delete i;

	delete live_nodes;
	delete live_nodes_s;
}

void Brain::clear_signals(void)
{
	/* Phase 0 - reset all counters */
	for(auto it_n : all_nodes)
		it_n->clear_signal();
}

void Brain::insert_signal(int neuron_index)
{
	Node *root = neurons[neuron_index].get_root();

	root->set_value(1);
	live_nodes->push_back(root);
	live_nodes_s->insert(root);
}

bool Brain::read_signal(int neuron_index)
{
	return neurons[neuron_index].get_root()->get_value();
}

int Brain::propagate_signal(double thresh)
{
	/* Conceptually a propagation step occurs as follows:
	 *
	 * int sum = 0;
	 * for (auto i : all_nodes)
	 *     i->push_temp_next();
	 * for (auto i : all_nodes)
	 *     sum += i->pop_temp(0.0);
	 *
	 * however it is very inefficient to touch all nodes in the network,
	 * therefore we use the code below instead. This stores a queue (a deque
	 * actually, so that it is iteratable) of all the nodes that are actually
	 * live so that only they are updated. At the same time, a set is
	 * maintained in order to ensure uniqueness, by attempting to add to the
	 * set before adding to the deque. */

	int length = live_nodes->size();

	/* Phase 1 - move to temp variable of next node(s) */
	for (int i = 0; i < length; ++i)
	{
		Node *front = live_nodes->front();
		live_nodes_s->erase(front);
		live_nodes->pop_front();

		front->push_temp_next();

		std::vector<Node*> next = front->get_next();

		for(auto it_n : next)
		{
			auto result = live_nodes_s->insert(it_n);

			if(result.second)
				live_nodes->push_back(it_n);
			/* Only put into the queue if we could successfully insert into
			 * the set first, this ensures that elements in the queue are
			 * unique */
		}
	}

	/* Phase 1.5 - update synapse thresholds */
	for(auto it : all_synapses)
		it->update_threshold();

	/* Phase 2 - move from temp variable to value variable */
	int sum = 0;
	std::vector<Node*> erase;

	for(auto it : *live_nodes)
		if(it->pop_temp(thresh))
			sum++;
		else
			erase.push_back(it);

	/* If propagation does not occur, e.g. in the case of not enough signals
	 * entering a neuron to cause a firing, then this node should be removed
	 * from the collection of live nodes.*/
	for(auto it : erase)
	{
		live_nodes->erase(std::find(live_nodes->begin(), live_nodes->end(), it));
		live_nodes_s->erase(it);
	}

	return sum;
}

void Brain::print_network(const std::ostringstream &fileName, bool isolate_path, bool isolate_signal)
{
	/* If isolate_signal == 1 then nodes which have no signal (value = 0) are
	 * not printed.
	 * If isolate_path == 1 then nodes which are not on the signal path are
	 * not printed. */
	std::ofstream out_stream;
	out_stream.open(fileName.str(), std::fstream::trunc);

	auto nodes = path_nodes;
	if(!isolate_path) nodes = all_nodes;

	for(auto it_n : nodes)
		if(!isolate_signal || it_n->get_value())
		{
			for(auto it_d : it_n->get_pos())
				out_stream << it_d << " ";
			out_stream << std::endl;
		}

	out_stream.close();
}

void Brain::print_neuron_adj(const std::ostringstream &fileName)
{
	std::ofstream out_stream;
	out_stream.open(fileName.str(), std::fstream::trunc);

	for(auto i : neuron_adjacency)
	{
		for(auto j : i)
			out_stream << j << " ";
		out_stream << std::endl;
	}

}

int Brain::network_size(void)
{
	return static_cast<int>(all_nodes.size());
}

int Brain::connections(void)
{
	return static_cast<int>(all_synapses.size());
}

void Brain::place_neurons(int n, std::valarray<std::pair<double, double>> bounds)
{
	for (int i = 0; i < n; i++)
		neurons.push_back(Tree::Tree(bounds, all_nodes, all_synapses));

	neuron_adjacency.resize(n, std::valarray<bool>(false, n));
}

void Brain::grow_axons(double l)
{
	for(auto i : neurons)
		i.grow_axon(l);
}


void Brain::connect_network(double l, double fwhm, int t)
{
	for (int i = 0; i < neurons.size(); ++i)
	{
		std::cout << 100 * i / neurons.size() << "%\r";
		for (int j = 0; j < neurons.size(); ++j)
		{
			if(neurons[i].get_root() == neurons[j].get_root())
				continue;
			else if(Node *synapse = neurons[i].grow_branch(neurons[j], l, fwhm, t))
			{
				neuron_adjacency[i][j] = true;
				// synapses.push_back(synapse);
			}
		}
	}

	std::cout << "100%\n" << std::endl;
}
