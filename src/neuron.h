#pragma once

#include <fstream>
#include <iostream>
#include <limits>

#include "rand_gen.h"
#include "node.h"

class Neuron
{
public:
	int dim;
	Node *base_soma;
	std::vector<double> grow_dir;
	std::vector<Node*> *all_nodes;

	Neuron();
	Neuron(int d, std::vector<double> box, std::vector<Node*> *an);
	Neuron(const Neuron &n);
	Neuron &operator= (const Neuron &n);
	~Neuron();

	Node *Neuron::find_shortest(Neuron &target);

private:
	std::vector<double> Neuron::r_vec(int dim, std::vector<double> b_box);
	std::vector<double> Neuron::r_u_vec(int dim);
	void normalise_grow_dir(void);
};