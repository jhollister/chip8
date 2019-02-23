#ifndef OPCODE_H
#define OPCODE_H

#include <stdint.h>

#include "chip8.h"


void opcode0(struct chip8 *cpu, uint16_t opcode);
void opcode1(struct chip8 *cpu, uint16_t opcode);
void opcode2(struct chip8 *cpu, uint16_t opcode);
void opcode3(struct chip8 *cpu, uint16_t opcode);
void opcode4(struct chip8 *cpu, uint16_t opcode);
void opcode5(struct chip8 *cpu, uint16_t opcode);
void opcode6(struct chip8 *cpu, uint16_t opcode);
void opcode7(struct chip8 *cpu, uint16_t opcode);
void opcode8(struct chip8 *cpu, uint16_t opcode);
void opcode9(struct chip8 *cpu, uint16_t opcode);
void opcodeA(struct chip8 *cpu, uint16_t opcode);
void opcodeB(struct chip8 *cpu, uint16_t opcode);
void opcodeC(struct chip8 *cpu, uint16_t opcode);
void opcodeD(struct chip8 *cpu, uint16_t opcode);
void opcodeE(struct chip8 *cpu, uint16_t opcode);
void opcodeF(struct chip8 *cpu, uint16_t opcode);

#endif // OPCODE_H