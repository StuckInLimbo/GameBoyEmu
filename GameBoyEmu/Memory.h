#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

//typedef struct ConditionCodes {
//	uint8_t		z : 1;	//Zero
//	uint8_t		n : 1;	//Subtract
//	uint8_t		h : 1;	//Half Carry
//	uint8_t		c : 1;	//Carry
//	uint8_t		pad : 4;
//} ConditionCodes;

/* The Flag Register consists of the following bits:
* 7 6 5 4 3 2 1 0
* Z N H C 0 0 0 0
* 
* Zero Flag(Z):
*	This bit is set when the result of a math operation is zero or 
*	two values match when using the CP instruction.
*
* Subtract Flag(N):
*	This bit is set if a subtraction was performed in the last math instruction.
*
* Half Carry Flag(H):
*	This bit is set if a carry occurred from the lower nibble in the last math operation.
*
* Carry Flag(C):
*	This bit is set if a carry occurred from the last math operation or if 
*	register A is the smaller value when executing the CP instruction
*/

struct StateGB {
	union {
		struct {
			uint8_t		f;	//REGISTER F
			uint8_t		a;	//REGISTER A
		};
		uint16_t	af;		//REGISTER AF
	};
	union {
		struct {
			uint8_t		c;	//REGISTER C
			uint8_t		b;	//REGISTER B
		};
		uint16_t	bc;		//REGISTER BC
	};
	union {
		struct {
			uint8_t		e;	//REGISTER E
			uint8_t		d;	//REGISTER D
		};
		uint16_t	de;		//REGISTER DE
	};
	union {
		struct {
			uint8_t		l;	//REGISTER L
			uint8_t		h;	//REGISTER H
		};
		uint16_t	hl;		//REGISTER HL
	};
	uint16_t	sp; //STACK POINTER
	uint16_t	pc; //PROGRAM COUNTER
	uint8_t* memory; //Memory Ptr
	uint8_t		interrupts; //Enable/Disable Interrupts
	uint8_t		time; //Cycles since last instruction
} extern state;

uint16_t ReadShort(uint16_t address);
uint16_t ReadShortFromStack();
uint8_t ReadMem(uint16_t address);
void WriteMem(uint16_t address, uint8_t value);
void WriteMem16(uint16_t address, uint16_t value);
void WriteShort(uint16_t address, uint16_t value);
void WriteShortToStack(uint16_t value);
