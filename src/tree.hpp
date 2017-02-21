#pragma once

#include <fstream>
#include <iostream>
#include <limits>
#include <tuple>

#include "rand_gen.hpp"
#include "node.hpp"
#include "config.h"

// A tree is one neuron/axon/dendrite(s) system. The root node is the neuron,
// from which the axon is grown. Dendrites are then grown from neighbouring
// axons back the the root neuron.
class Tree
{
public:
	Tree(std::valarray<std::pair<double, double>> b, int cr,
		std::vector<Node*> &a, std::vector<Synapse*> &s);

	Neuron *get_root(void);
	std::valarray<double> get_grow_dir(void);

	// Grow one unit of axon with length l in the direction grow_dir.
	void grow_axon(double l);

	// Grow a branch (dendrite) from another axon (that which emerges from
	// target) the root neuron of this tree.
	Node *grow_branch(Tree &target, user_config_t &cf);

private:
	// Create a unit box.
	std::valarray<std::pair<double, double>> unit_box(size_t d);

	// Generate a random vector that lies within box.
	std::valarray<double> r_vec(std::valarray<std::pair<double, double>> box);

	// Find the point on a target axon at which the distance to the root node
	// of this tree is the shortest.
	double find_shortest(const Tree &target, Node **shortest_ptr,
		std::valarray<double> &vec_r, user_config_t &cf);

	// Add a new node positioned relative to the node add_at.
	Node *add_node(Node *add_at, std::valarray<double> rel_pos);

	// Add a new synapse node positioned relative to the node add_at.
	Synapse *add_synapse(Node *add_at, std::valarray<double> rel_pos, int t);

	// Translate a point p such that it lies within the system bounds.
	void impose_bc(std::valarray<double> &p);

	void normalise(std::valarray<double> &v);
	double gaussian(double x, double c);

	Neuron *root;

	// Pointers to the arrays which keep track of the whole network/all of the
	// synapses.
	// To do: replace with smart pointers.
	std::vector<Node*> *all;
	std::vector<Synapse*> *all_s;

	std::valarray<double> grow_dir;
	std::valarray<std::pair<double, double>> bounds;
};

// A comparison for use in the depth first path search in the Brain class.
struct TreeRootComp
{
 	explicit TreeRootComp(Node *n) : node(n) { }
 	inline bool operator()(Tree & m) const { return m.get_root() == node; }
private:
 	Node *node;
};
