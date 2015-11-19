#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <map>
#include <utility>

#include "utility.h"
#include "rand_gen.h"
#include "tree.h"

class Brain
{
public:
	void create_network(user_config_t &config);

	void find_loops(int n);

	void clear_signals(void);
	void insert_signal(int neuron_index);
	bool read_signal(int neuron_index);
	void propagate_signal(double noise);

	void print_network(std::ostringstream &fileName, bool no_signal);
	int network_size(void);
	int connections(void);

private:
	void place_neurons(int n, std::valarray<std::pair<double, double>> b);
	void grow_axons(double l);
	void connect_network(double l);

	void turn_off_synapses(std::vector<Node*> path_s);
	bool depth_first_path_search(int &ln, int &loop_length, Node &node, Node &root, std::vector<Node*> &r_path, std::vector<Node*> &s_path);
	bool set_root_flag(Node &node, std::vector<Node*> &path);
	bool set_synapse_flag(Node &node, std::vector<Node*> &path);
	std::vector<Tree> neurons;
	std::vector<Node*> all_nodes;
	std::vector<Node*> synapses;
};

// std::vector<Node*> all_synapses;
// std::vector<bool> all_synapses_state;
// std::map<Node*, int> synapses;