#pragma once

#include <Windows.h>
#include "interface_type.hpp"

void read_key_state(struct key_state_t &key_state);
bool pressed(struct key_state_t &key_state);
