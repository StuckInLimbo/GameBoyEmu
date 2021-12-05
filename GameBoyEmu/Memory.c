#include "Memory.h"

uint8_t ReadMem(StateGB* state, uint16_t address) {
	return state->memory[address];
}

void WriteMem(StateGB* state, uint16_t address, uint8_t value) {
	state->memory[address] = value;
}