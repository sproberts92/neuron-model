#include "brain.hpp"

Brain::Brain(void)
{
	live_nodes = new std::deque<Node*>;
	live_nodes_s = new std::set<Node*>;
}

void Brain::create_network(user_config_t &cf)
{
	auto bounds = convert_bounds(cf);
	place_neurons(cf.n_neurons, bounds, cf.combination_rule);

	std::cout << "Neuron layout complete. Growing axons..." << std::endl;

	for (int i = 0; i < cf.growth_iter; i++)
	{
		std::cout << 100 * i / cf.growth_iter << "%\r";

		grow_axons(cf.schwann_l);
		print_network(file_name(cf.growth, i), 0, 0);
	}
	std::cout << "100%\n" << std::endl;

	std::cout << "Axon growth complete. Growing dendrites..." << std::endl;
	connect_network(cf);
	print_network(file_name(cf.network_c), 0, 0);
	print_neurons(file_name(cf.network_n));
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

int Brain::propagate_signal(double thresh, Long_Run_Statistics &lrs)
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
	for(auto it : all_nodes)
		it->update_threshold();

	/* Phase 2 - move from temp variable to value variable */
	int sum = 0;
	std::vector<Node*> erase;

	for(auto it : *live_nodes)
	{
		Statistics ns;

		if(it->pop_temp(thresh, ns))
		{
			sum++;

			if(ns.last_visited > 0 && ns.last_visited < 150)
				lrs.last_visited[ns.last_visited]++;
		}
		else
			erase.push_back(it);

		if(!ns.pattern_found.empty())
			for(auto it : ns.pattern_found)
				if(it > 0) lrs.pattern_found[it]++;
	}
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
	/* If isolate_path == 1 then nodes which are not on the signal path are
	 * not printed.
	 * If isolate_signal == 1 then nodes which have no signal (value = 0) are
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

void Brain::print_neurons(const std::ostringstream &fileName)
{
	/* Print the positions of neurons only */

	std::ofstream out_stream;
	out_stream.open(fileName.str(), std::fstream::trunc);

	for(auto it_n : neurons)
	{
		for(auto it_d : it_n.get_root()->get_pos())
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

void Brain::place_neurons(int n, std::valarray<std::pair<double, double>> bounds, int cr)
{
	for (int i = 0; i < n; i++)
		neurons.push_back(Tree::Tree(bounds, cr, all_nodes, all_synapses));

	neuron_adjacency.resize(n, std::valarray<bool>(false, n));
}

void Brain::grow_axons(double l)
{
	for(auto i : neurons)
		i.grow_axon(l);
}


void Brain::connect_network(user_config_t &cf)
{
	for (int i = 0; i < neurons.size(); ++i)
	{
		std::cout << 100 * i / neurons.size() << "%\r";
		for (int j = 0; j < neurons.size(); ++j)
		{
			if(neurons[i].get_root() == neurons[j].get_root())
				continue;
			else if(Node *synapse = neurons[i].grow_branch(neurons[j], cf))
			{
				neuron_adjacency[i][j] = true;
				// synapses.push_back(synapse);
			}
		}
	}

	std::cout << "100%\n" << std::endl;
}

void Brain::find_loops(int n)
{
	std::vector<Node*> n_path, r_path, s_path;
	/* n_paths = node path
	 * r_paths = root path (ie neurons)
	 * s_paths = synapse path */

	int loop_num = 3;
	int loop_length = 0;
	Node *loop_root = neurons.front().get_root();
	depth_first_path_search(loop_num, loop_length, *loop_root, *loop_root, r_path, s_path);
	std::cout << loop_length << std::endl;
	turn_off_synapses(s_path);

	std::cout << "Path nodes: " << path_nodes.size() << std::endl;
}

bool Brain::depth_first_path_search(int &ln, int &loop_length, Node &node, Node &loop_root, std::vector<Node*> &r_path, std::vector<Node*> &s_path)
{
	static int loop_c = 0;
	static int loop_l = std::numeric_limits<int>::max();

	if(loop_length - (loop_l*loop_c) - 5 > loop_l)
		return false;

	loop_length++;
	bool is_root = set_root_flag(node, r_path);
	bool is_synapse = set_synapse_flag(node, s_path);
	bool is_u_node = set_u_node_flag(node);

	for(auto it_n : node.get_next())
	{
		if(it_n == &loop_root && loop_c == 0)
		{
			loop_l = loop_length;
			loop_c++;

			std::cout << "Loop found, length: " << loop_l << " nodes, " << r_path.size() << " neurons." << std::endl;
			if(depth_first_path_search(ln, loop_length, *it_n, loop_root, r_path, s_path))
				return true;
		}
		else if(it_n == &loop_root && loop_length - (loop_l*loop_c) == loop_l)
		{
			std::cout << "Degenerate loop found, " << loop_length - 1 << " " << r_path.size() << " neurons." << std::endl;
			if(++loop_c == ln || depth_first_path_search(ln, loop_length, *it_n, loop_root, r_path, s_path))
				return true;
		}
		else if(!(std::find(r_path.begin(), r_path.end(), it_n) != r_path.end()))
			if(depth_first_path_search(ln, loop_length, *it_n, loop_root, r_path, s_path))
				return true;
	}

	loop_length--;
	if(is_root)	r_path.pop_back();
	if(is_synapse) s_path.pop_back();
	if(is_u_node) path_nodes.pop_back();

	return false;
}

void Brain::turn_off_synapses(std::vector<Node*> path_s)
{
	for(auto it : all_synapses)
		if(!(std::find(path_s.begin(), path_s.end(), it) != path_s.end()))
			it->turn_off();
}

bool Brain::set_root_flag(Node &node, std::vector<Node*> &path)
{
	if(std::find_if(neurons.begin(), neurons.end(), TreeRootComp(&node)) != neurons.end())
	{
		path.push_back(&node);
		return true;
	}
	else return false;
}

bool Brain::set_synapse_flag(Node &node, std::vector<Node*> &path)
{
	if(std::find(all_synapses.begin(), all_synapses.end(), &node) != all_synapses.end())
	{
		path.push_back(&node);
		return true;
	}
	else return false;
}

bool Brain::set_u_node_flag(Node &node)
{
	if(!(std::find(path_nodes.begin(), path_nodes.end(), &node) != path_nodes.end()))
	{
		path_nodes.push_back(&node);
		return true;
	}
	else return false;
}
