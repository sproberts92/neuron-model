#pragma once

#include <fstream>

#include "rand_gen.h"
#include "node.h"

class neuron
{
public:
	int dim;
	node *base_soma;
	std::vector<double> grow_dir;

	neuron();
	neuron(int d, std::vector<double> box);
	neuron(const neuron &n);
	neuron &operator= (const neuron &n);
	~neuron();

	void neuron::grow_axon(double rate, std::vector<double> &box);
	void print_axon(std::ofstream &out_stream);

private:
	std::vector<double> r_vec(int dim, std::vector<double> b_box);
	void normalise_grow_dir(void);
};