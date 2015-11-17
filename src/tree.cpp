#include "tree.h"

Tree::Tree(int d, std::vector<double> box, std::vector<Node*> &all)
{
	root = new Node(d , r_vec(box));
	all.push_back(root);

	grow_dir = r_vec(unit_box(d));
	normalise(grow_dir);
}

Node *Tree::get_root(void) { return root; }

std::valarray<double> Tree::get_grow_dir(void) { return grow_dir; }

std::valarray<double> Tree::r_vec(std::vector<double> b_box)
{
	std::valarray<double> vec(b_box.size() / 2);
	static rand_gen <double> r_gen(0, 1);

	for (int i = 0; i < vec.size(); ++i)
		vec[i] = ((b_box[2 * i + 1] - b_box[2 * i]) * r_gen.get_rnum() + b_box[2 * i]);		

/*	for(auto it = b_box.begin(); it != b_box.end(); ++it)
	{
		auto min = it++;
		vec.push_back((*it - *min) * r_gen.get_rnum() + *min);
	}
*/	
	return vec;
}

void Tree::normalise(std::valarray<double> &v)
{
	double length = 0.0;
	for (auto x : v)
		length += x * x ;

	for (auto &x : v)
		x /= sqrt(length);
}

std::vector<double> Tree::unit_box(int d)
{
	std::vector<double> box;
	for (int i = 0; i < d; ++i) { box.push_back(-1); box.push_back(1); }
	return box;
}

Node *Tree::find_shortest(Tree &target)
{
	double shortest_r = std::numeric_limits<double>::infinity();
	Node *shortest_ptr = nullptr;

	Node *list_ptr;
	list_ptr = target.root;

	// for (int i = 0; i < 80; i++)
		// if(!list_ptr->get_next().empty()) list_ptr = list_ptr->get_next()[0];

	if(list_ptr != nullptr)
		while(!list_ptr->get_next().empty())
		{
			double r = 0.0;
			int dim = (int)list_ptr->get_pos().size();
			for (int i = 0; i < dim; i++)
				r += (list_ptr->get_pos()[i] - this->root->get_pos()[i])*(list_ptr->get_pos()[i] - this->root->get_pos()[i]);

			/* Compare r^2 values, don't need to waste time with sqrt */
			if(abs(r) < shortest_r)
			{
				shortest_r = abs(r);
				shortest_ptr = list_ptr;
			}
			list_ptr = list_ptr->get_next()[0];
		}

	/* CHECK UP ON BOUNDARY CONDITIONS*/

	return shortest_ptr;
}