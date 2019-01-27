#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

#include "chip8.h"

void chip8_initialize(struct chip8_s *chip8) {
    srand(time(0));
    cpu_initialize(chip8);
}

bool chip8_load(struct chip8_s *chip8, const char *file_name) {
    FILE *file = fopen(file_name, "rb");
    if (file == NULL) {
        printf("ERROR: Could not open ROM\n");
        return false;
    }
    uint8_t *rom_start = &chip8->memory[CHIP8_ROM_START];
    int size = fread(rom_start, sizeof (rom_start[0]), CHIP8_MAX_ROM_SIZE, file);
    fclose(file);
    if (size == 0) {
        printf("ERROR: Could not read ROM\n");
        return false;
    }
    return true;
}

void chip8_run(struct chip8_s *chip8) {
    while(cpu_execute(chip8));
}