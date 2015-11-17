#pragma once

#include <fstream>
#include <iostream>
#include <limits>

#include "rand_gen.h"
#include "node.h"

class Tree
{
public:
	Tree();
	Tree(int d, std::vector<double> box, std::vector<Node*> &an);

	Node *get_root(void);
	std::valarray<double> get_grow_dir(void);
	
	Node *find_shortest(Tree &target);

private:
	std::valarray<double> r_vec(std::vector<double> b_box);
	std::vector<double> unit_box(int d);
	void normalise(std::valarray<double> &v);
	
	Node *root;
	std::valarray<double> grow_dir;
};