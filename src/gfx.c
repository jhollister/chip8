#include <stdlib.h>

#include "chip8_gfx.h"

static SDL_Window *window;
static SDL_Renderer *renderer;

void gfx_init() {
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    window = SDL_CreateWindow("CHIP8 Emulator",
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               640,
                               320,
                               SDL_WINDOW_OPENGL);
    if (window == NULL) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void gfx_update(uint8_t *disp_buf) {
    for (int x = 0; x < 640; x++) {
        for (int y = 0; y < 320; y++) {
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    SDL_Delay(100);
}

void gfx_quit() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}