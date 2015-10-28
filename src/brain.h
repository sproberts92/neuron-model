#pragma once

#include "neuron.h"

class brain
{
public:
	std::vector<node> all_nodes;
	neuron *neurons;

	brain(int d, std::vector<double> b, int n, double l);
	~brain(void);

	void place_neurons(void);
	void grow_axons(void);
	void brain::connect_network(void);
	node *grow_axon(node *base, std::vector<double> g_dir);
	node *brain::branch_axon(node *base, std::vector<double> g_dir);

private:
	int n_neurons;
	int dim;
	std::vector<double> bounds;

	double schwann_l;
};