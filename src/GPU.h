#pragma once
#include "CPU.h"
#include "Memory.h"

struct GPU {
	// values in the pixels
	int pixels[160][144][3];

	// value in the clock of the gpu
	int clock;

	// the mode in which the gpu is
	// 0 - horizontal blank
	// 1 - vertical blank
	// 2 - scanline (accessing oam)
	// 3 - scanline (accessing vram)
	int mode;

	// line number being drawn
	int line;


};