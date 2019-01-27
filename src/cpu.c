#include <stdint.h>
#include <stdio.h>

#include "chip8.h"

void cpu_initialize(struct chip8_s *chip8) {
    chip8->i = 0;
    chip8->pc = CHIP8_ROM_START;
    chip8->sp = 0;
    chip8->dt = 0;
    chip8->st = 0;
}

/**
 * Executes a single instruction of the cpu pointed at by pc, the program counter.
 * Returns false if invalid instruction.
 **/
bool cpu_execute(struct chip8_s *cpu) {
    if (cpu->pc > CHIP8_MAX_ROM_SIZE) {
        return false;
    }
    uint16_t opcode = (cpu->memory[cpu->pc] << 8) | cpu->memory[cpu->pc + 1];
    if (opcode == 0) {
        return false;
    }
    printf("opcode: %#06x\n", opcode);
    uint8_t instr_code = (opcode & 0xF000) >> 12;
    printf("switching on: %x\n", instr_code);
    switch (instr_code) {
        case 0x0: printf("SYS, CLS, RET\n");
        break;
        case 0x1: printf("JMP\n");
        break;
        case 0x2: printf("CALL\n");
        break;
        case 0x3: printf("SE\n");
        break;
        case 0x4: printf("SNE\n");
        break;
        case 0x5: printf("SE\n");
        break;
        case 0x6: printf("LD\n");
        break;
        case 0x7: printf("ADD\n");
        break;
        case 0x8: printf("LD, OR, AND, XOR, ADD, SUB, SHR, SUBN, SHL\n");
        break;
        case 0x9: printf("SNE\n");
        break;
        case 0xA: printf("LD\n");
        break;
        case 0xB: printf("JP\n");
        break;
        case 0xC: printf("RND\n");
        break;
        case 0xD: printf("DRW\n");
        break;
        case 0xE: printf("SKP, SKNP\n");
        break;
        case 0xF: printf("LD\n");
        break;
        default: printf("Invalid opcode\n");
                 return false;
        break;
    }
    cpu->pc += 2;
    return true;
}

