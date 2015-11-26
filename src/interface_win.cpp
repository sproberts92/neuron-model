#include "interface_win.h"

void read_key_state(struct key_state_t &key_state)
{
	key_state.esc = (GetKeyState(0x1B) & WM_KEYDOWN) ? true : false;
	
	key_state.a = (GetKeyState(0x41) & WM_KEYDOWN) ? true : false;
	key_state.l = (GetKeyState(0x4C) & WM_KEYDOWN) ? true : false;

	key_state.g = (GetKeyState(0x47) & WM_KEYDOWN) ? true : false;
	key_state.b = (GetKeyState(0x42) & WM_KEYDOWN) ? true : false;
}