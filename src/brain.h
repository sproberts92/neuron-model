#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <map>
#include <utility>

#include "rand_gen.h"
#include "tree.h"

class Brain
{
public:
	std::vector<Tree> neurons;

	Brain(int d, int n, double l);

	int network_size(void);
	void place_neurons(std::valarray<std::pair<double, double>> b);
	void grow_axons(void);
	void connect_network(void);
	void print_network(std::ostringstream &fileName, bool no_signal);
	void propagate_signal(double noise);
	void clear_signals(void);
	void check_path(double thresh);
	void depth_first_path_search(Node &node, Node &root, std::vector<Node*> path);

private:
	std::vector<Node*> all_nodes;
	int n_neurons;
	int dim;
	rand_gen<double> r_gen;

	double schwann_l;

	Node *Brain::branch_axon(Node &base, std::valarray<double> g_dir);
	double gaussian(double x, double c);
};

// std::vector<Node*> all_synapses;
// std::vector<bool> all_synapses_state;
// std::map<Node*, int> synapses;