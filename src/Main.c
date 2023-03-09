#pragma once
#include "Memory.h"
#include "CPU.h"

void ReadFileIntoMemoryAt( char* filename, uint32_t offset) {
	FILE* f; 
	fopen_s(&f, filename, "rb");
	if (f == NULL) {
		printf("error: Couldn't open %s\n", filename);
		exit(1);
	}
	fseek(f, 0L, SEEK_END);
	int fsize = ftell(f);
	fseek(f, 0L, SEEK_SET);

	uint8_t* buffer = &state.memory[offset];
	fread(buffer, fsize, 1, f);
	fclose(f);
}

void InitGB(void) {
	state.memory = malloc(0x40000);  //64K
}

int main(int argc, char** argv) {
	int done = 0;
	int vblankcycles = 0;
	InitGB();

	ReadFileIntoMemoryAt("Tetris (World) (Rev A).gb", 0);

	//Reset();

	//TEST because wtf is going on
	//unsigned short value1 = 0x0214;
	//signed char value2 = 0x00FC;
	////Expected: 0x310. Result: 0x210
	//uint16_t result = value1 + value2;
	//printf("0x%04", result);

	printf("%c", FLAGS_ISSET(FLAGS_ZERO) ? 'z' : '.');
	printf("%c", FLAGS_ISSET(FLAGS_NEGATIVE) ? 'n' : '.');
	printf("%c", FLAGS_ISSET(FLAGS_HALFCARRY) ? 'h' : '.');
	printf("%c ", FLAGS_ISSET(FLAGS_CARRY) ? 'c' : '.');
	printf("AF $%04x BC $%04x DE $%04x HL $%04x PC %04x SP %04x\n", state.af, state.bc, state.de, state.hl, state.pc, state.sp);

	while (done == 0) {
		done = EmulateGBOp();
	}
	return 0;
}