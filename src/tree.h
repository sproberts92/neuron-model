#pragma once

#include <fstream>
#include <iostream>
#include <limits>
#include <utility>

#include "rand_gen.h"
#include "node.h"

class Tree
{
public:
	Tree();
	Tree(std::valarray<std::pair<double, double>> b, std::vector<Node*> &all);

	Node *get_root(void);
	std::valarray<double> get_grow_dir(void);
	
	void grow_axon(double l);
	void grow_branch(Tree &target, double l);

private:
	std::valarray<std::pair<double, double>> unit_box(size_t d);
	std::valarray<double> r_vec(std::valarray<std::pair<double, double>> box);
	void normalise(std::valarray<double> &v);
	
	double find_shortest(const Tree &target, Node **shortest_ptr);
	Node *add_node(Node *add_at, std::valarray<double> g_dir);
	void impose_bc(std::valarray<double> &p);

	Node *root;
	std::vector<Node*> *all;
	std::valarray<double> grow_dir;
	std::valarray<std::pair<double, double>> bounds;
};