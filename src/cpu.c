#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "chip8.h"
#include "opcodes.h"

#define NELEMS(arr) (sizeof(arr)/sizeof(arr[0]))

const uint8_t chip8_fonts[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,    /* 0 */
    0x20, 0x60, 0x20, 0x20, 0x70,    /* 1 */
    0xF0, 0x10, 0xF0, 0x80, 0xF0,    /* 2 */
    0xF0, 0x10, 0xF0, 0x10, 0xF0,    /* 3 */
    0x90, 0x90, 0xF0, 0x10, 0x10,    /* 4 */
    0xF0, 0x80, 0xF0, 0x10, 0xF0,    /* 5 */
    0xF0, 0x80, 0xF0, 0x90, 0xF0,    /* 6 */
    0xF0, 0x10, 0x20, 0x40, 0x40,    /* 7 */
    0xF0, 0x90, 0xF0, 0x90, 0xF0,    /* 8 */
    0xF0, 0x90, 0xF0, 0x10, 0xF0,    /* 9 */
    0xF0, 0x90, 0xF0, 0x90, 0x90,    /* A */
    0xE0, 0x90, 0xE0, 0x90, 0xE0,    /* B */
    0xF0, 0x80, 0x80, 0x80, 0xF0,    /* C */
    0xE0, 0x90, 0x90, 0x90, 0xE0,    /* D */
    0xF0, 0x80, 0xF0, 0x80, 0xF0,    /* E */
    0xF0, 0x80, 0xF0, 0x80, 0x80,    /* F */
};

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
    memset(chip8->memory, 0, sizeof(chip8->memory));
    memset(chip8->v, 0, sizeof(chip8->v));
    memset(chip8->stack, 0, sizeof(chip8->stack));
    memset(chip8->display, 0, sizeof(chip8->display));
    for (int i = 0; i < NELEMS(chip8_fonts); i++) {
        chip8->memory[i] = chip8_fonts[i];
    }
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

