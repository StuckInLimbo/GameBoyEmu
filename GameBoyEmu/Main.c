#pragma once
#include "Memory.h"
#include "CPU.h"

void ReadFileIntoMemoryAt(StateGB* state, char* filename, uint32_t offset) {
	FILE* f; 
	fopen_s(&f, filename, "rb");
	if (f == NULL) {
		printf("error: Couldn't open %s\n", filename);
		exit(1);
	}
	fseek(f, 0L, SEEK_END);
	int fsize = ftell(f);
	fseek(f, 0L, SEEK_SET);

	uint8_t* buffer = &state->memory[offset];
	fread(buffer, fsize, 1, f);
	fclose(f);
}

StateGB* InitGB(void) {
	StateGB* state = calloc(1, sizeof(StateGB));
	state->memory = malloc(0x40000);  //64K
	return state;
}

int main(int argc, char** argv) {
	int done = 0;
	int vblankcycles = 0;
	StateGB* state = InitGB();

	ReadFileIntoMemoryAt(state, "Tetris.gb", 0);
	
	while (done == 0) {
		done = EmulateGBOp(state);
	}
	return 0;
}