#pragma once
#include <stdint.h>
#include <stdbool.h>

#define CHIP8_ROM_START     0x200
#define CHIP8_MAX_ROM_SIZE  (0xFFF - 0x200)
#define CHIP8_MEMORY_SIZE   4096
#define CHIP8_REG_COUNT     16
#define CHIP8_STACK_SIZE    16
#define CHIP8_FONT_HEIGHT   5
#define CHIP8_DISP_WIDTH    64
#define CHIP8_DISP_HEIGHT   32
#define CHIP8_DISP_PIXELS   (CHIP8_DISP_WIDTH * CHIP8_DISP_HEIGHT)


struct chip8 {
    uint8_t memory[CHIP8_MEMORY_SIZE];
    uint8_t v[CHIP8_REG_COUNT];
    uint8_t stack[CHIP8_STACK_SIZE];
    uint8_t display[CHIP8_DISP_PIXELS];
    /* keys is set in chip8_run() using gfx lib, each bit represents one of 16 keys */
    uint16_t keys;
    uint16_t i;
    uint16_t pc;
    uint8_t sp;
    uint8_t dt;
    uint8_t st;
};

void cpu_initialize(struct chip8 *chip8);
bool cpu_execute(struct chip8 *chip8);

bool chip8_load(struct chip8 *chip8, const char *file_name);
void chip8_run(struct chip8 *chip8);
void chip8_disassemble(struct chip8 *chip8);
