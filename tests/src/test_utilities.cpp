#include "test_utilities.hpp"

bool valarrays_are_equal(std::valarray<bool> result)
{
 	int reduced = std::accumulate(begin(result), end(result), 0);
 	return reduced == result.size() ? true : false;
}
