#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <deque>
#include <map>
#include <set>

#include "utility.hpp"
#include "rand_gen.hpp"
#include "tree.hpp"

class Brain
{
public:
	void create_network(user_config_t &config);

	void find_loops(int n);

	void clear_signals(void);
	void insert_signal(int neuron_index);
	bool read_signal(int neuron_index);
	int propagate_signal(double noise);

	void print_network(const std::ostringstream &fileName, bool isolate_path, bool isolate_signal);
	void print_neuron_adj(const std::ostringstream &fileName);
	int network_size(void);
	int connections(void);
	void read_message(std::vector<bool> &message);

	std::vector<Node*> all_nodes;
	std::vector<Node*> path_nodes;
	std::deque<Node*> live_nodes;

private:
	void place_neurons(int n, std::valarray<std::pair<double, double>> b);
	void grow_axons(double l);
	void connect_network(double l, double fwhm);
	void grow_read_assembly(int neuron_index);

	void turn_off_synapses(std::vector<Node*> path_s);
	bool depth_first_path_search(int &ln, int &loop_length, Node &node, Node &root, std::vector<Node*> &r_path, std::vector<Node*> &s_path);
	bool set_root_flag(Node &node, std::vector<Node*> &path);
	bool set_synapse_flag(Node &node, std::vector<Node*> &path);
	bool Brain::set_u_node_flag(Node &node);
	std::vector<Tree> neurons;
	std::vector<Node*> read_nodes;
	std::vector<Node*> synapses;
	std::set<Node*> live_nodes_s;
	std::valarray<std::valarray<bool>> neuron_adjacency;
};
