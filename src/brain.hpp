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

typedef struct
{
	std::array<int, 150> last_visited = {0};
} Long_Run_Statistics;

class Brain
{
public:
	Brain(void);
	~Brain();

	void create_network(user_config_t &config);

	void find_loops(int n);

	void clear_signals(void);
	void insert_signal(int neuron_index);
	bool read_signal(int neuron_index);
	int propagate_signal(double noise, Long_Run_Statistics &lrs);

	void print_network(const std::ostringstream &fileName, bool isolate_path, bool isolate_signal);
	void print_neuron_adj(const std::ostringstream &fileName);
	int network_size(void);
	int connections(void);
	void read_message(std::vector<bool> &message);

	std::vector<Node*> all_nodes;
	std::vector<Node*> path_nodes;
	std::deque<Node*> *live_nodes;

private:
	void place_neurons(int n, std::valarray<std::pair<double, double>> b);
	void grow_axons(double l);
	void connect_network(double l, double fwhm, int t);

	std::vector<Tree> neurons;
	std::vector<Synapse*> all_synapses;
	std::set<Node*> *live_nodes_s;
	std::valarray<std::valarray<bool>> neuron_adjacency;
};
