/*
 * Main graphics file for chip8 emulator
 * Uses SDL2 graphics library
 */
#ifndef CHIP8_GFX_H
#define CHIP8_GFX_H

#include <SDL2/SDL.h>


void gfx_init();
void gfx_update(uint8_t *);
void gfx_quit();

#endif // CHIP8_GFX_H