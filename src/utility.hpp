#pragma once

#include <sstream>
#include <valarray>

extern "C" {
	#include "config.h"
}

std::ostringstream file_name(path_t &path);
std::ostringstream file_name(path_t &path, int it);
std::ostringstream file_name(path_t &path, std::initializer_list<int> list);
std::valarray<std::pair<double, double>> convert_bounds(user_config_t &config);
