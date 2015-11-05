#pragma once

#include <fstream>
#include <sstream>

#include "neuron.h"
#include "rand_gen.h"
class Brain
{
public:
	std::vector<Node*> all_nodes;
	std::vector<Node*> all_synapses;
	Neuron *neurons;

	Brain(int d, std::vector<double> b, int n, double l);
	~Brain(void);

	void place_neurons(void);
	void grow_axons(void);
	void Brain::connect_network(void);
	void Brain::print_network(std::ostringstream &fileName, bool no_signal);
	void Brain::propagate_signal(bool noise);

private:
	int n_neurons;
	int dim;
	std::vector<double> bounds;
	std::vector<rand_gen <double>> r_gen;

	double schwann_l;

	Node *grow_axon(Node *base, std::vector<double> g_dir);
	Node *Brain::branch_axon(Node *base, std::vector<double> g_dir);
	double gaussian(double x, double c);
};