#include "tree_test.hpp"

TEST_CASE("Tree set up", "[tree]")
{
	std::vector<Node*> all;
	std::valarray<std::pair<double, double>> bounds = {{-10.0, 10.0}, {-10.0, 10.0}, {-10.0, 10.0}};

	Tree test_tree(bounds, all);

	SECTION("Tree construction")
	{
		std::valarray<double> test_position = test_tree.get_root()->get_pos();

		for (int i = 0; i < test_position.size(); ++i)
		{
			REQUIRE(test_position[i] > bounds[i].first);
			REQUIRE(test_position[i] < bounds[i].second);
		}

		std::valarray<double> test_grow_dir = test_tree.get_grow_dir();
		for (auto i : test_grow_dir)
			REQUIRE(i < 1.0f);
	}

	SECTION("Axon growth")
	{
		double grow_length = 1.23;

		test_tree.grow_axon(grow_length);

		Node *root_node = test_tree.get_root();
		std::vector<Node*> next_nodes = root_node->get_next();

		for (auto i : next_nodes)
		{
			std::valarray<double> diff = i->get_pos() - root_node->get_pos();

			double precision = 0.0001;
			double length = sqrt((diff*diff).sum());

			REQUIRE(length < grow_length + precision);
			REQUIRE(length > grow_length - precision);
		}

		for (auto i : next_nodes)
		{
			bool result = valarrays_are_close(
				i->get_pos() - root_node->get_pos(),
				grow_length * test_tree.get_grow_dir(),
				0.1);

			REQUIRE(result);
		}
	}
}

TEST_CASE("Test tree branching", "[tree]")
{
	std::vector<Node*> all;
	std::valarray<std::pair<double, double>> bounds = {{-10.0, 10.0}, {-10.0, 10.0}, {-10.0, 10.0}};

	double grow_length = 0.1;

	Tree test_tree(bounds, all);
	Tree test_target(bounds, all);

	SECTION("Test branching")
	{
		test_target.grow_axon(grow_length);
		test_tree.grow_axon(grow_length);

		test_tree.grow_branch(test_target, grow_length, 100.0);
		test_target.grow_branch(test_tree, grow_length, 100.0);

		test_target.get_root()->set_value(true);

		int test_sum = 0;
		for (int i = 0; i < 2 *  all.size(); ++i)
		{
			test_sum += test_tree.get_root()->get_value();

			for (auto i : all)
				i->push_temp_next();
			for (auto i : all)
				i->pop_temp(0.0);
		}

		REQUIRE(test_sum == 2);
	}
}
