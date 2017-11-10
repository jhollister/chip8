#ifndef CHIP8_H
#define CHIP8_H

class chip8 {
    unsigned char memory[4096];
    unsigned char V_reg[16];
    unsigned short I_reg;
    unsigned short pc;
    unsigned char delay_timer;
    unsigned char sound_timer;
    unsigned short stack[16];
    unsigned char sp;

}

#endif
