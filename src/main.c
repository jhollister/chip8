#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "chip8.h"

#define MAX_ROM_SIZE (0xFFF - 0x200)

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./chip8 <file_name>\n");
        return EXIT_FAILURE;
    }

    char *file_name = argv[1];
    struct chip8 chip8;
    if (chip8_load(&chip8, file_name)) {
        chip8_disassemble(&chip8);
        // chip8_run(&chip8);
    }
    else {
        printf("Error: Could not load ROM file: %s\n", file_name);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
