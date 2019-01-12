#include <stdint.h>
#include <stdio.h>

#include "chip8.h"

void chip8_cpu_initialize(Chip8_Cpu *cpu) {
    cpu->i = 0;
    cpu->pc = CHIP8_ROM_START;
    cpu->sp = 0;
    cpu->dt = 0;
    cpu->st = 0;
}

/**
 * Executes a single instruction of the cpu pointed at by pc, the program counter.
 * Returns false when there are no more instructions to execute.
 **/
bool chip8_cpu_execute(Chip8_Cpu *cpu, uint8_t *memory) {
    uint16_t opcode = (memory[cpu->pc] << 8) | memory[cpu->pc + 1];
    printf("opcode: %#06x\n", opcode);
    cpu->pc += 2;
    if (cpu->pc > CHIP8_MAX_ROM_SIZE) {
        return false;
    }
    if (opcode == 0) {
        return false;
    }
    return true;
}

