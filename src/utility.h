#pragma once

#include <sstream>

extern "C" {
	#include "config.h"
}

std::ostringstream file_name(path_t &path);
std::ostringstream file_name(path_t &path, int it);
