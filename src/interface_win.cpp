#include "interface_win.h"

void read_key_state(struct key_state_t &key_state)
{
	key_state.esc = (GetKeyState(0x1B) & 0x8000) ? true : false;
	
	key_state.a = (GetKeyState(0x41) & 0x8000) ? true : false;
	key_state.l = (GetKeyState(0x4C) & 0x8000) ? true : false;

	key_state.g = (GetKeyState(0x47) & 0x8000) ? true : false;
	key_state.b = (GetKeyState(0x42) & 0x8000) ? true : false;
}