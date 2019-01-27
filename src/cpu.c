#include <stdint.h>
#include <stdio.h>

#include "chip8.h"
#include "opcodes.h"

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
    if (cpu->pc > CHIP8_MEMORY_SIZE) {
        printf("Program counter exceeded memory %d\n", cpu->pc);
        return false;
    }
    uint16_t opcode = (cpu->memory[cpu->pc] << 8) | cpu->memory[cpu->pc + 1];
    printf("%#06x: ", opcode);
    uint8_t instr_code = (opcode & 0xF000) >> 12;
    switch (instr_code) {
        case 0x0: opcode0(cpu, opcode);
        break;
        case 0x1: opcode1(cpu, opcode);
        break;
        case 0x2: opcode2(cpu, opcode);
        break;
        case 0x3: opcode3(cpu, opcode);
        break;
        case 0x4: opcode4(cpu, opcode);
        break;
        case 0x5: opcode5(cpu, opcode);
        break;
        case 0x6: opcode6(cpu, opcode);
        break;
        case 0x7: opcode7(cpu, opcode);
        break;
        case 0x8: opcode8(cpu, opcode);
        break;
        case 0x9: opcode9(cpu, opcode);
        break;
        case 0xA: opcodeA(cpu, opcode);
        break;
        case 0xB: opcodeB(cpu, opcode);
        break;
        case 0xC: opcodeC(cpu, opcode);
        break;
        case 0xD: opcodeD(cpu, opcode);
        break;
        case 0xE: opcodeE(cpu, opcode);
        break;
        case 0xF: opcodeF(cpu, opcode);
        break;
        default: printf("Invalid opcode\n");
                 return false;
        break;
    }
    return true;
}

