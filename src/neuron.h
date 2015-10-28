#pragma once

#include <fstream>
#include <iostream>
#include <limits>

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

	node *neuron::find_shortest(neuron &target);

private:
	std::vector<double> neuron::r_vec(int dim, std::vector<double> b_box);
	std::vector<double> neuron::r_u_vec(int dim);
	void normalise_grow_dir(void);
};