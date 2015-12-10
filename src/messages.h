#pragma once

#include <unordered_map>

// Format:
// {sentinal, object, object, object, opinion, opinion, opinion flag, sentinel}

std::unordered_map<std::string, std::vector<bool>> messages = {
	{"ag", {1,0,0,1,0,1,1,1}},
	{"a",  {1,0,0,1,0,0,0,1}},
	{"rg", {1,0,1,0,0,1,1,1}},
	{"r",  {1,0,1,0,0,0,0,1}}
};