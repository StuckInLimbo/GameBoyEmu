#include "CPU.h"

#if defined _DEBUG && _WIN32
	#define PRINT_OPS 1
	#define PRINT_DBG 1
#endif

#define FLAGS_ZERO (1 << 7)
#define FLAGS_NEGATIVE (1 << 6)
#define FLAGS_HALFCARRY (1 << 5)
#define FLAGS_CARRY (1 << 4)

int DisassembleGBOp(unsigned char* codebuffer, int pc) {
	unsigned char* code = &codebuffer[pc];
	int opbytes = 1;
	printf("%04x\t", pc);
	switch (*code) {
		//--------------------------x00_x0F-----------------------------//
		switch (*code) {
		case 0x00:									//NOP
			printf("NOP"); break;
		case 0x01:									//LD BC,d16
			printf("LD BC,$%02x%02x", code[2], code[1]); opbytes = 3; break;
		case 0x02:									//LD (BC),A
			printf("LD (BC),A"); break;
		case 0x03:									//INC BC
			printf("INC	BC"); break;
		case 0x04:									//INC B
			printf("INC	B"); break;
		case 0x05:									//DEC B
			printf("DEC	B"); break;
		case 0x06:									//LD B,d8		
			printf("LD B,$%02x", code[1]); opbytes = 2; break;
		case 0x07:									//RLCA
			printf("RLCA"); break;
		case 0x08:									//LD (a16),SP
			printf("LD $%02x%02x,SP", code[2], code[1]); opbytes = 3; break;
		case 0x09:									//ADD HL,BC
			printf("ADD HL,BC"); break;
		case 0x0a:									//LD A,(BC)
			printf("LD A,(BC)"); break;
		case 0x0b:									//DEC BC
			printf("DEC BC"); break;
		case 0x0c:									//INC C
			printf("INC C"); break;
		case 0x0d:									//DEC C
			printf("DEC C"); break;
		case 0x0e:									//LD C,d8
			printf("LD C,d8"); break;
		case 0x0f:									//RRCA
			printf("RRCA"); break;
			//--------------------------x10_x1F-----------------------------//
		case 0x10:									//STOP 0
			printf("STOP 0"); opbytes = 2; break;
		case 0x11:									//LD DE,d16
			printf("LD DE,$%02x%02x", code[2], code[1]); opbytes = 3; break;
		case 0x12:									//LD (DE),A
			printf("LD (DE),A"); break;
		case 0x13:									//INC DE
			printf("INC DE"); break;
		case 0x14:									//INC D
			printf("INC D"); break;
		case 0x15:									//DEC D
			printf("DEC D"); break;
		case 0x16:									//LD H,d8
			printf("LD H,$%02x", code[1]); opbytes = 2; break;
		case 0x17:									//DAA
			printf("DAA"); break;
		case 0x18:									//JR Z,r8
			printf("JR Z,$%02x", code[1]); opbytes = 2; break;
		case 0x19:									//ADD HL,(HL)
			printf("ADD HL,(HL)"); break;
		case 0x1a:									//LD A,(HL+)
			printf("LD A,(HL+)"); break;
		case 0x1b: 									//DEC HL
			printf("DEC HL"); break;
		case 0x1c:									//INC L
			printf("INC L"); break;
		case 0x1d:									//DEC L
			printf("DEC L"); break;
		case 0x1e:									//LD L,d8
			printf("LD L,$%02x", code[1]); opbytes = 2; break;
		case 0x1f:									//CPL
			printf("CPL"); break;
			//--------------------------x20_x2F-----------------------------//
		case 0x20:									//JR NZ,r8
			printf("JR NZ,r8"); opbytes = 2; break;
		case 0x21:									//LD HL,d16
			printf("LD HL,d16"); opbytes = 3; break;
		case 0x22:									//LD (HL+),A
			printf("LD (HL+),A"); break;
		case 0x23: 									//INC HL
			printf("INC HL"); break;
		case 0x24: 									//INC H
			printf("INC H"); break;
		case 0x25: 									//DEC H
			printf("DEC H"); break;
		case 0x26:  								//LD H,d8
			printf("LD H,d8"); opbytes = 2; break;
		case 0x27: 									//DAA
			printf("DAA"); break;
		case 0x28: 									//JR Z,r8
			printf("JR Z,r8"); opbytes = 2; break;
		case 0x29:									//ADD HL,(HL)
			printf("ADD HL,(HL)"); break;
		case 0x2a:									//LD A,(HL+)
			printf("LD A,(HL+)"); break;
		case 0x2b: 									//DEC HL
			printf("DEC HL"); break;
		case 0x2c:									//INC L
			printf("INC L"); break;
		case 0x2d:									//DEC L
			printf("DEC L"); break;
		case 0x2e:									//LD L,d8
			printf("LD L,d8"); opbytes = 2; break;
		case 0x2f:									//CCF
			printf("CCF"); break;
			//--------------------------x30_x3F-----------------------------//
		case 0x30:									//JR NC,r8
			printf("JR NC,r8"); opbytes = 2; break;
		case 0x31:									//LD SP,d16
			printf("LD SP,d16"); opbytes = 3; break;
		case 0x32:									//LD (HL-),A
			printf("LD (HL-),A"); break;
		case 0x33:									//INC SP
			printf("INC SP"); break;
		case 0x34:									//INC (HL)
			printf("INC (HL)"); break;
		case 0x35:									//DEC (HL)
			printf("DEC (HL)"); break;
		case 0x36:  								//LD (HL),d8
			printf("LD (HL),d8"); opbytes = 2; break;
		case 0x37:  								//SCF
			printf("SCF"); break;
		case 0x38:   								//JR C,r8
			printf("JR C,r8"); opbytes = 2; break;
		case 0x39:   								//ADD HL,SP
			printf("ADD HL,SP"); break;
		case 0x3a:									//LD A,(HL-)
			printf("LD A,(HL-)"); break;
		case 0x3b:									//DEC SP
			printf("DEC SP"); break;
		case 0x3c:									//INC A
			printf("INC A"); break;
		case 0x3d:									//DEC A
			printf("DEC A"); break;
		case 0x3e:									//LD A, d8
			printf("LD A, d8"); opbytes = 2; break;
		case 0x3f:									//CCF
			printf("CCF"); break;
			//--------------------------x40_x4F-----------------------------//
		case 0x40:									//LD B,B
			printf("LD B,B"); break;		
		case 0x41:									//LD B,C
			printf("LD B,C"); break;
		case 0x42:									//LD B,D
			printf("LD B,D"); break;
		case 0x43:									//LD B,E 
			printf("LD B,E"); break;
		case 0x44:									//LD B,H
			printf("LD B,H"); break;
		case 0x45:									//LD B,L
			printf("LD B,L"); break;
		case 0x46:									//LD B,(HL)
			printf("LD B,(HL)"); break;
		case 0x47:									//LD B,A
			printf("LD B,A"); break;
		case 0x48:									//LD C,B
			printf("LD C,B"); break;
		case 0x49:									//LD C,C
			printf("LD C,C"); break;
		case 0x4a:									//LD C,D
			printf("LD C,D"); break;
		case 0x4b:									//LD C,E
			printf("LD C,E"); break;
		case 0x4c:									//LD C,H
			printf("LD C,H"); break;
		case 0x4d:									//LD C,L
			printf("LD C,L"); break;
		case 0x4e:									//LD C,(HL)
			printf("LD C,(HL)"); break;
		case 0x4f:									//LD C,A
			printf("LD C,A"); break;
		//--------------------------x50_x5F-----------------------------//
		case 0x50:									//LD D,B
			printf("LD D,B"); break;
		case 0x51:									//LD D,C
			printf("LD D,C"); break;
		case 0x52:									//LD D,D
			printf("LD D,D"); break;
		case 0x53:									//LD D,E
			printf("LD D,E"); break;
		case 0x54:									//LD D,H
			printf("LD D,H"); break;
		case 0x55:									//LD D,L
			printf("LD D,L"); break;
		case 0x56: 									//LD D,(HL)
			printf("LD D,(HL)"); break;
		case 0x57:									//LD D,A
			printf("LD D,A"); break;
		case 0x58:									//LD E,B
			printf("LD E,B"); break;
		case 0x59:									//LD E,C
			printf("LD E,C"); break;
		case 0x5a:									//LD E,D
			printf("LD E,D"); break;
		case 0x5b:									//LD E,E
			printf("LD E,E"); break;
		case 0x5c:									//LD E,H
			printf("LD E,H"); break;
		case 0x5d:									//LD E,L
			printf("LD E,L"); break;
		case 0x5e:									//LD E,(HL)
			printf("LD E,(HL)"); break;
		case 0x5f:									//LD E,A
			printf("LD E,A"); break;
		//--------------------------x60_x6F-----------------------------//
		case 0x60:									//LD H,B
			printf("LD H,B"); break;
		case 0x61:									//LD H,C
			printf("LD H,C"); break;
		case 0x62:									//LD H,D
			printf("LD H,D"); break;
		case 0x63:									//LD H,E
			printf("LD H,E"); break;
		case 0x64:									//LD H,H
			printf("LD H,H"); break;
		case 0x65: 									//LD H,L
			printf("LD H,L"); break;
		case 0x66:									//LD H,(HL)
			printf("LD H,(HL)"); break;
		case 0x67:									//LD H,A
			printf("LD H,A"); break;
		case 0x68:									//LD L,B
			printf("LD L,B"); break;
		case 0x69:									//LD L,C
			printf("LD L,C"); break;
		case 0x6a:									//LD L,D
			printf("LD L,D"); break;
		case 0x6b:									//LD L,E
			printf("LD L,E"); break;
		case 0x6c:									//LD L,H
			printf("LD L,H"); break;
		case 0x6d:									//LD L,L
			printf("LD L,L"); break;
		case 0x6e:									//LD L,(HL)
			printf("LD L,(HL)"); break;
		case 0x6f:									//LD L,A
			printf("LD L,A"); break;
		//--------------------------x70_x7F-----------------------------//
		case 0x70: 									//LD (HL),B
			printf("LD (HL),B"); break;
		case 0x71: 									//LD (HL),C
			printf("LD (HL),C"); break;
		case 0x72:									//LD (HL),D
			printf("LD (HL),D"); break;
		case 0x73:									//LD (HL),E
			printf("LD (HL),E"); break;
		case 0x74: 									//LD (HL),H
			printf("LD (HL),H"); break;
		case 0x75: 									//LD (HL),L
			printf("LD (HL),L"); break;
		case 0x76:									//HLT
			printf(""); break;
		case 0x77: 									//LD (HL),A
			printf("LD (HL),A"); break;
		case 0x78:									//LD A,B
			printf("LD A,B"); break;
		case 0x79:									//LD A,C
			printf("LD A,C"); break;
		case 0x7a:									//LD A,D
			printf("LD A,D"); break;
		case 0x7b:									//LD A,E
			printf("LD A,E"); break;
		case 0x7c:									//LD A,H
			printf("LD A,H"); break;
		case 0x7d: 									//LD A,L
			printf("LD A,L"); break;
		case 0x7e: 									//LD A,(HL)
			printf("LD A,(HL)"); break;
		case 0x7f: 									//LD A,A
			printf("LD A,A"); break;
		//--------------------------x80_x8F-----------------------------//
		case 0x80:									//ADD A,B
			printf("ADD A,B"); break;
		case 0x81:									//ADD A,C
			printf("ADD A,C"); break;
		case 0x82:									//ADD A,D
			printf("ADD A,D"); break;
		case 0x83:									//ADD A,E
			printf("ADD A,E"); break;
		case 0x84:									//ADD A,H
			printf("ADD A,H"); break;
		case 0x85:									//ADD A,L
			printf("ADD A,L"); break;
		case 0x86:									//ADD A,(HL)
			printf("ADD A,(HL)"); break;
		case 0x87:									//ADD A,A
			printf("ADD A,A"); break;
		case 0x88: 									//ADC A,B
			printf("ADC A,B"); break;
		case 0x89: 									//ADC A,C
			printf("ADC A,C"); break;
		case 0x8a: 									//ADC A,D
			printf("ADC A,D"); break;
		case 0x8b: 									//ADC A,E
			printf("ADC A,E"); break;
		case 0x8c: 									//ADC A,H
			printf("ADC A,H"); break;
		case 0x8d: 									//ADC A,L
			printf("ADC A,L"); break;
		case 0x8e: 									//ADC A,(HL)
			printf("ADC A,(HL)"); break;
		case 0x8f: 									//ADC A,A
			printf("ADC A,A"); break;
		//--------------------------x90_x9F-----------------------------//
		case 0x90:  								//SUB B
			printf("SUB B"); break;
		case 0x91:  								//SUB C
			printf("SUB C"); break;
		case 0x92:  								//SUB D
			printf("SUB D"); break;
		case 0x93:  								//SUB E
			printf("SUB E"); break;
		case 0x94:  								//SUB H
			printf("SUB H"); break;
		case 0x95:  								//SUB L
			printf("SUB L"); break;
		case 0x96:  								//SUB (HL)
			printf("SUB (HL)"); break;
		case 0x97:  								//SUB A
			printf("SUB A"); break;
		case 0x98:   								//SBC A,B
			printf("SBC A,B"); break;
		case 0x99:   								//SBC A,C
			printf("SBC A,C"); break;
		case 0x9a:   								//SBC A,D
			printf("SBC A,D"); break;
		case 0x9b:   								//SBC A,E
			printf("SBC A,E"); break;
		case 0x9c:   								//SBC A,H
			printf("SBC A,H"); break;
		case 0x9d:   								//SBC A,L
			printf("SBC A,L"); break;
		case 0x9e:   								//SBC A,(HL)
			printf("SBC A,(HL)"); break;
		case 0x9f:   								//SBC A,A
			printf("SBC A,A"); break;
		//--------------------------xA0_xAF-----------------------------//
		case 0xa0:									//AND B
			printf("AND B"); break;
		case 0xa1:									//AND C
			printf("AND C"); break;
		case 0xa2:									//AND D
			printf("AND D"); break;
		case 0xa3:									//AND E
			printf("AND E"); break;
		case 0xa4:									//AND H
			printf("AND H"); break;
		case 0xa5:									//AND L
			printf("AND L"); break;
		case 0xa6:									//AND (HL)
			printf("AND (HL)"); break;
		case 0xa7:									//AND B
			printf("AND B"); break;
		case 0xA8: 									//XOR B
			printf("XOR B"); break;
		case 0xA9: 									//XOR C
			printf("XOR C"); break;
		case 0xAA: 									//XOR D
			printf("XOR D"); break;
		case 0xAB: 									//XOR E
			printf("XOR E"); break;
		case 0xAC: 									//XOR H
			printf("XOR H"); break;
		case 0xAD: 									//XOR L
			printf("XOR L"); break;
		case 0xAE: 									//XOR (HL)
			printf("XOR (HL)"); break;
		case 0xAF: 									//XOR A
			printf("XOR A"); break;
		//--------------------------xB0_xBF-----------------------------//
		case 0xb0: 									//OR B
			printf("OR B"); break;
		case 0xb1: 									//OR C
			printf("OR C"); break;
		case 0xb2: 									//OR D
			printf("OR D"); break;
		case 0xb3: 									//OR E
			printf("OR E"); break;
		case 0xb4: 									//OR H
			printf("OR H"); break;
		case 0xb5: 									//OR L
			printf("OR L"); break;
		case 0xb6: 									//OR (HL)
			printf("OR (HL)"); break;
		case 0xb7: 									//OR A
			printf("OR A"); break;
		case 0xb8:  								//CP B 
			printf("CP B "); break;
		case 0xb9:  								//CP C 
			printf("CP C "); break;
		case 0xba:  								//CP D 
			printf("CP D "); break;
		case 0xbb:  								//CP E 
			printf("CP E "); break;
		case 0xbc:  								//CP H 
			printf("CP H "); break;
		case 0xbd:  								//CP L 
			printf("CP L "); break;
		case 0xbe:  								//CP (HL) 
			printf("CP (HL) "); break;
		case 0xbf:  								//CP A 
			printf("CP A "); break;
		//--------------------------xC0_xCF-----------------------------//
		case 0xc0:									//RET NZ
			printf("RET NZ"); break;
		case 0xc1:									//POP BC
			printf("POP BC"); break;
		case 0xc2:									//JP NZ,a16
			printf("JP NZ,a16"); opbytes = 3; break;
		case 0xc3: 									//JP a16
			printf("JP a16"); opbytes = 3; break;
		case 0xc4:									//CALL NZ,a16
			printf("CALL NZ,a16"); opbytes = 3; break;
		case 0xc5:									//PUSH BC
			printf("PUSH BC"); break;
		case 0xc6:									//ADD A,d8
			printf("ADD A,d8"); opbytes = 2; break;
		case 0xc7:									//RST 00H
			printf("RST 0x00"); break;
		case 0xc8:									//RET Z
			printf("RET Z"); break;
		case 0xc9: 									//RET
			printf("RET"); break;
		case 0xca: 									//JP Z,a16
			printf("JP Z,a16"); opbytes = 3; break;
		case 0xcb:									//PREFIX CB
			printf("PREFIX CB"); break;
		case 0xcc:									//CALL Z,a16
			printf("CALL Z,a16"); opbytes = 3; break;
		case 0xcd:									//CALL a16
			printf("CALL a16"); opbytes = 3; break;
		case 0xce:									//ADC A,d8
			printf("ADC A,d8"); opbytes = 2; break;
		case 0xcf:									//RST 08H
			printf("RST 0x08"); break;
		//--------------------------xD0_xDF-----------------------------//
		case 0xd0:									//RET NC
			printf("RET NC"); break;
		case 0xd1:									//POP DE
			printf("POP DE"); break;
		case 0xd2:									//JP NC,a16
			printf("JP NC,a16"); break;
		case 0xd3:									//NO INSTRUCTION
			printf("NO INSTRUCTION"); break;
		case 0xd4:									//CALL NC,a16
			printf("CALL NC,a16"); opbytes = 3; break;
		case 0xd5:									//PUSH DE 
			printf("PUSH DE"); break;
		case 0xd6:									//SUB d8
			printf("SUB d8"); opbytes = 2; break;
		case 0xd7:									//RST 10H
			printf("RST 0x10"); break;
		case 0xd8:									//RET C
			printf("RET C"); break;
		case 0xd9:									//RETI
			printf("RETI"); break;
		case 0xda:									//JP C,a16
			printf("JP C,a16"); opbytes = 3; break;
		case 0xdb:									//NO INSTR
			printf("NO INSTRUCTION"); break;
		case 0xdc:									//CALL C,a16
			printf("CALL C,a16"); opbytes = 3; break;
		case 0xdd:									//NO INSTR
			printf("NO INSTRUCTION"); break;
		case 0xde:									//SBC A,d8
			printf("SBC A,d8"); opbytes = 2; break;
		case 0xdf:									//RST 18H
			printf("RST 0x18"); break;
		//--------------------------xE0_xEF-----------------------------//
		case 0xe0:									//LDH (a8),A
			printf("LDH (a8),A"); opbytes = 2; break;
		case 0xe1:									//POP HL
			printf("POP HL"); break;
		case 0xe2:									//LD (C),A
			printf("LD (C),A"); break;
		case 0xe3:									//NO INSTR
			printf("NO INSTRUCTION"); break;
		case 0xe4:									//NO INSTR
			printf("NO INSTRUCTION"); break;
		case 0xe5: 									//PUSH HL
			printf("PUSH HL"); break;
		case 0xe6:									//AND d8
			printf("AND d8"); break;
		case 0xe7:									//RST 20H
			printf("RST 0x20"); break;
		case 0xe8:									//ADD SP,r8
			printf("ADD SP,r8"); opbytes = 2; break;
		case 0xe9:									//JP (HL)
			printf("JP (HL)"); break;
		case 0xea:									//LD (a16),A
			printf("LD (a16),A"); opbytes = 3; break;
		case 0xeb:									//NO INSTR
			printf("NO INSTRUCTION"); break;
		case 0xec: 									//NO INSTR
			printf("NO INSTRUCTION"); break;
		case 0xed:									//NO INSTR
			printf("NO INSTRUCTION"); break;
		case 0xee:									//XOR d8
			printf("XOR d8"); opbytes = 2; break;
		case 0xef:									//RST 28H
			printf("RST 0x28"); break;
		//--------------------------xF0_xFF-----------------------------//
		case 0xf0:									//LDH A,(a8)
			printf("LDH A,(a8)"); opbytes = 2; break;
		case 0xf1:									//POP AF
			printf("POP AF"); break;
		case 0xf2:									//LD A,(C)
			printf("LD A,(C)"); break;
		case 0xf3:									//DI
			printf("DI"); break;
		case 0xf4:									//NO INSTR
			printf("NO INSTRUCTION"); break;
		case 0xf5:									//PUSH AF
			printf("PUSH AF"); break;
		case 0xf6:									//OR d8
			printf("OR d8"); opbytes = 2; break;
		case 0xf7:									//RST 30H
			printf("RST 0x30"); break;
		case 0xf8:									//LD HL,SP+r8
			printf("LD HL,SP+r8"); opbytes = 2; break;
		case 0xf9:  								//LD SP,HL
			printf("LD SP,HL"); break;
		case 0xfa: 									//LD A,(a16)
			printf("LD A,(a16)"); opbytes = 3; break;
		case 0xfb:									//EI
			printf("EI"); break;
		case 0xfc:									//NO INSTR
			printf("NO INSTRUCTION"); break;
		case 0xfd:									//NO INSTR
			printf("NO INSTRUCTION"); break;
		case 0xfe:									//CP d8
			printf("CP d8"); break;
		case 0xFF: 									//RST 38H
			printf("RST 0x38"); break;
		}
		return opbytes;
	}
}

//void ClockDivider() {
//	divide_counter += t;
//	if (divide_counter >= 255)
//	{
//		divide_counter = 0;
//		memory[divider_reg]++;
//	}
//}

uint8_t GetHL(StateGB* state) {
	uint16_t address = ((state->h << 8) | state->l);
	return state->memory[address];
}

void SetHL(StateGB* state, uint8_t value) {
	WriteMem(state, ((state->h << 8) | state->l), value);
}

void Push(StateGB* state, uint8_t high, uint8_t low) {
	WriteMem(state, state->sp - 1, high);
	WriteMem(state, state->sp - 2, low);
	state->sp -= 2;
}

void Pop(StateGB* state, uint8_t* high, uint8_t* low) {
	*low = state->memory[state->sp];
	*high = state->memory[state->sp + 1];
	state->sp += 2;
}

void Add(StateGB* state) {

}

void Inc(StateGB* state, uint8_t* reg) {
	if (((*reg) & 0x0f) == 0x0f)
		state->f.h = 1;
	else
		state->f.h = 0;
	*reg++;
	if (*reg == 0)
		state->f.z = 1;
	else
		state->f.z = 0;
	//Subtract flag reset to 0
	state->f.n = 0;
}

void Dec(StateGB* state, uint8_t* reg) {
	if (((*reg) & 0x0f) == 0x0f)
		state->f.h = 1;
	else
		state->f.h = 0;
	*reg--;
	if (*reg == 0)
		state->f.z = 1;
	else
		state->f.z = 0;
	//Subtract flag reset to 1
	state->f.n = 1;
}

void CheckZero(StateGB* state) {
	if (state->a == 0)
		state->f.z = 1;
	else
		state->f.z = 0;
}

void UnimplementedInstruction(StateGB* state) {
	printf("Error: Unimplemented instruction\n");
	state->pc--;
	DisassembleGBOp(state->memory, state->pc);
	printf("\n");
	exit(1);
}

int EmulateGBOp(StateGB* state) {
	//Get current opcode from memory via CPU state
	unsigned char* opcode = &state->memory[state->pc];

#if PRINT_OPS
	DisassembleGBOp(state->memory, state->pc);
#endif

	state->pc += 1;
#pragma region 0x00 through 0x0F
	switch (*opcode) {
	case 0x00:									//NOP
		break;
	case 0x01:									//LD BC,d16
		state->c = opcode[1];
		state->b = opcode[2];
		state->pc += 2;	//Advance 2 more bytes 
		break;
	case 0x02:									//LD (BC),A
		WriteMem(state, (state->b << 8) | state->c, state->a);
		break;
	case 0x03:									//INC BC
		state->c++;
		if (state->c == 0)
			state->b++;
		break;
	case 0x04:									//INC B
		Inc(state, state->b);
		break;
	case 0x05:									//DEC B
		Dec(state, state->b);
		break;
	case 0x06:									//LD B,d8
		state->b = opcode[1];
		break;
	case 0x07:									//RLCA
		//Rotate A left, old 7 bit to carry
	{
		uint8_t result = state->a;
		state->a = ((result & 0x80) >> 7) | (result << 1);
		state->f.c = (0x80 == (result & 0x80));
		CheckZero(state); //Update Z Flag
		state->f.n = 0; //Reset N flag
		state->f.h = 0; //Reset H Flag
	}
		break;
	case 0x08:									//LD (a16),SP
		//Stores the lower byte of SP at address nn
		UnimplementedInstruction(state);
		break;
	case 0x09:									//ADD HL,BC
		break;
	case 0x0a:									//LD A,(BC)
		break;
	case 0x0b:									//DEC BC
		state->c--;
		if (state->c == 0xFF)
			state->b--;
		break;
	case 0x0c:									//INC C
		Inc(state, state->c);
		break;
	case 0x0d:									//DEC C
		Dec(state, state->l);
		break;
	case 0x0e:									//LD C,d8
		break;
	case 0x0f:									//RRCA
		break;
#pragma endregion
#pragma region 0x10 through 0x1F
	case 0x10:									//STOP 0
		break;
	case 0x11:									//LD DE,d16
		break;
	case 0x12:									//LD (DE),A
		break;
	case 0x13:									//INC DE
		state->e++;
		if (state->e == 0)
			state->d++;
		break;
	case 0x14:									//INC D
		Inc(state, state->d);
		break;
	case 0x15:									//DEC D
		Dec(state, state->d);
		break;
	case 0x16:									//LD H,d8
		break;
	case 0x17:									//DAA
		break;
	case 0x18:									//JR Z,r8
		break;
	case 0x19:									//ADD HL,(HL)
		break;
	case 0x1a:									//LD A,(HL+)
		break;
	case 0x1b: 									//DEC DE
		state->e--;
		if (state->e == 0xFF)
			state->d--;
		break;
	case 0x1c:									//INC L
		Inc(state, state->l);
		break;
	case 0x1d:									//DEC L
		Dec(state, state->l);
		break;
	case 0x1e:									//LD L,d8
		break;
	case 0x1f:									//CPL
		break;
#pragma endregion
#pragma region 0x20 through 0x2F
	case 0x20:									//JR NZ,r8
		break;
	case 0x21:									//LD HL,d16
		break;
	case 0x22:									//LD (HL+),A
		break;
	case 0x23: 									//INC HL
		state->l++;
		if (state->l == 0)
			state->h++;
		break;
	case 0x24: 									//INC H
		Inc(state, state->h);
		break;
	case 0x25: 									//DEC H
		Dec(state, state->h);
		break;
	case 0x26:  								//LD H,d8
		break;
	case 0x27: 									//DAA
		break;
	case 0x28: 									//JR Z,r8
		break;
	case 0x29:									//ADD HL,(HL)
		break;
	case 0x2a:									//LD A,(HL+)
		break;
	case 0x2b: 									//DEC HL
		state->l--;
		if (state->l == 0xFF)
			state->h--;
		break;
	case 0x2c:									//INC L
		Inc(state, state->l);
		break;
	case 0x2d:									//DEC L
		Dec(state, state->l);
		break;
	case 0x2e:									//LD L,d8
		break;
	case 0x2f:									//CPL
		break;
#pragma endregion
#pragma region 0x30 through 0x3F
	case 0x30:									//JR NC,r8
		break;
	case 0x31:									//LD SP,d16
		break;
	case 0x32:									//LD (HL-),A
		break;
	case 0x33:									//INC SP
		break;
	case 0x34:									//INC (HL)
		break;
	case 0x35:									//DEC (HL)
		break;
	case 0x36:  								//LD (HL),d8
		break;
	case 0x37:  								//SCF
		break;
	case 0x38:   								//JR C,r8
		break;
	case 0x39:   								//ADD HL,SP
		break;
	case 0x3a:									//LD A,(HL-)
		break;
	case 0x3b:									//DEC SP
		break;
	case 0x3c:									//INC A
		break;
	case 0x3d:									//DED A
		break;
	case 0x3e:									//LD A, d8
		break;
	case 0x3f:									//CCF
		break;
#pragma endregion
#pragma region 0x40 through 0x4F
	case 0x40: state->b = state->b; break;		//LD B,B
	case 0x41: state->b = state->c; break;		//LD B,C
	case 0x42: state->b = state->d; break;		//LD B,D    
	case 0x43: state->b = state->e; break;		//LD B,E 
	case 0x44: state->b = state->h; break;		//LD B,H
	case 0x45: state->b = state->l; break;		//LD B,L
	case 0x46: state->b = GetHL(state); break;	//LD B,(HL)
	case 0x47: state->b = state->a; break;		//LD B,A
	case 0x48: state->c = state->b; break;		//LD C,B
	case 0x49: state->c = state->c; break;		//LD C,C
	case 0x4a: state->c = state->d; break;		//LD C,D
	case 0x4b: state->c = state->e; break;		//LD C,E
	case 0x4c: state->c = state->h; break;		//LD C,H
	case 0x4d: state->c = state->l; break;		//LD C,L
	case 0x4e: state->c = GetHL(state); break;	//LD C,(HL)
	case 0x4f: state->c = state->a; break;		//LD C,A
#pragma endregion
#pragma region 0x50 through 0x5F
	case 0x50: state->d = state->b; break;		//LD D,B
	case 0x51: state->d = state->c; break;		//LD D,C
	case 0x52: state->d = state->d; break;		//LD D,D
	case 0x53: state->d = state->e; break;		//LD D,E
	case 0x54: state->d = state->h; break;		//LD D,H
	case 0x55: state->d = state->l; break;		//LD D,L
	case 0x56: state->d = GetHL(state); break;	//LD D,(HL)
	case 0x57: state->d = state->a; break;		//LD D,A
	case 0x58: state->e = state->b; break;		//LD E,B
	case 0x59: state->e = state->c; break;		//LD E,C
	case 0x5a: state->e = state->d; break;		//LD E,D
	case 0x5b: state->e = state->e; break;		//LD E,E
	case 0x5c: state->e = state->h; break;		//LD E,H
	case 0x5d: state->e = state->l; break;		//LD E,L
	case 0x5e: state->e = GetHL(state); break;	//LD E,(HL)	
	case 0x5f: state->e = state->a; break;		//LD E,A
#pragma endregion
#pragma region 0x60 through 0x6F
	case 0x60: state->h = state->b; break;		//LD H,B
	case 0x61: state->h = state->c; break;		//LD H,C
	case 0x62: state->h = state->d; break;		//LD H,D
	case 0x63: state->h = state->e; break;		//LD H,E
	case 0x64: state->h = state->h; break;		//LD H,H
	case 0x65: state->h = state->l; break;		//LD H,L
	case 0x66: state->h = GetHL(state); break;	//LD H,(HL)
	case 0x67: state->h = state->a; break;		//LD H,A
	case 0x68: state->l = state->b; break;		//LD L,B
	case 0x69: state->l = state->c; break;		//LD L,C
	case 0x6a: state->l = state->d; break;		//LD L,D
	case 0x6b: state->l = state->e; break;		//LD L,E
	case 0x6c: state->l = state->h; break;		//LD L,H
	case 0x6d: state->l = state->l; break;		//LD L,L
	case 0x6e: state->l = GetHL(state); break;	//LD L,(HL)
	case 0x6f: state->l = state->a; break;		//LD L,A
#pragma endregion
#pragma region 0x70 through 0x7F
	case 0x70: SetHL(state, state->b); break;	//LD (HL),B
	case 0x71: SetHL(state, state->c); break;	//LD (HL),C
	case 0x72: SetHL(state, state->d); break;	//LD (HL),D
	case 0x73: SetHL(state, state->e); break;	//LD (HL),E
	case 0x74: SetHL(state, state->h); break;	//LD (HL),H
	case 0x75: SetHL(state, state->l); break;	//LD (HL),L
	case 0x76:									//HLT
		break;
	case 0x77: SetHL(state, state->a); break;	//LD (HL),A
	case 0x78: state->a = state->b; break;		//LD A,B
	case 0x79: state->a = state->c; break;		//LD A,C
	case 0x7a: state->a = state->d; break;		//LD A,D
	case 0x7b: state->a = state->e; break;		//LD A,E
	case 0x7c: state->a = state->h; break;		//LD A,H
	case 0x7d: state->a = state->l; break;		//LD A,L
	case 0x7e: state->a = GetHL(state); break;	//LD A,(HL)
	case 0x7f: state->a = state->a; break;		//LD A,A
#pragma endregion
#pragma region 0x80 through 0x8F
	case 0x80:									//ADD A,B
	{
		uint16_t answer = (uint16_t)state->a + (uint16_t)state->b;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	break;
	case 0x81:									//ADD A,C
	{
		uint16_t answer = (uint16_t)state->a + (uint16_t)state->c;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x82:									//ADD A,D
	{
		uint16_t answer = (uint16_t)state->a + (uint16_t)state->d;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x83:									//ADD A,E
	{
		uint16_t answer = (uint16_t)state->a + (uint16_t)state->e;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x84:									//ADD A,H
	{
		uint16_t answer = (uint16_t)state->a + (uint16_t)state->h;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x85:									//ADD A,L
	{
		uint16_t answer = (uint16_t)state->a + (uint16_t)state->l;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x86:									//ADD A,(HL)
	{
		uint16_t answer = (uint16_t)state->a + GetHL(state);
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x87:									//ADD A,A
	{
		uint16_t answer = (uint16_t)state->a + (uint16_t)state->a;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x88: 									//ADC A,B
	{
		uint16_t answer = (uint16_t)state->a + (uint16_t)state->b + state->f.c;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x89: 									//ADC A,C
	{
		uint16_t answer = (uint16_t)state->a + (uint16_t)state->c + state->f.c;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x8a: 									//ADC A,D
	{
		uint16_t answer = (uint16_t)state->a + (uint16_t)state->d + state->f.c;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x8b: 									//ADC A,E
	{
		uint16_t answer = (uint16_t)state->a + (uint16_t)state->e + state->f.c;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x8c: 									//ADC A,H
	{
		uint16_t answer = (uint16_t)state->a + (uint16_t)state->h + state->f.c;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x8d: 									//ADC A,L
	{
		uint16_t answer = (uint16_t)state->a + (uint16_t)state->l + state->f.c;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x8e: 									//ADC A,(HL)
	{
		uint16_t answer = (uint16_t)state->a + GetHL(state) + state->f.c;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x8f: 									//ADC A,A
	{
		uint16_t answer = (uint16_t)state->a + (uint16_t)state->a + state->f.c;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
#pragma endregion
#pragma region 0x90 through 0x9F
	case 0x90:  								//SUB B
	{
		uint16_t answer = (uint16_t)state->a - (uint16_t)state->b;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x91:  								//SUB C
	{
		uint16_t answer = (uint16_t)state->a - (uint16_t)state->c;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x92:  								//SUB D
	{
		uint16_t answer = (uint16_t)state->a - (uint16_t)state->d;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x93:  								//SUB E
	{
		uint16_t answer = (uint16_t)state->a - (uint16_t)state->e;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x94:  								//SUB H
	{
		uint16_t answer = (uint16_t)state->a - (uint16_t)state->h;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x95:  								//SUB L
	{
		uint16_t answer = (uint16_t)state->a - (uint16_t)state->l;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x96:  								//SUB (HL)
	{
		uint16_t answer = (uint16_t)state->a - GetHL(state);
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x97:  								//SUB A
	{
		uint16_t answer = (uint16_t)state->a - (uint16_t)state->a;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x98:   								//SBC A,B
	{
		uint16_t answer = (uint16_t)state->a - (uint16_t)state->b - state->f.c;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x99:   								//SBC A,C
	{
		uint16_t answer = (uint16_t)state->a - (uint16_t)state->c - state->f.c;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x9a:   								//SBC A,D
	{
		uint16_t answer = (uint16_t)state->a - (uint16_t)state->d - state->f.c;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x9b:   								//SBC A,E
	{
		uint16_t answer = (uint16_t)state->a - (uint16_t)state->e - state->f.c;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x9c:   								//SBC A,H
	{
		uint16_t answer = (uint16_t)state->a - (uint16_t)state->h - state->f.c;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x9d:   								//SBC A,L
	{
		uint16_t answer = (uint16_t)state->a - (uint16_t)state->l - state->f.c;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x9e:   								//SBC A,(HL)
	{
		uint16_t answer = (uint16_t)state->a - GetHL(state) - state->f.c;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
	case 0x9f:   								//SBC A,A
	{
		uint16_t answer = (uint16_t)state->a - (uint16_t)state->a - state->f.c;
		MathFlags(state, answer);
		state->a = answer & 0xFF;
	} break;
#pragma endregion
#pragma region 0xA0 through 0xAF
	case 0xa0:									//AND B
		state->a = state->a & state->b;
		LogicFlags(state); break;
	case 0xa1:									//AND C
		state->a = state->a & state->c;
		LogicFlags(state); break;
	case 0xa2:									//AND D
		state->a = state->a & state->d;
		LogicFlags(state); break;
	case 0xa3:									//AND E
		state->a = state->a & state->e;
		LogicFlags(state); break;
	case 0xa4:									//AND H
		state->a = state->a & state->h;
		LogicFlags(state); break;
	case 0xa5:									//AND L
		state->a = state->a & state->l;
		LogicFlags(state); break;
	case 0xa6:									//AND (HL)
		state->a = state->a & GetHL(state);
		LogicFlags(state); break;
	case 0xa7:									//AND B
		state->a = state->a & state->a;
		LogicFlags(state); break;
	case 0xA8: 									//XOR B
		state->a = state->a ^ state->b;
		LogicFlags(state); break;
	case 0xA9: 									//XOR C
		state->a = state->a ^ state->c;
		LogicFlags(state); break;
	case 0xAA: 									//XOR D
		state->a = state->a ^ state->d;
		LogicFlags(state); break;
	case 0xAB: 									//XOR E
		state->a = state->a ^ state->e;
		LogicFlags(state); break;
	case 0xAC: 									//XOR H
		state->a = state->a ^ state->h;
		LogicFlags(state); break;
	case 0xAD: 									//XOR L
		state->a = state->a ^ state->l;
		LogicFlags(state); break;;
	case 0xAE: 									//XOR (HL)
		state->a = state->a ^ GetHL(state);
		LogicFlags(state); break;
	case 0xAF: 									//XOR A
		state->a = state->a ^ state->a;
		LogicFlags(state); break;
#pragma endregion
#pragma region 0xB0 through 0xBF
	case 0xb0: 									//OR B
		state->a = state->a | state->b;
		LogicFlags(state); break;
	case 0xb1: 									//OR C
		state->a = state->a | state->c;
		LogicFlags(state); break;
	case 0xb2: 									//OR D
		state->a = state->a | state->d;
		LogicFlags(state); break;
	case 0xb3: 									//OR E
		state->a = state->a | state->e;
		LogicFlags(state); break;
	case 0xb4: 									//OR H
		state->a = state->a | state->h;
		LogicFlags(state); break;
	case 0xb5: 									//OR L
		state->a = state->a | state->l;
		LogicFlags(state); break;
	case 0xb6: 									//OR (HL)
		state->a = state->a | GetHL(state);
		LogicFlags(state); break;
	case 0xb7: 									//OR A
		state->a = state->a | state->a;
		LogicFlags(state); break;
	case 0xb8:  								//CP B 
	{
		uint16_t answer = (uint16_t)state->a - (uint16_t)state->b;
		MathFlags(state, answer);
	} break;
	case 0xb9:  								//CP C 
	{
		uint16_t answer = (uint16_t)state->a - (uint16_t)state->c;
		MathFlags(state, answer);
	} break;
	case 0xba:  								//CP D 
	{
		uint16_t answer = (uint16_t)state->a - (uint16_t)state->d;
		MathFlags(state, answer);
	} break;
	case 0xbb:  								//CP E 
	{
		uint16_t answer = (uint16_t)state->a - (uint16_t)state->e;
		MathFlags(state, answer);
	} break;
	case 0xbc:  								//CP H 
	{
		uint16_t answer = (uint16_t)state->a - (uint16_t)state->h;
		MathFlags(state, answer);
	} break;
	case 0xbd:  								//CP L 
	{
		uint16_t answer = (uint16_t)state->a - (uint16_t)state->l;
		MathFlags(state, answer);
	} break;
	case 0xbe:  								//CP (HL) 
	{
		uint16_t answer = (uint16_t)state->a - GetHL(state);
		MathFlags(state, answer);
	} break;
	case 0xbf:  								//CP A 
	{
		uint16_t answer = (uint16_t)state->a - (uint16_t)state->a;
		MathFlags(state, answer);
	} break;
#pragma endregion
#pragma region 0xC0 through CxCF
	case 0xc0:									//RET NZ
		if (state->f.z == 0) {
			state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
			state->sp += 2;
		}
		break;
	case 0xc1:									//POP BC
		Pop(state, &state->b, &state->c);
		break;
	case 0xc2:									//JP NZ,a16
		if (0 == state->f.z)
			state->pc = (opcode[2] << 8) | opcode[1];
		else //branch not taken
			state->pc += 2;
		break;
	case 0xc3: 									//JP a16
		state->pc = (opcode[2] << 8) | opcode[1];
		break;
	case 0xc4:									//CALL NZ,a16
	{
		if (state->f.z == 0) {
			uint16_t ret = state->pc + 2;
			WriteMem(state, state->sp - 1, (ret >> 8) & 0xFF);
			WriteMem(state, state->sp - 2, (ret & 0xFF));
			state->sp = state->sp - 2;
			state->pc = (opcode[2] << 8) | opcode[1];
		}
		else
			state->pc += 2;
	} break;
	case 0xc5:									//PUSH BC
		Push(state, state->b, state->c);
		break;
	case 0xc6:									//ADD A,d8
	{
		uint16_t answer = (uint16_t)state->a + (uint16_t)opcode[1];
		ZNHFlags(state, answer & 0xFF);
		state->f.c = (answer > 0xFF);
		state->a = answer & 0xFF;
		state->pc++;
	} break;
	case 0xc7:									//RST 00H
	{
		uint16_t ret = state->pc + 2;
		WriteMem(state, state->sp - 1, (ret >> 8) & 0xFF);
		WriteMem(state, state->sp - 2, (ret & 0xFF));
		state->sp = state->sp - 2;
		state->pc = 0x0000;
	} break;
	case 0xc8:									//RET Z
		if (state->f.z) {
			state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
			state->sp += 2;
		}
		break;
	case 0xc9: 									//RET
		state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
		state->sp += 2;
		break;
	case 0xca: 									//JP Z,a16
		if (state->f.z)
			state->pc = (opcode[2] << 8) | opcode[1];
		else
			state->pc += 2;
		break;
	case 0xcb:									//PREFIX CB
		break;
	case 0xcc:									//CALL Z,a16
	{
		if (state->f.z == 1) {
			uint16_t ret = state->pc + 2;
			WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
			WriteMem(state, state->sp - 2, (ret & 0xff));
			state->sp = state->sp - 2;
			state->pc = (opcode[2] << 8) | opcode[1];
		}
		else
			state->pc += 2;
	} break;
	case 0xcd:									//CALL a16
	{
		uint16_t ret = state->pc + 2;
		WriteMem(state, state->sp - 1, (ret >> 8) & 0xFF);
		WriteMem(state, state->sp - 2, (ret & 0xFF));
		state->sp = state->sp - 2;
		state->pc = (opcode[2] << 8) | opcode[1];
	} break;
	case 0xce:									//ADC A,d8
	{
		uint16_t x = state->a + opcode[1] + state->f.c;
		ZNHFlags(state, x & 0xFF);
		state->f.c = (x > 0xFF);
		state->a = x & 0xFF;
		state->pc++;
	} break;
	case 0xcf:									//RST 08H
	{
		uint16_t ret = state->pc + 2;
		WriteMem(state, state->sp - 1, (ret >> 8) & 0xFF);
		WriteMem(state, state->sp - 2, ret & 0xFF);
		state->sp = state->sp - 2;
		state->pc = 0x0008;
	} break;
#pragma endregion
#pragma region 0xD0 through 0xDF
	case 0xd0:									//RET NC
		if (state->f.c == 0) {
			state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
			state->sp += 2;
		}
		break;
	case 0xd1:									//POP DE
		Pop(state, &state->d, &state->e);
		break;
	case 0xd2:									//JP NC,a16
		if (state->f.c == 0)
			state->pc = (opcode[2] << 8) | opcode[1];
		else
			state->pc += 2;
		break;
	case 0xd3:									//NO INSTRUCTION
		UnimplementedInstruction(state);
		break;
	case 0xd4:									//CALL NC,a16
	{
		if (state->f.c == 0) {
			uint16_t ret = state->pc + 2;
			WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
			WriteMem(state, state->sp - 2, (ret & 0xff));
			state->sp = state->sp - 2;
			state->pc = (opcode[2] << 8) | opcode[1];
		}
		else
			state->pc += 2;
	} break;
	case 0xd5:									//PUSH DE 
		Push(state, state->d, state->e);
		break;
	case 0xd6:									//SUB d8
	{
		uint8_t x = state->a - opcode[1];
		ZNHFlags(state, x & 0xff);
		state->f.c = (state->a < opcode[1]);
		state->a = x;
		state->pc++;
	} break;
	case 0xd7:									//RST 10H
	{
		uint16_t ret = state->pc + 2;
		WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
		WriteMem(state, state->sp - 2, (ret & 0xff));
		state->sp = state->sp - 2;
		state->pc = 0x10;
	} break;
	case 0xd8:									//RET C
		if (state->f.c != 0) {
			state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
			state->sp += 2;
		}
		break;
	case 0xd9:									//RETI
		break;
	case 0xda:									//JP C,a16
		if (state->f.c != 0)
			state->pc = (opcode[2] << 8) | opcode[1];
		else
			state->pc += 2;
		break;
	case 0xdb:									//NO INSTR
		UnimplementedInstruction(state);
		break;
	case 0xdc:									//CALL C,a16
	{
		if (state->f.c != 0) {
			uint16_t ret = state->pc + 2;
			WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
			WriteMem(state, state->sp - 2, (ret & 0xff));
			state->sp = state->sp - 2;
			state->pc = (opcode[2] << 8) | opcode[1];
		}
		else
			state->pc += 2;
	} break;
	case 0xdd:									//NO INSTR
		UnimplementedInstruction(state);
		break;
	case 0xde:									//SBI byte
	{
		uint16_t x = state->a - opcode[1] - state->f.c;
		ZNHFlags(state, x & 0xff);
		state->f.c = (x > 0xFF);
		state->a = x & 0xFF;
		state->pc++;
	} break;
	case 0xdf:									//RST 3
	{
		uint16_t ret = state->pc + 2;
		WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
		WriteMem(state, state->sp - 2, (ret & 0xff));
		state->sp = state->sp - 2;
		state->pc = 0x18;
	} break;
#pragma endregion
#pragma region 0xE0 through 0xEF
	case 0xe0:									//RPO
		if (state->f.h == 0) {
			state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
			state->sp += 2;
		}
		break;
	case 0xe1:									//POP H
		Pop(state, &state->h, &state->l);
		break;
	case 0xe2:									//JPO
		if (state->f.h == 0)
			state->pc = (opcode[2] << 8) | opcode[1];
		else
			state->pc += 2;
		break;
	case 0xe3:									//NO INSTR
		UnimplementedInstruction(state);
		break;
	case 0xe4:									//NO INSTR
		UnimplementedInstruction(state);
		break;
	case 0xe5: 									//PUSH H
		Push(state, state->h, state->l);
		break;
	case 0xe6:									//ANI byte
		state->a = state->a & opcode[1];
		LogicFlags(state);
		state->pc++;
		break;
	case 0xe7:									//RST 4
	{
		uint16_t ret = state->pc + 2;
		WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
		WriteMem(state, state->sp - 2, (ret & 0xff));
		state->sp = state->sp - 2;
		state->pc = 0x20;
	} break;
	case 0xe8:									//RPE
		if (state->f.h != 0) {
			state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
			state->sp += 2;
		}
		break;
	case 0xe9:									//PCHL
		state->pc = (state->h << 8) | state->l;
		break;
	case 0xea:									//JPE
		if (state->f.h != 0)
			state->pc = (opcode[2] << 8) | opcode[1];
		else
			state->pc += 2;
		break;
	case 0xeb:									//NO INSTR
		break;
	case 0xec: 									//NO INSTR
		break;
	case 0xed:									//NO INSTR
		break;
	case 0xee:									//XRI byte
	{
		uint8_t x = state->a ^ opcode[1];
		ZNHFlags(state, x);
		state->f.c = 0;
		state->a = x;
		state->pc++;
	} break;
	case 0xef:									//RST 5
	{
		uint16_t ret = state->pc + 2;
		WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
		WriteMem(state, state->sp - 2, (ret & 0xff));
		state->sp = state->sp - 2;
		state->pc = 0x28;
	} break;
#pragma endregion
#pragma region 0xF0 through 0xFF
	case 0xf0:									//RP
		if (state->f.n == 0) {
			state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
			state->sp += 2;
		}
		break;
	case 0xf1:									//POP PSW
		Pop(state, &state->a, (unsigned char*)&state->f);
		break;
	case 0xf2:									//JP addr
		if (state->f.n == 0)
			state->pc = (opcode[2] << 8) | opcode[1];
		else
			state->pc += 2;
		break;
	case 0xf3: state->interrupts = 0; break;	//DI
	case 0xf4:									//NO INSTR
		break;
	case 0xf5:									//PUSH PSW
		Push(state, state->a, *(unsigned char*)&state->f);
		break;
	case 0xf6:									//ORI byte
	{
		uint8_t x = state->a | opcode[1];
		ZNHFlags(state, x);
		state->f.c = 0;
		state->a = x;
		state->pc++;
	} break;
	case 0xf7:									//RST 6
	{
		uint16_t ret = state->pc + 2;
		WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
		WriteMem(state, state->sp - 2, (ret & 0xff));
		state->sp = state->sp - 2;
		state->pc = 0x30;
	} break;
	case 0xf8:									//RM
		if (state->f.n != 0) {
			state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
			state->sp += 2;
		}
		break;
	case 0xf9:  								//SPHL
		state->sp = state->l | (state->h << 8);
		break;
	case 0xfa: 									//JM
		if (state->f.n != 0)
			state->pc = (opcode[2] << 8) | opcode[1];
		else
			state->pc += 2;
		break;
	case 0xfb: state->interrupts = 1;  break;	//EI
	case 0xfc:									//NO INSTR
		break;
	case 0xfd:									//NO INSTR
		break;
	case 0xfe:									//CPI byte
	{
		uint8_t x = state->a - opcode[1];
		ZNHFlags(state, x);
		state->f.c = (state->a < opcode[1]);
		state->pc++;
	} break;
	case 0xFF: 									//RST 6
	{
		uint16_t ret = state->pc + 2;
		WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
		WriteMem(state, state->sp - 2, (ret & 0xff));
		state->sp = state->sp - 2;
		state->pc = 0x38;
	} break;
#pragma endregion
	default:   UnimplementedInstruction(state); break;
	}
}