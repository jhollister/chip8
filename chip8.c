#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <chip8.h>

void chip8_initialize(Chip8 *chip8) {
    return;
}

bool chip8_load(Chip8 *chip8, char *file_name) {
    FILE *file = fopen(file_name, "rb");
    if (file == NULL) {
        printf("ERROR: Could not open ROM\n");
        return false;
    }
    uint8_t *rom_start = &chip8->memory[CHIP8_ROM_START];
    int size = fread(rom_start, sizeof (rom_start[0]), CHIP8_MAX_ROM_SIZE, file);
    if (size == 0) {
        printf("ERROR: Could not read ROM\n");
        return false;
    }
    return true;
}

void chip8_run(Chip8 *chip8) {
    return;
}