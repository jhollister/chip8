#include <stdio.h>
#include <stdlib.h>

#include "chip8.h"
#include "opcodes.h"

#define REGX(x) ((x & 0x0F00) >> 8)
#define REGY(y) ((y & 0x00F0) >> 4)
#define VAL(x) (x & 0x00FF)
#define ADDR(x) (x & 0x0FFF)
#define COORD(x, y) (y*CHIP8_DISP_WIDTH + x)

#define PC_STEP 2


/*
 * OPCODE: 0nnn - SYS addr
 */
void op_sys_addr(struct chip8 *cpu, uint16_t addr) {
    printf("SYS %#5x\t; Ignored\n", addr);
    cpu->pc += PC_STEP;
}

/*
 * OPCODE: 00E0 - CLS
 */
void op_cls(struct chip8 *cpu) {
    printf("CLS\t; Clear display\n");
    for (int i = 0; i < CHIP8_DISP_PIXELS; i++) {
        cpu->display[i] = 0;
    }
    cpu->pc += PC_STEP;
}

/*
 * OPCODE: 00EE - RET
 */
void op_ret(struct chip8 *cpu) {
    printf("RET\t; Return from subroutine\n");
    if (cpu->sp > 0) {
        cpu->pc = cpu->stack[--(cpu->sp)];
    }
    else {
        fprintf(stderr, "ERROR op_ret: Nothing to return to\n");
        exit(EXIT_FAILURE);
    }
}

/*
 * OPCODE: 1nnn - JMP addr
 */
void op_jmp(struct chip8 *cpu, uint16_t addr) {
    printf("JMP %#5x\t; Jump to location %#5x\n", addr, addr);
    cpu->pc = addr;
}

/*
 * OPCODE: 2nnn - CALL addr
 */
void op_call_addr(struct chip8 *cpu, uint16_t addr) {
    printf("CALL %#5x\t; Call subroutine at %#5x\n", addr, addr);
    if (cpu->sp < CHIP8_STACK_SIZE) {
        cpu->stack[(cpu->sp)++] = cpu->pc + PC_STEP;
        cpu->pc = addr;
    }
    else {
        fprintf(stderr, "ERROR op_call_addr: Stack overflow\n");
        exit(EXIT_FAILURE);
    }
}

/*
 * OPCODE: 3xkk - SE Vx, val
 */
void op_se_vx_val(struct chip8 *cpu, uint8_t reg, uint8_t val) {
    printf("SE V%x, %d\t; Skip next instruction if V%x == %d\n", reg, val, reg, val);
    if (cpu->v[reg] == val) {
        cpu->pc += 2*PC_STEP;
    }
    else {
        cpu->pc += PC_STEP;
    }
}

/*
 * OPCODE: 4xkk - SNE Vx, val
 */
void op_sne_vx_val(struct chip8 *cpu, uint8_t reg, uint8_t val) {
    printf("SNE V%x, %d\t; Skip next instruction if V%x != %d\n", reg, val, reg, val);
    if (cpu->v[reg] != val) {
        cpu->pc += 2*PC_STEP;
    }
    else {
        cpu->pc += PC_STEP;
    }
}

/*
 * OPCODE:  5xy0 - SE Vx, Vy
 */
void op_se_vx_vy(struct chip8 *cpu, uint8_t regx, uint8_t regy) {
    printf("SE V%x, V%x\t; Skip next instruction if V%x == V%x\n", regx, regy, regx, regy);
    if (cpu->v[regx] == cpu->v[regy]) {
        cpu->pc += 2*PC_STEP;
    }
    else {
        cpu->pc += PC_STEP;
    }
}

/*
 * OPCODE: 6xkk - LD Vx, val
 */
void op_ld_vx_val(struct chip8 *cpu, uint8_t reg, uint16_t val) {
    printf("LD V%x, %d\t; Set V%x = %d\n", reg, val, reg, val);
    cpu->v[reg] = val;
    cpu->pc += PC_STEP;
}

/*
 * OPCODE: 7xkk - ADD Vx, val
 */
void op_add_vx_val(struct chip8 *cpu, uint8_t reg, uint8_t val) {
    printf("ADD V%x, %d\t; Set V%x += %d\n", reg, val, reg, val);
    cpu->v[reg] += val;
    cpu->pc += PC_STEP;
}

/*
 * OPCODE: 8xy0 - LD Vx, Vy
 */
void op_ld_vx_vy(struct chip8 *cpu, uint8_t regx, uint8_t regy) {
    printf("LD V%x, V%x\t; Set V%x = V%x\n", regx, regy, regx, regy);
    cpu->v[regx] = cpu->v[regy];
    cpu->pc += PC_STEP;
}

/*
 * OPCODE: 8xy1 - OR Vx, Vy
 */
void op_or_vx_vy(struct chip8 *cpu, uint8_t regx, uint8_t regy) {
    printf("OR V%x, V%x\t; Set V%x |= V%x\n", regx, regy, regx, regy);
    cpu->v[regx] |= cpu->v[regy];
    cpu->pc += PC_STEP;
}

/*
 * OPCODE: 8xy2 - AND Vx, Vy
 */
void op_and_vx_vy(struct chip8 *cpu, uint8_t regx, uint8_t regy) {
    printf("AND V%x, V%x\t; Set V%x &= V%x\n", regx, regy, regx, regy);
    cpu->v[regx] &= cpu->v[regy];
    cpu->pc += PC_STEP;
}

/*
 * OPCODE: 8xy3 - XOR Vx, Vy
 */
void op_xor_vx_vy(struct chip8 *cpu, uint8_t regx, uint8_t regy) {
    printf("XOR V%x, V%x\t; Set V%x ^= V%x\n", regx, regy, regx, regy);
    cpu->v[regx] ^= cpu->v[regy];
    cpu->pc += PC_STEP;
}

/*
 * OPCODE: 8xy4 - ADD Vx, Vy
 */
void op_add_vx_vy(struct chip8 *cpu, uint8_t regx, uint8_t regy) {
    printf("ADD V%x, V%x\t; Set V%x += V%x\n", regx, regy, regx, regy);
    uint16_t result = (uint16_t) cpu->v[regx] + cpu->v[regy];
    /* Set Vf on overflow */
    cpu->v[0xF] = result > 0xFF ? 1 : 0;
    cpu->v[regx] = (uint8_t) result;
    cpu->pc += PC_STEP;
}

/*
 * OPCODE: 8xy5 - SUB Vx, Vy
 */
void op_sub_vx_vy(struct chip8 *cpu, uint8_t regx, uint8_t regy) {
    printf("SUB V%x, V%x\t; Set V%x -= V%x\n", regx, regy, regx, regy);
    /* Set Vf when no borrow */
    cpu->v[0xF] = cpu->v[regx] > cpu->v[regy] ? 1 : 0;
    cpu->v[regx] -= cpu->v[regy];
    cpu->pc += PC_STEP;
}

/*
 * OPCODE: 8xy6 - SHR Vx {, Vy}
 */
void op_shr_vx_vy(struct chip8 *cpu, uint8_t regx, uint8_t regy) {
    printf("SHR V%x, V%x\t; Set V%x >>= 1\n", regx, regy, regx);
    /* Set Vf if least significant bit of Vx is 1 */
    cpu->v[0xF] = cpu->v[regx] & 1;
    cpu->v[regx] >>= 1;
    cpu->pc += PC_STEP;
}

/*
 * OPCODE: 8xy7 - SUBN Vx, Vy
 */
void op_subn_vx_vy(struct chip8 *cpu, uint8_t regx, uint8_t regy) {
    printf("SUBN V%x, V%x\t; Set V%x = V%x - V%x\n", regx, regy, regx, regy, regx);
    /* If Vy > Vx set Vf */
    cpu->v[0xF] = cpu->v[regy] > cpu->v[regx] ? 1 : 0;
    cpu->v[regx] = cpu->v[regy] - cpu->v[regx];
    cpu->pc += PC_STEP;
}

/*
 * OPCODE: 8xyE - SHL Vx {, Vy}
 */
void op_shl_vx_vy(struct chip8 *cpu, uint8_t regx, uint8_t regy) {
    printf("SHL V%x, V%x\t; Set V%x <<= 1\n", regx, regy, regx, regy);
    /* set Vf if most significant digit of Vx is 1 */
    cpu->v[0xF] = cpu->v[regx] & 0x80 ? 1 : 0;
    cpu->v[regx] <<= 1;
    cpu->pc += PC_STEP;
}

/*
 * OPCODE: 9xy0 - SNE Vx, Vy
 */
void op_sne_vx_vy(struct chip8 *cpu, uint8_t regx, uint8_t regy) {
    printf("SNE V%x, V%x\t; Skip next instruction if V%x == V%x\n", regx, regy, regx, regy);
    if (cpu->v[regx] != cpu->v[regy]) {
        cpu->pc += 2*PC_STEP;
    }
    else {
        cpu->pc += PC_STEP;
    }
}

/*
 * OPCODE: Annn - LD, I, addr
 */
void op_ld_i_addr(struct chip8 *cpu, uint16_t addr) {
    printf("LD I, %#5x\t; Set I = %#5x\n", addr, addr);
    cpu->i = addr;
    cpu->pc += PC_STEP;
}

/*
 * OPCODE: Bnnn - JMP V0, addr
 */
void op_jmp_v0_addr(struct chip8 *cpu, uint16_t addr) {
    printf("JMP V0, %#5x\t; Jump to location %#5x + value in V0\n", addr, addr);
    cpu->pc = cpu->v[0] + addr;
}

/*
 * OPCODE: Cxkk - RND Vx, val
 */
void op_rnd_vx_val(struct chip8 *cpu, uint8_t reg, uint8_t val) {
    printf("RND V%x, %d\t; Set V%x = %d & RAND\n", reg, val, reg, val);
    cpu->v[reg] = (uint8_t)(rand() % 255) & val;
    cpu->pc += PC_STEP;
}

/*
 * OPCODE: Dxyn - DRW Vx, Vy, nibble
 */
void op_drw_vx_vy_nibble(struct chip8 *cpu, uint8_t regx, uint8_t regy, uint8_t nibble) {
    printf("DRW V%x, V%x, %d\t; Display %d-byte sprite at memory location I at (V%x, V%x)\n",
            regx, regy, nibble, nibble, regx, regy);
    bool pixel_erased = false; /* set if any pixel is flipped from 1 to 0 */
    uint16_t addr = cpu->i;
    uint8_t x_pos = cpu->v[regx];
    uint8_t y_pos = cpu->v[regy];
    uint8_t *display = cpu->display;
    uint8_t sprite_row = cpu->memory[addr++];
    for (int i = 0; i < nibble; i++) {
        // for each bit
        for (int j = 0; j < 8; j++) {
            if (display[COORD(x_pos, y_pos)] && !(sprite_row & 0x80)) {
                pixel_erased = true;
            }
            display[COORD(x_pos, y_pos)] ^= sprite_row & 0x80;
            sprite_row <<= 1;
            x_pos = (x_pos + 1) % CHIP8_DISP_WIDTH;
        }
        x_pos = cpu->v[regx];
        sprite_row = cpu->memory[addr++];
        y_pos = (y_pos + 1) % CHIP8_DISP_HEIGHT;
    }
    cpu->pc += PC_STEP;
    cpu->v[0xF] = pixel_erased ? 1 : 0;
    chip8_dbg_drw(cpu);
}

/*
 * OPCODE: Ex9E - SKP Vx
 */
void op_skp_vx(struct chip8 *cpu, uint8_t reg) {
    printf("SKP V%x\t; Skip next instruction if key pressed with value of V%x\n", reg, reg);
    cpu->pc = (cpu->keys & (1 << cpu->v[reg])) ? cpu->pc + 2*PC_STEP : cpu->pc + PC_STEP;
}

/*
 * OPCODE: ExA1 - SKNP Vx
 */
void op_sknp_vx(struct chip8 *cpu, uint8_t reg) {
    printf("SKNP V%x\t; Skip next instruction if key not pressed with value of V%x\n", reg, reg);
    cpu->pc = !(cpu->keys & (1 << cpu->v[reg])) ? cpu->pc + 2*PC_STEP : cpu->pc + PC_STEP;
}

/*
 * OPCODE: Fx07 - LD Vx, DT
 */
void op_ld_vx_dt(struct chip8 *cpu, uint8_t reg) {
    printf("LD V%x, DT\t; Set V%x = delay timer value\n", reg, reg);
    cpu->v[reg] = cpu->dt;
    cpu->pc += PC_STEP;
}

/*
 * OPCODE: Fx0A - LD Vx, K
 */
void op_ld_vx_k(struct chip8 *cpu, uint8_t reg) {
    printf("LD V%x, K\t; Wait for a key press, store the value in V%x\n", reg, reg);
    uint16_t keys = cpu->keys;
    if (keys) {
        for (int i = 0; i < sizeof(keys)*8; i++) {
            if (keys & 1) {
                // set register to the value of the key pressed
                cpu->v[reg] = i;
            }
            keys >> 1;
        }
        cpu->pc += PC_STEP;
    }
    /* Return without changing pc to wait for a key press */
}

/*
 * OPCODE: Fx15 - LD DT, Vx
 */
void op_ld_dt_vx(struct chip8 *cpu, uint8_t reg) {
    printf("LD DT, V%x\t; Set delay timer = V%x\n", reg, reg);
    cpu->dt = cpu->v[reg];
    cpu->pc += PC_STEP;
}

/*
 * OPCODE: Fx18 - LD ST, Vx
 */
void op_ld_st_vx(struct chip8 *cpu, uint8_t reg) {
    printf("LD ST, V%x\t; Set sound timer = V%x\n", reg, reg);
    cpu->st = cpu->v[reg];
    cpu->pc += PC_STEP;
}

/*
 * OPCODE: Fx1E - ADD I, Vx
 */
void op_add_i_vx(struct chip8 *cpu, uint8_t reg) {
    printf("ADD I, V%x\t; Set sound timer = V%x\n", reg, reg);
    cpu->i += cpu->v[reg];
    cpu->pc += PC_STEP;
}

/*
 * OPCODE: Fx29 - LD F, Vx
 */
void op_ld_f_vx(struct chip8 *cpu, uint8_t reg) {
    printf("LD F, V%x \t; Set I = location of sprite for digit in V%x\n", reg, reg);
    if (cpu->v[reg] > 0xF) {
        fprintf(stderr, "ERROR op_ld_f_vs: Invalid sprite value %d\n", cpu->v[reg]);
        exit(EXIT_FAILURE);
    }
    cpu->i = cpu->v[reg]*CHIP8_FONT_HEIGHT;
    cpu->pc += PC_STEP;
}

/*
 * OPCODE: Fx33 - LD B, Vx
 */
void op_ld_b_vx(struct chip8 *cpu, uint8_t reg) {
    printf("LD B, V%x\t; Store BCD representation of V%x in I\n", reg, reg);
    uint16_t i = cpu->i + 2;
    uint8_t num = cpu->v[reg];
    /* Note: since Vx is 8-bit value we don't worry about any number over 3 digits */
    for (int count = 0; count < 3; count++) {
        cpu->memory[i--] = num % 10;
        num /= 10;
    }
    cpu->pc += PC_STEP;
}

/*
 * OPCODE: Fx55 - LD [I], Vx
 */
void op_ld_i_vx(struct chip8 *cpu, uint8_t reg) {
    printf("LD [I], V%x\t; Store registers V0 through V%x in I\n", reg, reg);
    // Store registers V0 through Vx in memory starting at location I
    uint16_t i = cpu->i;
    for (int x = 0; x <= reg; x++) {
        cpu->memory[i++] = cpu->v[x];
    }
    cpu->pc += PC_STEP;
}

/*
 * OPCODE: Fx65 - LD Vx, [I]
 */
void op_ld_vx_i(struct chip8 *cpu, uint8_t reg) {
    printf("LD V%x, [I]\t; Read registers V0-V%x from memory starting at I\n", reg, reg);
    // Read regisers V0 through Vx from memory starting at location I
    uint16_t i = cpu->i;
    for (int x = 0; x <= reg; x++) {
        cpu->v[x] = cpu->memory[i++];
    }
    cpu->pc += PC_STEP;
}

void opcode0(struct chip8 *cpu, uint16_t opcode) {
    if ( (opcode & 0x00FF) == 0xE0) {
        op_cls(cpu);
    }
    else if ( (opcode & 0x00FF) == 0xEE) {
        op_ret(cpu);
    }
    else {
        uint16_t addr = opcode & 0x0FFF;
        op_sys_addr(cpu, addr);
    }
}

void opcode1(struct chip8 *cpu, uint16_t opcode) {
    uint16_t addr = ADDR(opcode);
    op_jmp(cpu, addr);
}

void opcode2(struct chip8 *cpu, uint16_t opcode) {
    uint16_t addr = ADDR(opcode);
    op_call_addr(cpu, addr);
}

void opcode3(struct chip8 *cpu, uint16_t opcode) {
    uint8_t reg = REGX(opcode);
    uint16_t val = VAL(opcode);
    op_se_vx_val(cpu, reg, val);
}

void opcode4(struct chip8 *cpu, uint16_t opcode) {
    uint8_t reg = REGX(opcode);
    uint8_t val = VAL(opcode);
    op_sne_vx_val(cpu, reg, val);
}

void opcode5(struct chip8 *cpu, uint16_t opcode) {
    uint8_t regx = REGX(opcode);
    uint8_t regy = REGY(opcode);
    op_se_vx_vy(cpu, regx, regy);

}

void opcode6(struct chip8 *cpu, uint16_t opcode) {
    uint8_t reg = REGX(opcode);
    uint8_t val = VAL(opcode);
    op_ld_vx_val(cpu, reg, val);
}

void opcode7(struct chip8 *cpu, uint16_t opcode) {
    uint8_t reg = REGX(opcode);
    uint8_t val = VAL(opcode);
    op_add_vx_val(cpu, reg, val);
}

void opcode8(struct chip8 *cpu, uint16_t opcode) {
    uint8_t nibble = (opcode & 0x000F);
    uint8_t regx = REGX(opcode);
    uint8_t regy = REGY(opcode);
    switch (nibble) {
        case 0x0:
        {
            op_ld_vx_vy(cpu, regx, regy);
            break;
        }
        case 0x1:
        {
            op_or_vx_vy(cpu, regx, regy);
            break;
        }
        case 0x2:
        {
            op_and_vx_vy(cpu, regx, regy);
            break;
        }
        case 0x3:
        {
            op_xor_vx_vy(cpu, regx, regy);
            break;
        }
        case 0x4:
        {
            op_add_vx_vy(cpu, regx, regy);
            break;
        }
        case 0x5:
        {
            op_sub_vx_vy(cpu, regx, regy);
            break;
        }
        case 0x6:
        {
            op_shr_vx_vy(cpu, regx, regy);
            break;
        }
        case 0x7:
        {
            op_subn_vx_vy(cpu, regx, regy);
            break;
        }
        case 0xE:
        {
            op_shl_vx_vy(cpu, regx, regy);
            break;
        }
        default:
        {
            op_sys_addr(cpu, opcode & 0x0FFF);
            break;
        }
    }
}

void opcode9(struct chip8 *cpu, uint16_t opcode) {
    uint8_t regx = REGX(opcode);
    uint8_t regy = REGY(opcode);
    op_sne_vx_vy(cpu, regx, regy);
}

void opcodeA(struct chip8 *cpu, uint16_t opcode) {
    uint16_t addr = ADDR(opcode);
    op_ld_i_addr(cpu, addr);
}

void opcodeB(struct chip8 *cpu, uint16_t opcode) {
    uint16_t addr = ADDR(opcode);
    op_jmp_v0_addr(cpu, addr);
}

void opcodeC(struct chip8 *cpu, uint16_t opcode) {
    uint8_t reg = REGX(opcode);
    uint8_t val = VAL(opcode);
    op_rnd_vx_val(cpu, reg, val);

}

void opcodeD(struct chip8 *cpu, uint16_t opcode) {
    uint8_t regx = REGX(opcode);
    uint8_t regy = REGY(opcode);
    uint8_t nibble = opcode & 0x000F;
    op_drw_vx_vy_nibble(cpu, regx, regy, nibble);
}

void opcodeE(struct chip8 *cpu, uint16_t opcode) {
    uint8_t reg = REGX(opcode);
    switch (opcode & 0x00FF) {
        case 0x9E:
        {
            op_skp_vx(cpu, reg);
            break;
        }
        case 0xA1:
        {
            op_sknp_vx(cpu, reg);
            break;
        }
        default:
        {
            op_sys_addr(cpu, opcode & 0x0FFF);
            break;
        }
    }
    
}

void opcodeF(struct chip8 *cpu, uint16_t opcode) {
    uint8_t val = VAL(opcode);
    uint8_t reg = REGX(opcode);
    switch (val) {
        case 0x07:
        {
            op_ld_vx_dt(cpu, reg);
            break;
        }
        case 0x0A:
        {
            op_ld_vx_k(cpu, reg);
            break;
        }
        case 0x15:
        {
            op_ld_dt_vx(cpu, reg);
            break;
        }
        case 0x18:
        {
            op_ld_st_vx(cpu, reg);
            break;
        }
        case 0x1E:
        {
            op_add_i_vx(cpu, reg);
            break;
        }
        case 0x29:
        {
            op_ld_f_vx(cpu, reg);
            break;
        }
        case  0x33:
        {
            op_ld_b_vx(cpu, reg);
            break;
        }
        case 0x55:
        {
            op_ld_i_vx(cpu, reg);
            break;
        }
        case 0x65:
        {
            op_ld_vx_i(cpu, reg);
            break;
        }
        default:
        {
            op_sys_addr(cpu, opcode & 0x0FFF);
            break;
        }

    }
}
