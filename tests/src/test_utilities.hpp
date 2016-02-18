#pragma once

#include <valarray>
#include <numeric>

bool valarrays_are_equal(std::valarray<bool> result);

template<typename T>
bool valarrays_are_close(std::valarray<T> one, std::valarray<T> two, double precision)
{
	bool result = true;

	if(one.size() != two.size())
		return false;
	else
	{	
		for (int i = 0; i < one.size(); ++i)
				result &= abs(one[i] - two[i]) < precision;
		return result;
	}
}
