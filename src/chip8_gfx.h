/*
 * Main graphics file for chip8 emulator
 * Uses SDL2 graphics library
 */
#ifndef CHIP8_GFX_H
#define CHIP8_GFX_H

#include <SDL2/SDL.h>

// The multiplier for each pixel
#define PIXEL_SIZE       30
#define WINDOW_WIDTH     (CHIP8_DISP_WIDTH*PIXEL_SIZE)
#define WINDOW_HEIGHT    (CHIP8_DISP_HEIGHT*PIXEL_SIZE)

void gfx_init();
void gfx_update(uint8_t *);
void gfx_quit();
void gfx_check_events(uint16_t *);

#endif // CHIP8_GFX_H