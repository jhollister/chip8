#pragma once
#include <stdint.h>
#include <stdbool.h>

#define CHIP8_ROM_START     0x200
#define CHIP8_MAX_ROM_SIZE  (0xFFF - 0x200)
#define CHIP8_MEMORY_SIZE   4096
#define CHIP8_REG_COUNT     16

typedef struct chip8_cpu_s {
    uint8_t vx[CHIP8_REG_COUNT];
    uint16_t i;
    uint8_t pc;
    uint8_t sp;
} Chip8_Cpu;

typedef struct chip8_s {
    uint8_t memory[CHIP8_MEMORY_SIZE];
    Chip8_Cpu cpu;
} Chip8;

void chip8_cpu_intialize();
void chip8_execute();

void chip8_initialize(Chip8 *chip8);
bool chip8_load(Chip8 *chip8, char *file_name);
void chip8_run(Chip8 *chip8);
