#pragma once

#include <unordered_map>

// Format:
// {sentinal, object, object, object, opinion, opinion, opinion flag, sentinel}

std::unordered_map<std::string, std::vector<bool>> messages = {
	{"ag", {1,0,0,1,0,1,1,1}},
	{"ab", {1,0,0,1,1,0,1,1}},
	{"a",  {1,0,0,1,0,0,0,1}},
	{"sg", {1,0,1,0,0,1,1,1}},
	{"sb", {1,0,1,0,1,0,1,1}},
	{"s",  {1,0,1,0,0,0,0,1}},
	{"dg", {1,0,1,1,0,1,1,1}},
	{"db", {1,0,1,1,1,0,1,1}},
	{"d",  {1,0,1,1,0,0,0,1}}
};
