#include "test_test_utilities.hpp"

TEST_CASE("Valarrays are equal")
{
	std::valarray<double> one = {1.23, 4.56, 7.89};
	std::valarray<double> two = {1.21, 4.56, 7.89};
	std::valarray<double> thr = {9.87, 6.54, 3.21};

	REQUIRE(valarrays_are_equal(one == one));
	REQUIRE(!valarrays_are_equal(one == two));
	REQUIRE(!valarrays_are_equal(one == thr));
}

TEST_CASE("Valarrays are close")
{
	std::valarray<double> one = {1.23, 4.56, 7.89};
	std::valarray<double> two = {1.21, 4.56, 7.89};
	std::valarray<double> thr = {9.87, 6.54, 3.21};	

	double precision = 0.001;

	REQUIRE(valarrays_are_close(one, one, precision));
	REQUIRE(!valarrays_are_close(one, two, precision));
	REQUIRE(!valarrays_are_close(one, thr, precision));
}