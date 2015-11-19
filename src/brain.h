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

	void create_network(user_config_t &config);
	
	void find_loops(void);
	
	void clear_signals(void);
	void insert_signal(int neuron_index);
	void propagate_signal(double noise);
	
	void print_network(std::ostringstream &fileName, bool no_signal);
	int network_size(void);

private:
	void place_neurons(int n, std::valarray<std::pair<double, double>> b);
	void grow_axons(void);
	void connect_network(void);
	
	void depth_first_path_search(Node &node, Node &root, std::vector<Node*> path);
	
	double gaussian(double x, double c);

	double schwann_l;

	rand_gen<double> r_gen;

	std::vector<Tree> neurons;
	std::vector<Node*> all_nodes;
};

// std::vector<Node*> all_synapses;
// std::vector<bool> all_synapses_state;
// std::map<Node*, int> synapses;