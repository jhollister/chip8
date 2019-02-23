#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

#include "chip8.h"

static int rom_size = 0;

bool chip8_load(struct chip8 *chip8, const char *file_name) {
    srand(time(0));
    cpu_initialize(chip8);
    FILE *file = fopen(file_name, "rb");
    if (file == NULL) {
        printf("ERROR: Could not open ROM\n");
        return false;
    }
    uint8_t *rom_start = &chip8->memory[CHIP8_ROM_START];
    rom_size = fread(rom_start, sizeof (rom_start[0]), CHIP8_MAX_ROM_SIZE, file);
    fclose(file);
    if (rom_size == 0) {
        printf("ERROR: Could not read ROM\n");
        return false;
    }
    return true;
}

/*
 * Prints out the disassembly of the ROM that has been loaded
 * with chip8_load() to stdout
 */
void chip8_disassemble(struct chip8 *chip8) {
    uint16_t prev_pc = chip8->pc;
    // execute but ignore any pc changes the cpu makes
    while (cpu_execute(chip8) && (prev_pc-CHIP8_ROM_START) < rom_size) {
        // printf("PC: %x\n", chip8->pc);
        chip8->pc = prev_pc + 1;
        prev_pc = chip8->pc;
    }

}

void chip8_run(struct chip8 *chip8) {
    while(cpu_execute(chip8));
}