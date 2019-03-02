#include <stdlib.h>
#include <SDL2/SDL_events.h>

#include "chip8_gfx.h"
#include "chip8.h"

static SDL_Window *window;
static SDL_Renderer *render;

void gfx_check_events(uint16_t *keys) {
    SDL_Event event;
    *keys = 0;
    if (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                gfx_quit(EXIT_SUCCESS);
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_1:
                        *keys |= (1 << 0x1);
                        break;
                    case SDLK_2:
                        *keys |= (1 << 0x2);
                        break;
                    case SDLK_3:
                        *keys |= (1 << 0x3);
                        break;
                    case SDLK_4:
                        *keys |= (1 << 0xc);
                        break;
                    case SDLK_q:
                        *keys |= (1 << 0x4);
                        break;
                    case SDLK_w:
                        *keys |= (1 << 0x5);
                        break;
                    case SDLK_e:
                        *keys |= (1 << 0x6);
                        break;
                    case SDLK_r:
                        *keys |= (1 << 0xd);
                        break;
                    case SDLK_a:
                        *keys |= (1 << 0x7);
                        break;
                    case SDLK_s:
                        *keys |= (1 << 0x8);
                        break;
                    case SDLK_d:
                        *keys |= (1 << 0x9);
                        break;
                    case SDLK_f:
                        *keys |= (1 << 0xe);
                        break;
                    case SDLK_z:
                        *keys |= (1 << 0xa);
                        break;
                    case SDLK_x:
                        *keys |= (1 << 0x0);
                        break;
                    case SDLK_c:
                        *keys |= (1 << 0xb);
                        break;
                    case SDLK_v:
                        *keys |= (1 << 0xf);
                        break;
                    case SDLK_ESCAPE:
                        gfx_quit(EXIT_SUCCESS);
                        break;
                }
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

void gfx_quit(int code) {
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(code);
}