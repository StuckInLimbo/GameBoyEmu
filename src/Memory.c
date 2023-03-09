#include "Memory.h"

struct StateGB state;

uint8_t ReadMem(uint16_t address) {
	return state.memory[address];
}

uint16_t ReadShort(uint16_t address) {
	return ReadMem(address) | (ReadMem(address + 1) << 8);
}

uint16_t ReadShortFromStack(void) {
	uint16_t value = ReadShort(state.sp);
	state.sp += 2;
	return value;
}

void WriteMem(uint16_t address, uint8_t value) {
	state.memory[address] = value;
}

void WriteMem16(uint16_t address, uint16_t value) {
	uint8_t high, low;
	high = (value >> 8) & 0xFF;
	low = value & 0xFF;
	state.memory[address - 1] = high;
	state.memory[address - 2] = low;
}

void WriteShort(uint16_t address, uint16_t value) {
	WriteMem(address + 0, (value & 0x00FF) >> 0);
	WriteMem(address + 1, (value & 0xFF00) >> 8);
}

void WriteShortToStack(uint16_t value) {
	state.sp -= 2;
	WriteShort(state.sp, value);
}