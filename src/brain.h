#pragma once

#include <fstream>
#include <sstream>

#include "neuron.h"

class Brain
{
public:
	std::vector<Node*> all_nodes;
	Neuron *neurons;

	Brain(int d, std::vector<double> b, int n, double l);
	~Brain(void);

	void place_neurons(void);
	void grow_axons(void);
	void Brain::connect_network(void);
	void Brain::print_network(std::ostringstream &fileName, bool no_signal);
	void Brain::propagate_signal(void);
	Node *grow_axon(Node *base, std::vector<double> g_dir);
	Node *Brain::branch_axon(Node *base, std::vector<double> g_dir);

private:
	int n_neurons;
	int dim;
	std::vector<double> bounds;

	double schwann_l;
};