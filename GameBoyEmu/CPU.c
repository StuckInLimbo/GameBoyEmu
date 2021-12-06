#if defined _DEBUG && _WIN32
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
#endif
#define PRINT_OPS 1
#define PRINT_DBG 1
#include "CPU.h"

//void ClockDivider() {
//	divide_counter += t;
//	if (divide_counter >= 255)
//	{
//		divide_counter = 0;
//		memory[divider_reg]++;
//	}
//}

unsigned long ticks;
unsigned char stopped;

void Reset(void) {
	memset(state.memory, 0, sizeof(0x400000));

	state.a = 0x01;
	state.f = 0xb0;
	state.b = 0x00;
	state.c = 0x13;
	state.d = 0x00;
	state.e = 0xd8;
	state.h = 0x01;
	state.l = 0x4d;
	state.sp = 0xfffe;
	state.pc = 0x100;

	state.interrupts = 1;
	//interrupt.enable = 0;
	//interrupt.flags = 0;

	/*keys.a = 1;
	keys.b = 1;
	keys.select = 1;
	keys.start = 1;
	keys.right = 1;
	keys.left = 1;
	keys.up = 1;
	keys.down = 1;*/

	//memset(tiles, 0, sizeof(tiles));

	/*backgroundPalette[0] = palette[0];
	backgroundPalette[1] = palette[1];
	backgroundPalette[2] = palette[2];
	backgroundPalette[3] = palette[3];

	spritePalette[0][0] = palette[0];
	spritePalette[0][1] = palette[1];
	spritePalette[0][2] = palette[2];
	spritePalette[0][3] = palette[3];

	spritePalette[1][0] = palette[0];
	spritePalette[1][1] = palette[1];
	spritePalette[1][2] = palette[2];
	spritePalette[1][3] = palette[3];*/

	/*gpu.control = 0;
	gpu.scrollX = 0;
	gpu.scrollY = 0;
	gpu.scanline = 0;
	gpu.tick = 0;*/

	ticks = 0;
	stopped = 0;

#ifdef _WIN32
	//memset(framebuffer, 255, sizeof(framebuffer));
#endif

	WriteMem(0xFF05, 0);
	WriteMem(0xFF06, 0);
	WriteMem(0xFF07, 0);
	WriteMem(0xFF10, 0x80);
	WriteMem(0xFF11, 0xBF);
	WriteMem(0xFF12, 0xF3);
	WriteMem(0xFF14, 0xBF);
	WriteMem(0xFF16, 0x3F);
	WriteMem(0xFF17, 0x00);
	WriteMem(0xFF19, 0xBF);
	WriteMem(0xFF1A, 0x7A);
	WriteMem(0xFF1B, 0xFF);
	WriteMem(0xFF1C, 0x9F);
	WriteMem(0xFF1E, 0xBF);
	WriteMem(0xFF20, 0xFF);
	WriteMem(0xFF21, 0x00);
	WriteMem(0xFF22, 0x00);
	WriteMem(0xFF23, 0xBF);
	WriteMem(0xFF24, 0x77);
	WriteMem(0xFF25, 0xF3);
	WriteMem(0xFF26, 0xF1);
	WriteMem(0xFF40, 0x91);
	WriteMem(0xFF42, 0x00);
	WriteMem(0xFF43, 0x00);
	WriteMem(0xFF45, 0x00);
	WriteMem(0xFF47, 0xFC);
	WriteMem(0xFF48, 0xFF);
	WriteMem(0xFF49, 0xFF);
	WriteMem(0xFF4A, 0x00);
	WriteMem(0xFF4B, 0x00);
	WriteMem(0xFFFF, 0x00);
}

void printRegisters(void) {
	printf("A: 0x%02x\n", state.a);
	printf("F: 0x%02x\n", state.f);
	printf("B: 0x%02x\n", state.b);
	printf("C: 0x%02x\n", state.c);
	printf("D: 0x%02x\n", state.d);
	printf("E: 0x%02x\n", state.e);
	printf("H: 0x%02x\n", state.h);
	printf("L: 0x%02x\n", state.l);
	printf("SP: 0x%04x\n", state.sp);
	printf("PC: 0x%04x\n", state.pc);
	/*printf("IME: 0x%02x\n", interrupt.master);
	printf("IE: 0x%02x\n", interrupt.enable);
	printf("IF: 0x%02x\n", interrupt.flags);*/
}

const struct Instruction instructions[256] = {
	{ "NOP", 0, nop },                           // 0x00
	{ "LD BC, 0x%04X", 2, ld_bc_nn },            // 0x01
	{ "LD (BC), A", 0, ld_bcp_a },               // 0x02
	{ "INC BC", 0, inc_bc },                     // 0x03
	{ "INC B", 0, inc_b },                       // 0x04
	{ "DEC B", 0, dec_b },                       // 0x05
	{ "LD B, 0x%02X", 1, ld_b_n },               // 0x06
	{ "RLCA", 0, rlca },                         // 0x07
	{ "LD (0x%04X), SP", 2, ld_nnp_sp },         // 0x08
	{ "ADD HL, BC", 0, add_hl_bc },              // 0x09
	{ "LD A, (BC)", 0, ld_a_bcp },               // 0x0a
	{ "DEC BC", 0, dec_bc },                     // 0x0b
	{ "INC C", 0, inc_c },                       // 0x0c
	{ "DEC C", 0, dec_c },                       // 0x0d
	{ "LD C, 0x%02X", 1, ld_c_n },               // 0x0e
	{ "RRCA", 0, rrca },                         // 0x0f
	{ "STOP", 1, stop },                         // 0x10
	{ "LD DE, 0x%04X", 2, ld_de_nn },            // 0x11
	{ "LD (DE), A", 0, ld_de_a },               // 0x12
	{ "INC DE", 0, inc_de },                     // 0x13
	{ "INC D", 0, inc_d },                       // 0x14
	{ "DEC D", 0, dec_d },                       // 0x15
	{ "LD D, 0x%02X", 1, ld_d_n },               // 0x16
	{ "RLA", 0, rla },                           // 0x17
	{ "JR 0x%02X", 1, jr_n },                    // 0x18
	{ "ADD HL, DE", 0, add_hl_de },              // 0x19
	{ "LD A, (DE)", 0, ld_a_de },               // 0x1a
	{ "DEC DE", 0, dec_de },                     // 0x1b
	{ "INC E", 0, inc_e },                       // 0x1c
	{ "DEC E", 0, dec_e },                       // 0x1d
	{ "LD E, 0x%02X", 1, ld_e_n },               // 0x1e
	{ "RRA", 0, rra },                           // 0x1f
	{ "JR NZ, 0x%02X", 1, jr_nz_n },             // 0x20
	{ "LD HL, 0x%04X", 2, ld_hl_nn },            // 0x21
	{ "LDI (HL), A", 0, ld_hli_a },             // 0x22
	{ "INC HL", 0, inc_hl },                     // 0x23
	{ "INC H", 0, inc_h },                       // 0x24
	{ "DEC H", 0, dec_h },                       // 0x25
	{ "LD H, 0x%02X", 1, ld_h_n },               // 0x26
	{ "DAA", 0, daa },                           // 0x27
	{ "JR Z, 0x%02X", 1, jr_z_n },               // 0x28
	{ "ADD HL, HL", 0, add_hl_hl },              // 0x29
	{ "LDI A, (HL)", 0, ld_a_hli },             // 0x2a
	{ "DEC HL", 0, dec_hl },                     // 0x2b
	{ "INC L", 0, inc_l },                       // 0x2c
	{ "DEC L", 0, dec_l },                       // 0x2d
	{ "LD L, 0x%02X", 1, ld_l_n },               // 0x2e
	{ "CPL", 0, cpl },                           // 0x2f
	{ "JR NC, 0x%02X", 1, jr_nc_n },             // 0x30
	{ "LD SP, 0x%04X", 2, ld_sp_nn },            // 0x31
	{ "LDD (HL), A", 0, ld_hld_a },             // 0x32
	{ "INC SP", 0, inc_sp },                     // 0x33
	{ "INC (HL)", 0, inc_hlp },                  // 0x34
	{ "DEC (HL)", 0, dec_hlp },                  // 0x35
	{ "LD (HL), 0x%02X", 1, ld_hl_n },          // 0x36
	{ "SCF", 0, scf },                           // 0x37
	{ "JR C, 0x%02X", 1, jr_c_n },               // 0x38
	{ "ADD HL, SP", 0, add_hl_sp },              // 0x39
	{ "LDD A, (HL)", 0, ld_a_hld },              // 0x3a
	{ "DEC SP", 0, dec_sp },                     // 0x3b
	{ "INC A", 0, inc_a },                       // 0x3c
	{ "DEC A", 0, dec_a },                       // 0x3d
	{ "LD A, 0x%02X", 1, ld_a_n },               // 0x3e
	{ "CCF", 0, ccf },                           // 0x3f
	{ "LD B, B", 0, nop },                       // 0x40
	{ "LD B, C", 0, ld_b_c },                    // 0x41
	{ "LD B, D", 0, ld_b_d },                    // 0x42
	{ "LD B, E", 0, ld_b_e },                    // 0x43
	{ "LD B, H", 0, ld_b_h },                    // 0x44
	{ "LD B, L", 0, ld_b_l },                    // 0x45
	{ "LD B, (HL)", 0, ld_b_hlp },               // 0x46
	{ "LD B, A", 0, ld_b_a },                    // 0x47
	{ "LD C, B", 0, ld_c_b },                    // 0x48
	{ "LD C, C", 0, nop },                       // 0x49
	{ "LD C, D", 0, ld_c_d },                    // 0x4a
	{ "LD C, E", 0, ld_c_e },                    // 0x4b
	{ "LD C, H", 0, ld_c_h },                    // 0x4c
	{ "LD C, L", 0, ld_c_l },                    // 0x4d
	{ "LD C, (HL)", 0, ld_c_hlp },               // 0x4e
	{ "LD C, A", 0, ld_c_a },                    // 0x4f
	{ "LD D, B", 0, ld_d_b },                    // 0x50
	{ "LD D, C", 0, ld_d_c },                    // 0x51
	{ "LD D, D", 0, nop },                       // 0x52
	{ "LD D, E", 0, ld_d_e },                    // 0x53
	{ "LD D, H", 0, ld_d_h },                    // 0x54
	{ "LD D, L", 0, ld_d_l },                    // 0x55
	{ "LD D, (HL)", 0, ld_d_hlp },               // 0x56
	{ "LD D, A", 0, ld_d_a },                    // 0x57
	{ "LD E, B", 0, ld_e_b },                    // 0x58
	{ "LD E, C", 0, ld_e_c },                    // 0x59
	{ "LD E, D", 0, ld_e_d },                    // 0x5a
	{ "LD E, E", 0, nop },                       // 0x5b
	{ "LD E, H", 0, ld_e_h },                    // 0x5c
	{ "LD E, L", 0, ld_e_l },                    // 0x5d
	{ "LD E, (HL)", 0, ld_e_hlp },               // 0x5e
	{ "LD E, A", 0, ld_e_a },                    // 0x5f
	{ "LD H, B", 0, ld_h_b },                    // 0x60
	{ "LD H, C", 0, ld_h_c },                    // 0x61
	{ "LD H, D", 0, ld_h_d },                    // 0x62
	{ "LD H, E", 0, ld_h_e },                    // 0x63
	{ "LD H, H", 0, nop },                       // 0x64
	{ "LD H, L", 0, ld_h_l },                    // 0x65
	{ "LD H, (HL)", 0, ld_h_hlp },               // 0x66
	{ "LD H, A", 0, ld_h_a },                    // 0x67
	{ "LD L, B", 0, ld_l_b },                    // 0x68
	{ "LD L, C", 0, ld_l_c },                    // 0x69
	{ "LD L, D", 0, ld_l_d },                    // 0x6a
	{ "LD L, E", 0, ld_l_e },                    // 0x6b
	{ "LD L, H", 0, ld_l_h },                    // 0x6c
	{ "LD L, L", 0, nop },                       // 0x6d
	{ "LD L, (HL)", 0, ld_l_hlp },               // 0x6e
	{ "LD L, A", 0, ld_l_a },                    // 0x6f
	{ "LD (HL), B", 0, ld_hlp_b },               // 0x70
	{ "LD (HL), C", 0, ld_hlp_c },               // 0x71
	{ "LD (HL), D", 0, ld_hlp_d },               // 0x72
	{ "LD (HL), E", 0, ld_hlp_e },               // 0x73
	{ "LD (HL), H", 0, ld_hlp_h },               // 0x74
	{ "LD (HL), L", 0, ld_hlp_l },               // 0x75
	{ "HALT", 0, halt },                         // 0x76
	{ "LD (HL), A", 0, ld_hl_a },               // 0x77
	{ "LD A, B", 0, ld_a_b },                    // 0x78
	{ "LD A, C", 0, ld_a_c },                    // 0x79
	{ "LD A, D", 0, ld_a_d },                    // 0x7a
	{ "LD A, E", 0, ld_a_e },                    // 0x7b
	{ "LD A, H", 0, ld_a_h },                    // 0x7c
	{ "LD A, L", 0, ld_a_l },                    // 0x7d
	{ "LD A, (HL)", 0, ld_a_hlp },               // 0x7e
	{ "LD A, A", 0, nop },                       // 0x7f
	{ "ADD A, B", 0, add_a_b },                  // 0x80
	{ "ADD A, C", 0, add_a_c },                  // 0x81
	{ "ADD A, D", 0, add_a_d },                  // 0x82
	{ "ADD A, E", 0, add_a_e },                  // 0x83
	{ "ADD A, H", 0, add_a_h },                  // 0x84
	{ "ADD A, L", 0, add_a_l },                  // 0x85
	{ "ADD A, (HL)", 0, add_a_hlp },             // 0x86
	{ "ADD A", 0, add_a_a },                     // 0x87
	{ "ADC B", 0, adc_b },                       // 0x88
	{ "ADC C", 0, adc_c },                       // 0x89
	{ "ADC D", 0, adc_d },                       // 0x8a
	{ "ADC E", 0, adc_e },                       // 0x8b
	{ "ADC H", 0, adc_h },                       // 0x8c
	{ "ADC L", 0, adc_l },                       // 0x8d
	{ "ADC (HL)", 0, adc_hlp },                  // 0x8e
	{ "ADC A", 0, adc_a },                       // 0x8f
	{ "SUB B", 0, sub_b },                       // 0x90
	{ "SUB C", 0, sub_c },                       // 0x91
	{ "SUB D", 0, sub_d },                       // 0x92
	{ "SUB E", 0, sub_e },                       // 0x93
	{ "SUB H", 0, sub_h },                       // 0x94
	{ "SUB L", 0, sub_l },                       // 0x95
	{ "SUB (HL)", 0, sub_hlp },                  // 0x96
	{ "SUB A", 0, sub_a },                       // 0x97
	{ "SBC B", 0, sbc_b },                       // 0x98
	{ "SBC C", 0, sbc_c },                       // 0x99
	{ "SBC D", 0, sbc_d },                       // 0x9a
	{ "SBC E", 0, sbc_e },                       // 0x9b
	{ "SBC H", 0, sbc_h },                       // 0x9c
	{ "SBC L", 0, sbc_l },                       // 0x9d
	{ "SBC (HL)", 0, sbc_hlp },                  // 0x9e
	{ "SBC A", 0, sbc_a },                       // 0x9f
	{ "AND B", 0, and_b },                       // 0xa0
	{ "AND C", 0, and_c },                       // 0xa1
	{ "AND D", 0, and_d },                       // 0xa2
	{ "AND E", 0, and_e },                       // 0xa3
	{ "AND H", 0, and_h },                       // 0xa4
	{ "AND L", 0, and_l },                       // 0xa5
	{ "AND (HL)", 0, and_hlp },                  // 0xa6
	{ "AND A", 0, and_a },                       // 0xa7
	{ "XOR B", 0, xor_b },                       // 0xa8
	{ "XOR C", 0, xor_c },                       // 0xa9
	{ "XOR D", 0, xor_d },                       // 0xaa
	{ "XOR E", 0, xor_e },                       // 0xab
	{ "XOR H", 0, xor_h },                       // 0xac
	{ "XOR L", 0, xor_l },                       // 0xad
	{ "XOR (HL)", 0, xor_hlp },                  // 0xae
	{ "XOR A", 0, xor_a },                       // 0xaf
	{ "OR B", 0, or_b },                         // 0xb0
	{ "OR C", 0, or_c },                         // 0xb1
	{ "OR D", 0, or_d },                         // 0xb2
	{ "OR E", 0, or_e },                         // 0xb3
	{ "OR H", 0, or_h },                         // 0xb4
	{ "OR L", 0, or_l },                         // 0xb5
	{ "OR (HL)", 0, or_hlp },                    // 0xb6
	{ "OR A", 0, or_a },                         // 0xb7
	{ "CP B", 0, cp_b },                         // 0xb8
	{ "CP C", 0, cp_c },                         // 0xb9
	{ "CP D", 0, cp_d },                         // 0xba
	{ "CP E", 0, cp_e },                         // 0xbb
	{ "CP H", 0, cp_h },                         // 0xbc
	{ "CP L", 0, cp_l },                         // 0xbd
	{ "CP (HL)", 0, cp_hlp },                    // 0xbe
	{ "CP A", 0, cp_a },                         // 0xbf
	{ "RET NZ", 0, ret_nz },                     // 0xc0
	{ "POP BC", 0, pop_bc },                     // 0xc1
	{ "JP NZ, 0x%04X", 2, jp_nz_nn },            // 0xc2
	{ "JP 0x%04X", 2, jp_nn },                   // 0xc3
	{ "CALL NZ, 0x%04X", 2, call_nz_nn },        // 0xc4
	{ "PUSH BC", 0, push_bc },                   // 0xc5
	{ "ADD A, 0x%02X", 1, add_a_n },             // 0xc6
	{ "RST 0x00", 0, rst_0 },                    // 0xc7
	{ "RET Z", 0, ret_z },                       // 0xc8
	{ "RET", 0, ret },                           // 0xc9
	{ "JP Z, 0x%04X", 2, jp_z_nn },              // 0xca
	{ "CB %02X", 1, undefined },                      // 0xcb
	{ "CALL Z, 0x%04X", 2, call_z_nn },          // 0xcc
	{ "CALL 0x%04X", 2, call_nn },               // 0xcd
	{ "ADC 0x%02X", 1, adc_n },                  // 0xce
	{ "RST 0x08", 0, rst_08 },                   // 0xcf
	{ "RET NC", 0, ret_nc },                     // 0xd0
	{ "POP DE", 0, pop_de },                     // 0xd1
	{ "JP NC, 0x%04X", 2, jp_nc_nn },            // 0xd2
	{ "UNKNOWN", 0, undefined },                 // 0xd3
	{ "CALL NC, 0x%04X", 2, call_nc_nn },        // 0xd4
	{ "PUSH DE", 0, push_de },                   // 0xd5
	{ "SUB 0x%02X", 1, sub_n },                  // 0xd6
	{ "RST 0x10", 0, rst_10 },                   // 0xd7
	{ "RET C", 0, ret_c },                       // 0xd8
	{ "RETI", 0, undefined },          // 0xd9
	{ "JP C, 0x%04X", 2, jp_c_nn },              // 0xda
	{ "UNKNOWN", 0, undefined },                 // 0xdb
	{ "CALL C, 0x%04X", 2, call_c_nn },          // 0xdc
	{ "UNKNOWN", 0, undefined },                 // 0xdd
	{ "SBC 0x%02X", 1, sbc_n },                  // 0xde
	{ "RST 0x18", 0, rst_18 },                   // 0xdf
	{ "LD (0xFF00 + 0x%02X), A", 1, ld_ff_n_ap },// 0xe0
	{ "POP HL", 0, pop_hl },                     // 0xe1
	{ "LD (0xFF00 + C), A", 0, ld_ff_c_a },      // 0xe2
	{ "UNKNOWN", 0, undefined },                 // 0xe3
	{ "UNKNOWN", 0, undefined },                 // 0xe4
	{ "PUSH HL", 0, push_hl },                   // 0xe5
	{ "AND 0x%02X", 1, and_n },                  // 0xe6
	{ "RST 0x20", 0, rst_20 },                   // 0xe7
	{ "ADD SP,0x%02X", 1, add_sp_n },            // 0xe8
	{ "JP HL", 0, jp_hl },                       // 0xe9
	{ "LD (0x%04X), A", 2, ld_nnp_a },           // 0xea
	{ "UNKNOWN", 0, undefined },                 // 0xeb
	{ "UNKNOWN", 0, undefined },                 // 0xec
	{ "UNKNOWN", 0, undefined },                 // 0xed
	{ "XOR 0x%02X", 1, xor_n },                  // 0xee
	{ "RST 0x28", 0, rst_28 },                   // 0xef
	{ "LD A, (0xFF00 + 0x%02X)", 1, ld_ff_ap_n },// 0xf0
	{ "POP AF", 0, pop_af },                     // 0xf1
	{ "LD A, (0xFF00 + C)", 0, ld_a_ff_c },      // 0xf2
	{ "DI", 0, di_inst },                        // 0xf3
	{ "UNKNOWN", 0, undefined },                 // 0xf4
	{ "PUSH AF", 0, push_af },                   // 0xf5
	{ "OR 0x%02X", 1, or_n },                    // 0xf6
	{ "RST 0x30", 0, rst_30 },                   // 0xf7
	{ "LD HL, SP+0x%02X", 1, ld_hl_sp_n },       // 0xf8
	{ "LD SP, HL", 0, ld_sp_hl },                // 0xf9
	{ "LD A, (0x%04X)", 2, ld_a_nnp },           // 0xfa
	{ "EI", 0, ei },                             // 0xfb
	{ "UNKNOWN", 0, undefined },                 // 0xfc
	{ "UNKNOWN", 0, undefined },                 // 0xfd
	{ "CP 0x%02X", 1, cp_n },                    // 0xfe
	{ "RST 0x38", 0, rst_38 },                   // 0xff
};

const unsigned char instructionTicks[256] = {
	2, 6, 4, 4, 2, 2, 4, 4, 10, 4, 4, 4, 2, 2, 4, 4, // 0x0_
	2, 6, 4, 4, 2, 2, 4, 4,  4, 4, 4, 4, 2, 2, 4, 4, // 0x1_
	0, 6, 4, 4, 2, 2, 4, 2,  0, 4, 4, 4, 2, 2, 4, 2, // 0x2_
	4, 6, 4, 4, 6, 6, 6, 2,  0, 4, 4, 4, 2, 2, 4, 2, // 0x3_
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2, // 0x4_
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2, // 0x5_
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2, // 0x6_
	4, 4, 4, 4, 4, 4, 2, 4,  2, 2, 2, 2, 2, 2, 4, 2, // 0x7_
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2, // 0x8_
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2, // 0x9_
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2, // 0xa_
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2, // 0xb_
	0, 6, 0, 6, 0, 8, 4, 8,  0, 2, 0, 0, 0, 6, 4, 8, // 0xc_
	0, 6, 0, 0, 0, 8, 4, 8,  0, 8, 0, 0, 0, 0, 4, 8, // 0xd_
	6, 6, 4, 0, 0, 8, 4, 8,  8, 2, 8, 0, 0, 0, 4, 8, // 0xe_
	6, 6, 4, 2, 0, 8, 4, 8,  6, 4, 8, 2, 0, 0, 4, 8  // 0xf_
};

int EmulateGBOp() {
	//Get current opcode from memory via CPU state
	uint8_t instruction = ReadMem(state.pc);
	uint16_t operand = 0;

	if (stopped) return;

	state.pc += 1;

	if (instructions[instruction].operandLength == 1) operand = ReadMem(state.pc);
	if (instructions[instruction].operandLength == 2) operand = ReadShort(state.pc);
	state.pc += instructions[instruction].operandLength;

	switch (instructions[instruction].operandLength) {
	case 0:
		((void (*)(void))instructions[instruction].execute)();
		break;
	case 1:
		((void (*)(unsigned char))instructions[instruction].execute)((unsigned char)operand);
		break;
	case 2:
		((void (*)(unsigned short))instructions[instruction].execute)(operand);
		break;
	}

	ticks += instructionTicks[instruction];

	if (state.pc == 0x282a) {
		FILE* f;
		fopen_s(&f, "tile0.bin", "wb");
		fwrite(ReadMem(0x8000), 16, 1, f);
		fclose(f);
	}

#if PRINT_OPS
	if (instructions[instruction].operandLength)
		printf(instructions[instruction].disassembly, operand);
	else
		printf(instructions[instruction].disassembly); 
	printf("\n");
#endif
#if PRINT_DBG
	printf("%c", FLAGS_ISSET(FLAGS_ZERO) ? 'z' : '.');
	printf("%c", FLAGS_ISSET(FLAGS_NEGATIVE) ? 'n' : '.');
	printf("%c", FLAGS_ISSET(FLAGS_HALFCARRY) ? 'h' : '.');
	printf("%c ", FLAGS_ISSET(FLAGS_CARRY) ? 'c' : '.');
	//printf("A $%02x B $%02x C $%02x D $%02x E $%02x H $%02x L $%02x SP %04x\n", state.a, state.b, state.c, state.d, state.e, state.h, state.l, state.sp);
	printf("AF $%04x BC $%04x DE $%04x HL $%04x PC %04x SP %04x\n", state.af, state.bc, state.de, state.hl, state.pc, state.sp);
#endif
	return 0;
}
void undefined(void) {
	state.pc--;

	unsigned char instruction = ReadMem(state.pc);

#ifdef _WIN32
	char d[100];
	sprintf_s(d, "Undefined instruction 0x%02x!\n\nCheck stdout for more details.", instruction);
	//MessageBox(NULL, d, "Cinoop", MB_OK);
#else
#ifndef PS4
	printf("Undefined instruction 0x%02x!\n", instruction);
#endif
#endif

	printRegisters();
	return -1;
}

void Add(uint8_t* address, uint8_t value) {
	unsigned int result = *address + value;

	if (result & 0xFF00) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);

	*address = (unsigned char)(result & 0xFF);

	if (*address) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);

	if (((*address & 0x0F) + (value & 0x0F)) > 0x0F) FLAGS_SET(FLAGS_HALFCARRY);
	else FLAGS_CLEAR(FLAGS_HALFCARRY);

	FLAGS_CLEAR(FLAGS_NEGATIVE);
}

void Add16(uint16_t* address, uint16_t value ) {
	unsigned long result = *address + value;

	if (result & 0xFFFF0000) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);

	*address = (unsigned short)(result & 0xffff);

	if (((*address & 0x0f) + (value & 0x0f)) > 0x0f) FLAGS_SET(FLAGS_HALFCARRY);
	else FLAGS_CLEAR(FLAGS_HALFCARRY);

	FLAGS_CLEAR(FLAGS_NEGATIVE);
}

static void Sub(uint8_t value) {
	FLAGS_SET(FLAGS_NEGATIVE);

	if (value > state.a) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);

	if ((value & 0x0f) > (state.a & 0x0f)) FLAGS_SET(FLAGS_HALFCARRY);
	else FLAGS_CLEAR(FLAGS_HALFCARRY);

	state.a -= value;

	if (state.a) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);
}

uint8_t Inc(uint8_t reg) {
	if (((reg) & 0x0f) == 0x0f)
		FLAGS_SET(FLAGS_HALFCARRY);
	else
		FLAGS_CLEAR(FLAGS_HALFCARRY);
	reg++;
	if (reg == 0)
		FLAGS_SET(FLAGS_ZERO);
	else
		FLAGS_CLEAR(FLAGS_ZERO);
	//Subtract flag reset to 0
	FLAGS_CLEAR(FLAGS_NEGATIVE);

	return reg;
}

uint8_t Dec( uint8_t reg) {
	if (((reg) & 0x0f) == 0x0f)
		FLAGS_SET(FLAGS_HALFCARRY);
	else
		FLAGS_CLEAR(FLAGS_HALFCARRY);
	reg--;
	if (reg == 0)
		FLAGS_SET(FLAGS_ZERO);
	else
		FLAGS_CLEAR(FLAGS_ZERO);
	//Subtract flag reset to 1
	FLAGS_SET(FLAGS_NEGATIVE);

	return reg;
}

static void Adc(uint8_t value) {
	value += FLAGS_ISCARRY ? 1 : 0;

	int result = state.a + value;

	if (result & 0xff00) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);

	if (value == state.a) FLAGS_SET(FLAGS_ZERO);
	else FLAGS_CLEAR(FLAGS_ZERO);

	if (((value & 0x0f) + (state.a & 0x0f)) > 0x0f) FLAGS_SET(FLAGS_HALFCARRY);
	else FLAGS_CLEAR(FLAGS_HALFCARRY);

	FLAGS_SET(FLAGS_NEGATIVE);

	state.a = (uint8_t)(result & 0xff);
}

static void Sbc(unsigned char value) {
	value += FLAGS_ISCARRY ? 1 : 0;

	FLAGS_SET(FLAGS_NEGATIVE);

	if (value > state.a) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);

	if (value == state.a) FLAGS_SET(FLAGS_ZERO);
	else FLAGS_CLEAR(FLAGS_ZERO);

	if ((value & 0x0f) > (state.a & 0x0f)) FLAGS_SET(FLAGS_HALFCARRY);
	else FLAGS_CLEAR(FLAGS_HALFCARRY);

	state.a -= value;
}

static void and (unsigned char value) {
	state.a &= value;

	if (state.a) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);

	FLAGS_CLEAR(FLAGS_CARRY | FLAGS_NEGATIVE);
	FLAGS_SET(FLAGS_HALFCARRY);
}

static void or (unsigned char value) {
	state.a |= value;

	if (state.a) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);

	FLAGS_CLEAR(FLAGS_CARRY | FLAGS_NEGATIVE | FLAGS_HALFCARRY);
}

static void xor (unsigned char value) {
	state.a ^= value;

	if (state.a) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);

	FLAGS_CLEAR(FLAGS_CARRY | FLAGS_NEGATIVE | FLAGS_HALFCARRY);
}

static void cp(unsigned char value) {
	if (state.a == value) FLAGS_SET(FLAGS_ZERO);
	else FLAGS_CLEAR(FLAGS_ZERO);

	if (value > state.a) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);

	if ((value & 0x0f) > (state.a & 0x0f)) FLAGS_SET(FLAGS_HALFCARRY);
	else FLAGS_CLEAR(FLAGS_HALFCARRY);

	FLAGS_SET(FLAGS_NEGATIVE);
}

// 0x00
void nop(void) {  }

// 0x01
void ld_bc_nn(unsigned short operand) { state.bc = operand; }

// 0x02
void ld_bcp_a(void) { state.bc = state.a; }

// 0x03
void inc_bc(void) { state.bc++; }

// 0x04
void inc_b(void) { state.b = Inc(state.b); }

// 0x05
void dec_b(void) { state.b = Dec(state.b); }

// 0x06
void ld_b_n(unsigned char operand) { state.b = operand; }

// 0x07
void rlca(void) {
	unsigned char carry = (state.a & 0x80) >> 7;
	if (carry) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);

	state.a <<= 1;
	state.a += carry;

	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_ZERO | FLAGS_HALFCARRY);
}

// 0x08
void ld_nnp_sp(unsigned short operand) { WriteShort(operand, state.sp); }

// 0x09
void add_hl_bc(void) { Add16(&state.hl, state.bc); }

// 0x0a
void ld_a_bcp(void) { state.a = state.bc; }

// 0x0b
void dec_bc(void) { state.bc--; }

// 0x0c
void inc_c(void) { state.c = Inc(state.c); }

// 0x0d
void dec_c(void) { state.c = Dec(state.c); }

// 0x0e
void ld_c_n(unsigned char operand) { state.c = operand; }

// 0x0f
void rrca(void) {
	unsigned char carry = state.a & 0x01;
	if (carry) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);

	state.a >>= 1;
	if (carry) state.a |= 0x80;

	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_ZERO | FLAGS_HALFCARRY);
}

// 0x10
void stop(unsigned char operand) { stopped = 1; }

// 0x11
void ld_de_nn(unsigned short operand) { state.de = operand; }

// 0x12
void ld_de_a(void) { state.de = state.a; }

// 0x13
void inc_de(void) { state.de++; }

// 0x14
void inc_d(void) { state.d = Inc(state.d); }

// 0x15
void dec_d(void) { state.d = Dec(state.d); }

// 0x16
void ld_d_n(unsigned char operand) { state.d = operand; }

// 0x17
void rla(void) {
	int carry = FLAGS_ISSET(FLAGS_CARRY) ? 1 : 0;

	if (state.a & 0x80) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);

	state.a <<= 1;
	state.a += carry;

	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_ZERO | FLAGS_HALFCARRY);
}

// 0x18
void jr_n(unsigned char operand) {
	state.pc += (signed char)operand;
}

// 0x19
void add_hl_de(void) { Add16(&state.hl, state.de); }

// 0x1a
void ld_a_de(void) { state.a = state.de; }

// 0x1b
void dec_de(void) { state.de--; }

// 0x1c
void inc_e(void) { state.e = Inc(state.e); }

// 0x1d
void dec_e(void) { state.e = Dec(state.e); }

// 0x1e
void ld_e_n(unsigned char operand) { state.e = operand; }

// 0x1f
void rra(void) {
	int carry = (FLAGS_ISSET(FLAGS_CARRY) ? 1 : 0) << 7;

	if (state.a & 0x01) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);

	state.a >>= 1;
	state.a += carry;

	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_ZERO | FLAGS_HALFCARRY);
}

// 0x20
void jr_nz_n(unsigned char operand) {
	if (FLAGS_ISZERO) ticks += 8;
	else {;
		state.pc = (state.pc + (char)operand);
		ticks += 12;
	}
}

// 0x21
void ld_hl_nn(unsigned short operand) { state.hl = operand; }

// 0x22
void ld_hli_a(void) { state.hl = state.a; state.hl++; }

// 0x23
void inc_hl(void) { state.hl++; }

// 0x24
void inc_h(void) { state.h = Inc(state.h); }

// 0x25
void dec_h(void) { state.h = Dec(state.h); }

// 0x26
void ld_h_n(unsigned char operand) { state.h = operand; }

// 0x27
void daa(void) {
	uint16_t s = state.a;

	if (FLAGS_ISNEGATIVE) {
		if (FLAGS_ISHALFCARRY) s = (s - 0x06) & 0xFF;
		if (FLAGS_ISCARRY) s -= 0x60;
	}
	else {
		if (FLAGS_ISHALFCARRY || (s & 0xF) > 9) s += 0x06;
		if (FLAGS_ISCARRY || s > 0x9F) s += 0x60;
	}

	state.a = s;
	FLAGS_CLEAR(FLAGS_HALFCARRY);

	if (state.a) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);

	if (s >= 0x100) FLAGS_SET(FLAGS_CARRY);

}

// 0x28
void jr_z_n(unsigned char operand) {
	if (FLAGS_ISZERO) {
		state.pc += (uint16_t)operand;
		ticks += 12;
	}
	else ticks += 8;
}

// 0x29
void add_hl_hl(void) { Add16(&state.hl, state.hl); }

// 0x2a
void ld_a_hli(void) { state.a = state.hl++; }

// 0x2b
void dec_hl(void) { state.hl--; }

// 0x2c
void inc_l(void) { state.l = Inc(state.l); }

// 0x2d
void dec_l(void) { state.l = Dec(state.l); }

// 0x2e
void ld_l_n(unsigned char operand) { state.l = operand; }

// 0x2f
void cpl(void) { state.a = ~state.a; FLAGS_SET(FLAGS_NEGATIVE | FLAGS_HALFCARRY); }

// 0x30
void jr_nc_n(char operand) {
	if (FLAGS_ISCARRY) ticks += 8;
	else {
		state.pc += operand;
		ticks += 12;
	}
}

// 0x31
void ld_sp_nn(unsigned short operand) { state.sp = operand; }

// 0x32
void ld_hld_a(void) { WriteMem(state.hl, state.a); state.hl--; }

// 0x33
void inc_sp(void) { state.sp++; }

// 0x34
void inc_hlp(void) { state.hl = Inc(state.hl); }

// 0x35
void dec_hlp(void) { state.hl = Dec(state.hl); }

// 0x36
void ld_hl_n(unsigned char operand) { state.hl = operand; }

// 0x37
void scf(void) { FLAGS_SET(FLAGS_CARRY); FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY); }

// 0x38
void jr_c_n(char operand) {
	if (FLAGS_ISCARRY) {
		state.pc += operand;
		ticks += 12;
	}
	else ticks += 8;
}

// 0x39
void add_hl_sp(void) { Add16(&state.hl, state.sp); }

// 0x3a
void ld_a_hld(void) { state.a = ReadMem(state.hl--); }

// 0x3b
void dec_sp(void) { state.sp--; }

// 0x3c
void inc_a(void) { state.a = Inc(state.a); }

// 0x3d
void dec_a(void) { state.a = Dec(state.a); }

// 0x3e
void ld_a_n(unsigned char operand) { state.a = operand; }

// 0x3f
void ccf(void) {
	if (FLAGS_ISCARRY) FLAGS_CLEAR(FLAGS_CARRY);
	else FLAGS_SET(FLAGS_CARRY);

	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY);
}

// 0x41
void ld_b_c(void) { state.b = state.c; }

// 0x42
void ld_b_d(void) { state.b = state.d; }

// 0x43
void ld_b_e(void) { state.b = state.e; }

// 0x44
void ld_b_h(void) { state.b = state.h; }

// 0x45
void ld_b_l(void) { state.b = state.l; }

// 0x46
void ld_b_hlp(void) { state.b = state.hl; }

// 0x47
void ld_b_a(void) { state.b = state.a; }

// 0x48
void ld_c_b(void) { state.c = state.b; }

// 0x4a
void ld_c_d(void) { state.c = state.d; }

// 0x4b
void ld_c_e(void) { state.c = state.e; }

// 0x4c
void ld_c_h(void) { state.c = state.h; }

// 0x4d
void ld_c_l(void) { state.c = state.l; }

// 0x4e
void ld_c_hlp(void) { state.c = state.hl; }

// 0x4f
void ld_c_a(void) { state.c = state.a; }

// 0x50
void ld_d_b(void) { state.d = state.b; }

// 0x51
void ld_d_c(void) { state.d = state.c; }

// 0x53
void ld_d_e(void) { state.d = state.e; }

// 0x54
void ld_d_h(void) { state.d = state.h; }

// 0x55
void ld_d_l(void) { state.d = state.l; }

// 0x56
void ld_d_hlp(void) { state.d = state.hl; }

// 0x57
void ld_d_a(void) { state.d = state.a; }

// 0x58
void ld_e_b(void) { state.e = state.b; }

// 0x59
void ld_e_c(void) { state.e = state.c; }

// 0x5a
void ld_e_d(void) { state.e = state.d; }

// 0x5c
void ld_e_h(void) { state.e = state.h; }

// 0x5d
void ld_e_l(void) { state.e = state.l; }

// 0x5e
void ld_e_hlp(void) { state.e = state.hl; }

// 0x5f
void ld_e_a(void) { state.e = state.a; }

// 0x60
void ld_h_b(void) { state.h = state.b; }

// 0x61
void ld_h_c(void) { state.h = state.c; }

// 0x62
void ld_h_d(void) { state.h = state.d; }

// 0x63
void ld_h_e(void) { state.h = state.e; }

// 0x65
void ld_h_l(void) { state.h = state.l; }

// 0x66
void ld_h_hlp(void) { state.h = state.hl; }

// 0x67
void ld_h_a(void) { state.h = state.a; }

// 0x68
void ld_l_b(void) { state.l = state.b; }

// 0x69
void ld_l_c(void) { state.l = state.c; }

// 0x6a
void ld_l_d(void) { state.l = state.d; }

// 0x6b
void ld_l_e(void) { state.l = state.e; }

// 0x6c
void ld_l_h(void) { state.l = state.h; }

// 0x6e
void ld_l_hlp(void) { state.l = state.hl; }

// 0x6f
void ld_l_a(void) { state.l = state.a; }

// 0x70
void ld_hlp_b(void) { state.hl = state.b; }

// 0x71
void ld_hlp_c(void) { state.hl = state.c; }

// 0x72
void ld_hlp_d(void) { state.hl = state.d; }

// 0x73
void ld_hlp_e(void) { state.hl = state.e; }

// 0x74
void ld_hlp_h(void) { state.hl = state.h; }

// 0x75
void ld_hlp_l(void) { state.hl = state.l; }

// 0x76
void halt(void) {
	//if (interrupt.master) {
	//	//HALT EXECUTION UNTIL AN INTERRUPT OCCURS
	//}
	//else state.pc++;
}

// 0x77
void ld_hl_a(void) { state.hl = state.a; }

// 0x78
void ld_a_b(void) { state.a = state.b; }

// 0x79
void ld_a_c(void) { state.a = state.c; }

// 0x7a
void ld_a_d(void) { state.a = state.d; }

// 0x7b
void ld_a_e(void) { state.a = state.e; }

// 0x7c
void ld_a_h(void) { state.a = state.h; }

// 0x7d
void ld_a_l(void) { state.a = state.l; }

// 0x7e
void ld_a_hlp(void) { state.a = state.hl; }

// 0x80
void add_a_b(void) { Add(&state.a, state.b); }

// 0x81
void add_a_c(void) { Add(&state.a, state.c); }

// 0x82
void add_a_d(void) { Add(&state.a, state.d); }

// 0x83
void add_a_e(void) { Add(&state.a, state.e); }

// 0x84
void add_a_h(void) { Add(&state.a, state.h); }

// 0x85
void add_a_l(void) { Add(&state.a, state.l); }

// 0x86
void add_a_hlp(void) { Add(&state.a, state.hl); }

// 0x87
void add_a_a(void) { Add(&state.a, state.a); }

// 0x88
void adc_b(void) { Adc(state.b); }

// 0x89
void adc_c(void) { Adc(state.c); }

// 0x8a
void adc_d(void) { Adc(state.d); }

// 0x8b
void adc_e(void) { Adc(state.e); }

// 0x8c
void adc_h(void) { Adc(state.h); }

// 0x8d
void adc_l(void) { Adc(state.l); }

// 0x8e
void adc_hlp(void) { Adc(state.hl); }

// 0x8f
void adc_a(void) { Adc(state.a); }

// 0x90
void sub_b(void) { Sub(state.b); }

// 0x91
void sub_c(void) { Sub(state.c); }

// 0x92
void sub_d(void) { Sub(state.d); }

// 0x93
void sub_e(void) { Sub(state.e); }

// 0x94
void sub_h(void) { Sub(state.h); }

// 0x95
void sub_l(void) { Sub(state.l); }

// 0x96
void sub_hlp(void) { Sub(state.hl); }

// 0x97
void sub_a(void) { Sub(state.a); }

// 0x98
void sbc_b(void) { Sbc(state.b); }

// 0x99
void sbc_c(void) { Sbc(state.c); }

// 0x9a
void sbc_d(void) { Sbc(state.d); }

// 0x9b
void sbc_e(void) { Sbc(state.e); }

// 0x9c
void sbc_h(void) { Sbc(state.h); }

// 0x9d
void sbc_l(void) { Sbc(state.l); }

// 0x9e
void sbc_hlp(void) { Sbc(state.hl); }

// 0x9f
void sbc_a(void) { Sbc(state.a); }

// 0xa0
void and_b(void) { and (state.b); }

// 0xa1
void and_c(void) { and (state.c); }

// 0xa2
void and_d(void) { and (state.d); }

// 0xa3
void and_e(void) { and (state.e); }

// 0xa4
void and_h(void) { and (state.h); }

// 0xa5
void and_l(void) { and (state.l); }

// 0xa6
void and_hlp(void) { and (state.hl); }

// 0xa7
void and_a(void) { and (state.a); }

// 0xa8
void xor_b(void) { xor (state.b); }

// 0xa9
void xor_c(void) { xor (state.c); }

// 0xaa
void xor_d(void) { xor (state.d); }

// 0xab
void xor_e(void) { xor (state.e); }

// 0xac
void xor_h(void) { xor (state.h); }

// 0xad
void xor_l(void) { xor (state.l); }

// 0xae
void xor_hlp(void) { xor (state.hl); }

// 0xaf
void xor_a(void) { xor (state.a); }

// 0xb0
void or_b(void) { or (state.b); }

// 0xb1
void or_c(void) { or (state.c); }

// 0xb2
void or_d(void) { or (state.d); }

// 0xb3
void or_e(void) { or (state.e); }

// 0xb4
void or_h(void) { or (state.h); }

// 0xb5
void or_l(void) { or (state.l); }

// 0xb6
void or_hlp(void) { or (state.hl); }

// 0xb7
void or_a(void) { or (state.a); }

// 0xb8
void cp_b(void) { cp(state.b); }

// 0xb9
void cp_c(void) { cp(state.c); }

// 0xba
void cp_d(void) { cp(state.d); }

// 0xbb
void cp_e(void) { cp(state.e); }

// 0xbc
void cp_h(void) { cp(state.h); }

// 0xbd
void cp_l(void) { cp(state.l); }

// 0xbe
void cp_hlp(void) { cp(state.hl); }

// 0xbf
void cp_a(void) { cp(state.a); }

// 0xc0
void ret_nz(void) {
	if (FLAGS_ISZERO) ticks += 8;
	else {
		state.pc = ReadShortFromStack();
		
		ticks += 20;
	}
}

// 0xc1
void pop_bc(void) { state.bc = ReadShortFromStack(); }

// 0xc2
void jp_nz_nn(unsigned short operand) {
	if (FLAGS_ISZERO) ticks += 12;
	else {
		state.pc = operand;
		
		ticks += 16;
	}
}

// 0xc3
void jp_nn(unsigned short operand) {
	state.pc = operand;
}

// 0xc4
void call_nz_nn(unsigned short operand) {
	if (FLAGS_ISZERO) ticks += 12;
	else {
		WriteShortToStack(state.pc);
		state.pc = operand;
		ticks += 24;
	}
}

// 0xc5
void push_bc(void) { WriteShortToStack(state.bc); }

// 0xc6
void add_a_n(unsigned char operand) { Add(&state.a, operand); }

// 0xc7
void rst_0(void) { WriteShortToStack(state.pc); state.pc = 0x0000; }

// 0xc8
void ret_z(void) {
	if (FLAGS_ISZERO) {
		state.pc = ReadShortFromStack();
		ticks += 20;
	}
	else ticks += 8;
}

// 0xc9
void ret(void) { state.pc = ReadShortFromStack(); }

// 0xca
void jp_z_nn(unsigned short operand) {
	if (FLAGS_ISZERO) {
		state.pc = operand;
		ticks += 16;
	}
	else ticks += 12;
}

// 0xcb
// cb.c

// 0xcc
void call_z_nn(unsigned short operand) {
	if (FLAGS_ISZERO) {
		WriteShortToStack(state.pc);
		state.pc = operand;
		ticks += 24;
	}
	else ticks += 12;
}

// 0xcd
void call_nn(unsigned short operand) { WriteShortToStack(state.pc); state.pc = operand; }

// 0xce
void adc_n(unsigned char operand) { Adc(operand); }

// 0xcf
void rst_08(void) { WriteShortToStack(state.pc); state.pc = 0x0008; }

// 0xd0
void ret_nc(void) {
	if (FLAGS_ISCARRY) ticks += 8;
	else {
		state.pc = ReadShortFromStack();
		ticks += 20;
	}
}

// 0xd1
void pop_de(void) { state.de = ReadShortFromStack(); }

// 0xd2
void jp_nc_nn(unsigned short operand) {
	if (!FLAGS_ISCARRY) {
		state.pc = operand;
		ticks += 16;
	}
	else ticks += 12;
}

// 0xd4
void call_nc_nn(unsigned short operand) {
	if (!FLAGS_ISCARRY) {
		WriteShortToStack(state.pc);
		state.pc = operand;
		ticks += 24;
	}
	else ticks += 12;
}

// 0xd5
void push_de(void) { WriteShortToStack(state.de); }

// 0xd6
void sub_n(unsigned char operand) { Sub(operand); }

// 0xd7
void rst_10(void) { WriteShortToStack(state.pc); state.pc = 0x0010; }

// 0xd8
void ret_c(void) {
	if (FLAGS_ISCARRY) {
		state.pc = ReadShortFromStack();
		ticks += 20;
	}
	else ticks += 8;
}

// 0xd9
// interrupts.c

// 0xda
void jp_c_nn(unsigned short operand) {
	if (FLAGS_ISCARRY) {
		state.pc = operand;
		ticks += 16;
	}
	else ticks += 12;
}

// 0xdc
void call_c_nn(unsigned short operand) {
	if (FLAGS_ISCARRY) {
		WriteShortToStack(state.pc);
		state.pc = operand;
		ticks += 24;
	}
	else ticks += 12;
}

// 0xde
void sbc_n(unsigned char operand) { Sbc(operand); }

// 0xdf
void rst_18(void) { WriteShortToStack(state.pc); state.pc = 0x0018; }

// 0xe0
void ld_ff_n_ap(unsigned char operand) { WriteMem(0xff00 + operand, state.a); }

// 0xe1
void pop_hl(void) { state.hl = ReadShortFromStack(); }

// 0xe2
void ld_ff_c_a(void) { WriteMem(0xff00 + state.c, state.a); }

// 0xe5
void push_hl(void) { WriteShortToStack(state.hl); }

// 0xe6
void and_n(unsigned char operand) {
	state.a &= operand;

	FLAGS_CLEAR(FLAGS_CARRY | FLAGS_NEGATIVE);
	FLAGS_SET(FLAGS_HALFCARRY);
	if (state.a) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);
}

// 0xe7
void rst_20(void) { WriteShortToStack(state.pc); state.pc = 0x0020; }

// 0xe8
void add_sp_n(char operand) {
	int result = state.sp + operand;

	if (result & 0xffff0000) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);

	state.sp = result & 0xffff;

	if (((state.sp & 0x0f) + (operand & 0x0f)) > 0x0f) FLAGS_SET(FLAGS_HALFCARRY);
	else FLAGS_CLEAR(FLAGS_HALFCARRY);

	// _does_ clear the zero flag
	FLAGS_CLEAR(FLAGS_ZERO | FLAGS_NEGATIVE);
}

// 0xe9
void jp_hl(void) {
	state.pc = state.hl;
}

// 0xea
void ld_nnp_a(unsigned short operand) { WriteMem(operand, state.a); }

// 0xee
void xor_n(unsigned char operand) { xor (operand); }

//0xef
void rst_28(void) { WriteShortToStack(state.pc); state.pc = 0x0028; }

// 0xf0
void ld_ff_ap_n(unsigned char operand) { 
	state.a = ReadMem(0xff00 + operand); 
}

// 0xf1
void pop_af(void) { state.af = ReadShortFromStack(); }

// 0xf2
void ld_a_ff_c(void) { state.a = ReadMem(0xff00 + state.c); }

// 0xf3
void di_inst(void) { state.interrupts = 0; }

// 0xf5
void push_af(void) { WriteShortToStack(state.af); }

// 0xf6
void or_n(unsigned char operand) { or (operand); }

// 0xf7
void rst_30(void) { WriteShortToStack(state.pc); state.pc = 0x0030; }

// 0xf8
void ld_hl_sp_n(unsigned char operand) {
	int result = state.sp + (signed char)operand;

	if (result & 0xffff0000) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);

	if (((state.sp & 0x0f) + (operand & 0x0f)) > 0x0f) FLAGS_SET(FLAGS_HALFCARRY);
	else FLAGS_CLEAR(FLAGS_HALFCARRY);

	FLAGS_CLEAR(FLAGS_ZERO | FLAGS_NEGATIVE);

	state.hl = (unsigned short)(result & 0xffff);
}

// 0xf9
void ld_sp_hl(void) { state.sp = state.hl; }

// 0xfa
void ld_a_nnp(unsigned short operand) { state.a = ReadMem(operand); }

// 0xfb
void ei(void) { state.interrupts = 1; }

// 0xfe
void cp_n(unsigned char operand) {
	FLAGS_SET(FLAGS_NEGATIVE);

	if (state.a == operand) FLAGS_SET(FLAGS_ZERO);
	else FLAGS_CLEAR(FLAGS_ZERO);

	if (operand > state.a) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);

	if ((operand & 0x0f) > (state.a & 0x0f)) FLAGS_SET(FLAGS_HALFCARRY);
	else FLAGS_CLEAR(FLAGS_HALFCARRY);
}

//0xff
void rst_38(void) { WriteShortToStack(state.pc); state.pc = 0x0038; }