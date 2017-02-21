#pragma once

#include <sstream>
#include <valarray>

extern "C" {
	#include "config.h"
}

// A collection of utility functions, mainly path building for output files
// but anything of generic nature can be added here too.

std::ostringstream file_name(path_t &path);
std::ostringstream file_name(path_t &path, int it);
std::ostringstream file_name(path_t &path, std::initializer_list<int> list);
std::ostringstream file_name(const std::string& path, std::initializer_list<int> list);

std::valarray<std::pair<double, double>> convert_bounds(user_config_t &config);
