#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <map>

#include "tree.h"
#include "rand_gen.h"
class Brain
{
public:
	std::vector<Tree> neurons;

	Brain(int d, std::vector<double> b, int n, double l);

	int network_size(void);
	void place_neurons(void);
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
	std::vector<double> bounds;
	rand_gen<double> r_gen;

	double schwann_l;

	Node *grow_axon(Node &base, std::valarray<double> g_dir);
	Node *Brain::branch_axon(Node &base, std::valarray<double> g_dir);
	double gaussian(double x, double c);
};

// std::vector<Node*> all_synapses;
// std::vector<bool> all_synapses_state;
// std::map<Node*, int> synapses;