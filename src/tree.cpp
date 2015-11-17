#include "tree.h"

Tree::Tree(int d, std::vector<double> box, std::vector<Node*> &all)
{
	root = new Node(d , r_vec(box));
	all.push_back(root);

	grow_dir = r_vec(unit_box(d));
	normalise(grow_dir);
}

Node *Tree::get_root(void)
{
	return root;
}

std::valarray<double> Tree::get_grow_dir(void)
{
	return grow_dir;
}

Node *Tree::find_shortest(Tree &target)
{
	Node *list_ptr = target.root, *shortest_ptr = nullptr;
	double shortest_r = std::numeric_limits<double>::infinity();

	while(!list_ptr->get_next().empty())
	{
		auto temp = list_ptr->get_pos() - root->get_pos();
		double r = (temp * temp).sum();

		/* Compare r^2 values, don't need to waste time with sqrt */
		if(r < shortest_r)
		{
			shortest_r = r;
			shortest_ptr = list_ptr;
		}

		list_ptr = list_ptr->get_next().front();
	}

	/* CHECK UP ON BOUNDARY CONDITIONS*/

	return shortest_ptr;
}

std::valarray<double> Tree::r_vec(std::vector<double> b_box)
{
	std::valarray<double> vec(b_box.size() / 2);
	static rand_gen<double> r_gen(0, 1);

	for (int i = 0; i < vec.size(); ++i)
		vec[i] = ((b_box[2 * i + 1] - b_box[2 * i]) * r_gen.get_rnum() + b_box[2 * i]);		

	return vec;
}

std::vector<double> Tree::unit_box(int d)
{
	std::vector<double> box;

	for (int i = 0; i < d; ++i)
	{ 
		box.push_back(-1);
		box.push_back(1);
	}
	
	return box;
}

void Tree::normalise(std::valarray<double> &v)
{
	v /= sqrt((v * v).sum());
}
