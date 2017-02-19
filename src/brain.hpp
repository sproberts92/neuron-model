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

typedef struct
{
	std::array<int, 150> last_visited = {0};
	std::array<int, 1<<10> pattern_found = {0};
} Long_Run_Statistics;

class Brain
{
public:
	Brain(void);
	~Brain();

	void create_network(user_config_t &config);

	bool find_loops(int n);

	void clear_signals(void);
	void insert_signal(int neuron_index);
	bool read_signal(int neuron_index);
	int propagate_signal(double noise, Long_Run_Statistics &lrs);

	void print_network(const std::ostringstream &fileName, bool isolate_path, bool isolate_signal);
	void print_neurons(const std::ostringstream &fileName);
	void print_neuron_adj(const std::ostringstream &fileName);
	int network_size(void);
	int connections(void);
	void read_message(std::vector<bool> &message);

	std::vector<Node*> all_nodes;
	std::vector<Node*> path_nodes;
	std::deque<Node*> *live_nodes;

private:
	void place_neurons(int n, std::valarray<std::pair<double, double>> b, int cr);
	void grow_axons(double l);
	void connect_network(user_config_t &cf);

	void turn_off_synapses(std::vector<Node*> path_s);
	bool depth_first_path_search(int &ln, int &loop_length, Node &node, Node &loop_root, std::vector<Node*> &r_path, std::vector<Node*> &s_path, int &loop_c, int &loop_l, int &iter);
	bool set_root_flag(Node &node, std::vector<Node*> &path);
	bool set_synapse_flag(Node &node, std::vector<Node*> &path);
	bool set_u_node_flag(Node &node);

	std::vector<Tree> neurons;
	std::vector<Synapse*> all_synapses;
	std::set<Node*> *live_nodes_s;
	std::valarray<std::valarray<bool>> neuron_adjacency;
};
