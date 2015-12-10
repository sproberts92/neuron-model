#include "interface_win.h"

void read_key_state(struct key_state_t &key_state)
{
	key_state.key = false;

	key_state.esc = (GetKeyState(0x1B) & WM_KEYDOWN) ? pressed(key_state) : false;
	
	key_state.a = (GetKeyState(0x41) & WM_KEYDOWN) ? pressed(key_state) : false;
	key_state.s = (GetKeyState(0x53) & WM_KEYDOWN) ? pressed(key_state) : false;
	key_state.d = (GetKeyState(0x44) & WM_KEYDOWN) ? pressed(key_state) : false;

	key_state.g = (GetKeyState(0x47) & WM_KEYDOWN) ? pressed(key_state) : false;
	key_state.b = (GetKeyState(0x42) & WM_KEYDOWN) ? pressed(key_state) : false;
}

bool pressed(struct key_state_t &key_state)
{
	key_state.key = true;
	return true;
}