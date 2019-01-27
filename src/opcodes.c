#include <stdio.h>

#include "chip8.h"
#include "opcodes.h"

#define REGX(x) ( (x & 0x0F00) >> 8 )
#define REGY(y) ( (y & 0x00F0) >> 4 )
#define VAL(x) ( x & 0x00FF )
#define ADDR(x) (x & 0x0FFF )

void opcode0(struct chip8_s *cpu, uint16_t opcode) {
    if ( (opcode & 0x00FF) == 0xE0) {
        printf("CLS\t; Clear display\n");
    }
    else if ( (opcode & 0x00FF) == 0xEE) {
        printf("RET\t; Return from subroutine\n");
    }
    else {
        uint16_t addr = opcode & 0x0FFF;
        printf("SYS %#5x\t; Ignored\n", addr);
    }

}

void opcode1(struct chip8_s *cpu, uint16_t opcode) {
    uint16_t addr = ADDR(opcode);
    printf("JMP %#5x\t; Jump to location %#5x\n", addr, addr);
}

void opcode2(struct chip8_s *cpu, uint16_t opcode) {
    uint16_t addr = ADDR(opcode);
    printf("CALL %#5x\t; Call subroutine at %#5x\n", addr, addr);
}

void opcode3(struct chip8_s *cpu, uint16_t opcode) {
    uint8_t reg = REGX(opcode);
    uint16_t val = VAL(opcode);
    printf("SE V%x, %d\t; Skip next instruction if V%x == %d\n", reg, val, reg, val);
}

void opcode4(struct chip8_s *cpu, uint16_t opcode) {
    uint8_t reg = REGX(opcode);
    uint8_t val = VAL(opcode);
    printf("SNE V%x, %d\t; Skip next instruction if V%x != %d\n", reg, val, reg, val);
}

void opcode5(struct chip8_s *cpu, uint16_t opcode) {
    uint8_t reg_x = REGX(opcode);
    uint8_t reg_y = REGY(opcode);
    printf("SE V%x, V%x\t; Skip next instruction if V%x == V%x\n", reg_x, reg_y, reg_x, reg_y);

}

void opcode6(struct chip8_s *cpu, uint16_t opcode) {
    uint8_t reg = REGX(opcode);
    uint8_t val = VAL(opcode);
    printf("LD V%x, %d\t; Set V%x = %d\n", reg, val, reg, val);

}

void opcode7(struct chip8_s *cpu, uint16_t opcode) {
    uint8_t reg = REGX(opcode);
    uint8_t val = VAL(opcode);
    printf("ADD V%x, %d\t; Set V%x += %d\n", reg, val, reg, val);
}

void opcode8(struct chip8_s *cpu, uint16_t opcode) {
    uint8_t nibble = (opcode & 0x000F);
    uint8_t reg_x = REGX(opcode);
    uint8_t reg_y = REGY(opcode);
    switch (nibble) {
        case 0x0:
        {
            printf("LD V%x, V%x\t; Set V%x = V%x\n", reg_x, reg_y, reg_x, reg_y);
            break;
        }
        case 0x1:
        {
            printf("OR V%x, V%x\t; Set V%x |= V%x\n", reg_x, reg_y, reg_x, reg_y);
            break;
        }
        case 0x2:
        {
            printf("AND V%x, V%x\t; Set V%x &= V%x\n", reg_x, reg_y, reg_x, reg_y);
            break;
        }
        case 0x3:
        {
            printf("XOR V%x, V%x\t; Set V%x ^= V%x\n", reg_x, reg_y, reg_x, reg_y);
            break;
        }
        case 0x4:
        {
            printf("ADD V%x, V%x\t; Set V%x += V%x\n", reg_x, reg_y, reg_x, reg_y);
            break;
        }
        case 0x5:
        {
            printf("SUB V%x, V%x\t; Set V%x -= V%x\n", reg_x, reg_y, reg_x, reg_y);
            break;
        }
        case 0x6:
        {
            printf("SHR V%x, V%x\t; Set V%x >>= 1\n", reg_x, reg_y, reg_x);
            break;
        }
        case 0x7:
        {
            printf("SUBN V%x, V%x\t; Set V%x = V%x - V%x\n", reg_x, reg_y, reg_x, reg_y, reg_x);
            break;
        }
        case 0xE:
        {
            printf("SHL V%x, V%x\t; Set V%x <<= 1\n", reg_x, reg_y, reg_x, reg_y);
            break;
        }
        default:
        {
            printf("SYS %#5x\t; Ignored\n", opcode & 0x0FFF);
            break;
        }
    }
}

void opcode9(struct chip8_s *cpu, uint16_t opcode) {
    uint8_t reg_x = REGX(opcode);
    uint8_t reg_y = REGY(opcode);
    printf("SNE V%x, V%x\t; Skip next instruction if V%x == V%x\n", reg_x, reg_y, reg_x, reg_y);
}

void opcodeA(struct chip8_s *cpu, uint16_t opcode) {
    uint16_t addr = ADDR(opcode);
    printf("LD I, %#5x\t; Set I = %#5x\n", addr, addr);
}

void opcodeB(struct chip8_s *cpu, uint16_t opcode) {
    uint16_t addr = ADDR(opcode);
    printf("JMP V0, %d\t; Jump to location %d + value in V0\n");
}

void opcodeC(struct chip8_s *cpu, uint16_t opcode) {
    uint8_t reg = REGX(opcode);
    uint8_t val = VAL(opcode);
    printf("RND V%x, %d\t; Set V%x = %d & RAND\n");
}

void opcodeD(struct chip8_s *cpu, uint16_t opcode) {
    uint8_t reg_x = REGX(opcode);
    uint8_t reg_y = REGY(opcode);
    uint8_t nibble = opcode & 0x000F;
    printf("DRW V%x, V%x, %d\t; Display %d-byte sprite at memory location I at (V%x, V%x)\n",
            reg_x, reg_y, nibble, nibble, reg_x, reg_y);
}

void opcodeE(struct chip8_s *cpu, uint16_t opcode) {
    uint8_t reg = REGX(opcode);
    switch (opcode & 0x00FF) {
        case 0x9E:
        {
            printf("SKP V%x\t; Skip next instruction if key pressed with value of V%x\n", reg, reg);
            break;
        }
        case 0xA1:
        {
            printf("SKNP V%x\t; Skip next instruction if key not pressed with value of V%x\n", reg, reg);
            break;
        }
        default:
        {
            printf("SYS %#5x\t; Ignored\n", opcode & 0x0FFF);
            break;
        }
    }
    
}

void opcodeF(struct chip8_s *cpu, uint16_t opcode) {
    uint8_t val = VAL(opcode);
    uint8_t reg = REGX(opcode);
    switch (val) {
        case 0x07:
        {
            printf("LD V%x, DT\t; Set V%x = delay timer value\n", reg, reg);
            break;
        }
        case 0x0A:
        {
            printf("LD V%x, K\t; Wait for a key press, store the value in V%x\n", reg, reg);
            break;
        }
        case 0x15:
        {
            printf("LD DT, V%x\t; Set delay timer = V%x\n", reg, reg);
            break;
        }
        case 0x18:
        {
            printf("LD ST, V%x\t; Set sound timer = V%x\n", reg, reg);
            break;
        }
        case 0x1E:
        {
            printf("ADD I, V%x\t; Set sound timer = V%x\n", reg, reg);
            break;
        }
        case 0x29:
        {
            printf("LD F, V%x \t; Set I = location of sprite for digit V%x\n", reg, reg);
            break;
        }
        case  0x33:
        {
            printf("LD B, V%x\t; Store BCD representation of V%x in I\n", reg, reg);
            break;
        }
        case 0x55:
        {
            printf("LD [I], V%x\t; Store registers V0 through V%x in I\n", reg, reg);
            break;
        }
        case 0x65:
        {
            printf("LD V%x, [I]\t; Read registers V0-V%x from memory starting at I\n", reg, reg);
            break;
        }
        default:
        {
            printf("SYS %#5x\t; Ignored\n", opcode & 0x0FFF);
            break;
        }

    }
}
