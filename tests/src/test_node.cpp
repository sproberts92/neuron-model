#include "node_test.hpp"

TEST_CASE("Test node setup", "[node]")
{
	std::valarray<double> test_pos = {1.1, 2.2, 3.3};
	Node test_node(test_pos);
	Node next_node({0.0, 0.0, 0.0});

	SECTION("Test position")
	{
		REQUIRE(valarrays_are_equal(test_node.get_pos() == test_pos));
		REQUIRE(test_node.get_value() == 0);
	}

	SECTION("Test get and set")
	{
		bool value = true;
		test_node.set_value(value);

		REQUIRE(test_node.get_value() == value);

		value = false;
		test_node.set_value(value);

		REQUIRE(test_node.get_value() == value);
	}

	SECTION("Test next node")
	{
		std::vector<Node*> next_comparison;
		next_comparison.push_back(&next_node);

		test_node.push_next(next_node);

		REQUIRE(test_node.get_next() == next_comparison);
	}
}

TEST_CASE("Test node propagation", "[node]")
{
	Node test_node({0.0, 0.0, 0.0});
	Node next_node({0.0, 0.0, 0.0});
	test_node.push_next(next_node);

	SECTION("Test propagation")
	{
		test_node.set_value(true);
		next_node.set_value(false);

		test_node.push_temp_next();
		next_node.pop_temp(0.0);

		REQUIRE(next_node.get_value() == true);
	}

	SECTION("Test clear signal")
	{
		test_node.set_value(true);
		test_node.clear_signal();
		REQUIRE(test_node.get_value() == false);

		test_node.set_value(true);
		next_node.set_value(false);

		test_node.push_temp_next();
		next_node.clear_signal();
		next_node.pop_temp(0.0);

		REQUIRE(next_node.get_value() == false);
	}

	SECTION("Test on and off")
	{
		test_node.set_value(true);
		next_node.set_value(false);

		next_node.turn_off();
		test_node.push_temp_next();
		next_node.pop_temp(0.0);

		REQUIRE(next_node.get_value() == false);

		test_node.set_value(true);
		next_node.set_value(false);

		next_node.turn_on();
		test_node.push_temp_next();
		next_node.pop_temp(0.0);

		REQUIRE(next_node.get_value() == true);
	}

	SECTION("Test noise")
	{
		test_node.set_value(true);
		next_node.set_value(false);

		test_node.push_temp_next();
		next_node.pop_temp(1.0);

		REQUIRE(next_node.get_value() == false);
	}
}

TEST_CASE("Test Neuron derived from Node")
{
	
}

TEST_CASE("Test Synapse derived from Node")
{
	
}