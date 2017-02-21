#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <deque>
#include <map>
#include <set>
#include <array>

#include "utility.hpp"
#include "rand_gen.hpp"
#include "tree.hpp"
#include "config.h"

// POD for statistics gathered cumulatively over many update steps.
struct Long_Run_Statistics
{
	std::array<int, 150> last_visited = {0};
	std::array<int, 1<<9> pattern_found = {0};
	// To do: have pattern_found length read for config if run-time sized
	// arrays ever make it into the standard.
};

// The Brain class contains a collections of trees. Is this way, the Brain
// contains the whole network. This encapsulation is needed so that we can
// destroy the Brain cleanly and create a new one for repeated simulations.
class Brain
{
public:
	Brain();
	~Brain();

	void create_network(user_config_t &config);

	// Use depth_first_path_search to find n cycles of equal length through the
	// network.
	bool find_loops(int n);

	void clear_signals(void);
	void insert_signal(int neuron_index);
	bool read_signal(int neuron_index);

	// Complete a propagation step for all nodes in the network, including
	// gathering of statistics related to the update.
	int propagate_signal(double noise, Long_Run_Statistics &lrs);

	void print_network(const std::ostringstream &fileName,
		bool isolate_path, bool isolate_signal);
	void print_neurons(const std::ostringstream &fileName);
	void print_neuron_adj(const std::ostringstream &fileName);

	// The number of nodes in the entire system.
	int network_size(void);

	// The number of synapses in the system, this is equivalent to the sum of
	// all elements in the adjacency matrix.
	int connections(void);

	std::vector<Node*> all_nodes;
	std::vector<Node*> path_nodes;

	// The collections of nodes that are currently active (value != 0), for
	//more efficient signal propagation.
	std::deque<Node*> *live_nodes;

private:
	// Place n neurons randomly in a box defined by b, initialised with a
	// propagation rule such that cr incoming signals are required for firing.
	void place_neurons(int n, std::valarray<std::pair<double, double>> b, int cr);

	// For each of the neurons in the system grow an axon along the neuron's
	// grow_dir, with each Schwann cell having a length of l.
	void grow_axons(double l);

	// Use the minimum distance method to grow dendrites between neurons and
	// neighbouring axons.
	void connect_network(user_config_t &cf);

	// Turn off all synapses that do not lie on path_s, which was found by the
	// find_loops function.
	void turn_off_synapses(std::vector<Node*> path_s);

	// To do: There are hard coded limits on search time which will need to be
	// read from config.
	bool depth_first_path_search(int &ln, int &loop_length, Node &node,
		Node &loop_root, std::vector<Node*> &r_path,
		std::vector<Node*> &s_path,int &loop_c, int &loop_l, int &iter);

	// Toggle flags for use in the depth_first_path_search.
	bool set_root_flag(Node &node, std::vector<Node*> &path);
	bool set_synapse_flag(Node &node, std::vector<Node*> &path);
	bool set_u_node_flag(Node &node);

	std::vector<Tree> neurons;
	std::vector<Synapse*> all_synapses;

	// The set of nodes that are currently active (value != 0) used in
	// conjunction with live_nodes, for more efficient signal propagation to
	// avoid double updating.
	std::set<Node*> *live_nodes_s;

	// The adjacency matrix of the neurons in the system.
	std::valarray<std::valarray<bool>> neuron_adjacency;
};
