#include <stdint.h>
#include <stdio.h>

#include "chip8.h"
#include "opcodes.h"

void (*opcode_handler[])(struct chip8_s*, uint16_t) = {
    opcode0,
    opcode1,
    opcode2,
    opcode3,
    opcode4,
    opcode5,
    opcode6,
    opcode7,
    opcode8,
    opcode9,
    opcodeA,
    opcodeB,
    opcodeC,
    opcodeD,
    opcodeE,
    opcodeF
};

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
    opcode_handler[instr_code](cpu, opcode);
    return true;
}

