#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_ROM_SIZE (0xFFF - 0x200)

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./chip8 rom_file");
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "rb");
    uint8_t buf[MAX_ROM_SIZE] = {0};
    fread(buf, sizeof(buf), sizeof(buf[0]), file);

    for (size_t i = 0; i < sizeof(buf); i += 2) {
        uint16_t high = buf[i] << 8;
        uint16_t opcode = high | buf[i + 1];
        printf("%#x\n", opcode);
    }


    fclose(file);
    return EXIT_SUCCESS;
}