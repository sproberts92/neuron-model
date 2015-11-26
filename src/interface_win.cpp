#include "interface_win.h"

void read_key_state(struct key_state_t &key_state)
{
	if(GetKeyState(0x1B) & 0x8000) key_state.esc = true;
	
	if(GetKeyState(0x41) & 0x8000) key_state.a = true;
	if(GetKeyState(0x4C) & 0x8000) key_state.l = true;

	if(GetKeyState(0x47) & 0x8000) key_state.g = true;
	if(GetKeyState(0x42) & 0x8000) key_state.b = true;
}