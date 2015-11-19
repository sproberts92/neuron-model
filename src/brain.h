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
	Brain(double l);

	void create_network(user_config_t &config, std::valarray<std::pair<double, double>> bounds);
	int network_size(void);
	void place_neurons(int n, std::valarray<std::pair<double, double>> b);
	void grow_axons(void);
	void connect_network(void);
	void print_network(std::ostringstream &fileName, bool no_signal);
	void propagate_signal(double noise);
	void clear_signals(void);
	void find_loops(void);
	void insert_signal(int neuron_index);

private:
	double gaussian(double x, double c);
	void depth_first_path_search(Node &node, Node &root, std::vector<Node*> path);

	double schwann_l;

	rand_gen<double> r_gen;

	std::vector<Tree> neurons;
	std::vector<Node*> all_nodes;
};

// std::vector<Node*> all_synapses;
// std::vector<bool> all_synapses_state;
// std::map<Node*, int> synapses;