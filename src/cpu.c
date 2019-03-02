#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "chip8.h"
#include "opcodes.h"


#define CHIP8_FONTSET_SIZE 80
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

void (*opcode_handler[])(struct chip8*, uint16_t) = {
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

void cpu_initialize(struct chip8 *chip8) {
    chip8->i = 0;
    chip8->pc = CHIP8_ROM_START;
    chip8->sp = 0;
    chip8->dt = 0;
    chip8->st = 0;
    chip8->keys = 0;
    memset(chip8->memory, 0, sizeof(chip8->memory));
    memset(chip8->v, 0, sizeof(chip8->v));
    memset(chip8->stack, 0, sizeof(chip8->stack));
    memset(chip8->display, 0, sizeof(chip8->display));
    memcpy(chip8->memory, chip8_fonts, sizeof(chip8_fonts));
    assert(chip8->memory[0] == 0xF0);
    assert(chip8->memory[CHIP8_FONTSET_SIZE-1] == 0x80);
}

// TODO: Implement actual timing
void cpu_update_timers(struct chip8 *cpu) {
    static int update_count = 0;
    // every 100 cpu ticks (because why not)
    if (update_count == 100) {
        if (cpu->dt) cpu->dt--;
        if (cpu->st) {
            cpu->st--;
            printf("BEEP\n");
        }
        update_count = 0;
    }
    update_count++;
}

/**
 * Executes a single instruction of the cpu pointed at by pc, the program counter.
 * Returns false if invalid instruction.
 **/
bool cpu_execute(struct chip8 *cpu) {
    if (cpu->pc > CHIP8_MEMORY_SIZE) {
        fprintf(stderr, "Program counter exceeded memory %d\n", cpu->pc);
        return false;
    }
    uint16_t opcode = (cpu->memory[cpu->pc] << 8) | cpu->memory[cpu->pc + 1];
    printf("%#06x: opcode: %#05x ", cpu->pc, opcode);
    uint8_t instr_code = (opcode & 0xF000) >> 12;
    opcode_handler[instr_code](cpu, opcode);
    cpu_update_timers(cpu);
    return true;
}

