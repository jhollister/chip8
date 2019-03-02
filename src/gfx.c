#include <stdlib.h>

#include "chip8_gfx.h"
#include "chip8.h"

static SDL_Window *window;
static SDL_Renderer *render;

void gfx_check_events(uint16_t *keys) {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                gfx_quit();
                exit(EXIT_SUCCESS);
                break;
            case SDL_KEYDOWN:
                break;
            default:
                SDL_Log("Unknow SDL event\n");
                break;
        }
    }
}

void gfx_init() {
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    window = SDL_CreateWindow("CHIP8 Emulator",
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               WINDOW_WIDTH,
                               WINDOW_HEIGHT,
                               SDL_WINDOW_OPENGL);
    if (window == NULL) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void gfx_update(uint8_t *disp_buf) {
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.h = PIXEL_SIZE;
    rect.w = PIXEL_SIZE;
    // Set background to black
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    SDL_RenderClear(render);
    for (int y = 0; y < CHIP8_DISP_HEIGHT; y++) {
        for (int x = 0; x < CHIP8_DISP_WIDTH; x++) {
            // create a white rectangle if the pixel is set
            if (disp_buf[y*CHIP8_DISP_WIDTH + x]) {
                SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
                rect.x = x*PIXEL_SIZE;
                rect.y = y*PIXEL_SIZE;
                SDL_RenderFillRect(render, &rect);
            }
        }
    }
    // Render to screen
    SDL_RenderPresent(render);
    SDL_Delay(2);
}

void gfx_quit() {
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
}