#pragma once

#include <fstream>
#include <iostream>
#include <limits>
#include <tuple>

#include "rand_gen.hpp"
#include "node.hpp"

class Tree
{
public:
	Tree();
	Tree(std::valarray<std::pair<double, double>> b, std::vector<Node*> &all);

	Neuron *get_root(void);
	std::valarray<double> get_grow_dir(void);

	void grow_axon(double l);
	Node *grow_branch(Tree &target, double l, double c);

private:
	std::valarray<std::pair<double, double>> unit_box(size_t d);
	std::valarray<double> r_vec(std::valarray<std::pair<double, double>> box);
	void normalise(std::valarray<double> &v);

	double find_shortest(const Tree &target, Node **shortest_ptr);
	Node *add_node(Node *add_at, std::valarray<double> g_dir);
	Synapse *add_synapse(Node *add_at, std::valarray<double> g_dir);
	void impose_bc(std::valarray<double> &p);
	double gaussian(double x, double c);

	Neuron *root;
	std::vector<Node*> *all;
	std::valarray<double> grow_dir;
	std::valarray<std::pair<double, double>> bounds;
};

struct TreeRootComp
{
 	explicit TreeRootComp(Node *n) : node(n) { }
 	inline const bool operator()(Tree & m) const { return m.get_root() == node; }
private:
 	Node *node;
};
