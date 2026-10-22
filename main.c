#include <SDL.h>
#include <stdio.h>
#include <math.h>

#define WIDTH   1280
#define HEIGHT  800


// Pure rainbow color for a hue angle (0..360)
void hue_to_rgb(float hue, Uint8 *r, Uint8 *g, Uint8 *b) {
    float x = 1.0f - fabsf(fmodf(hue / 60.0f, 2.0f) - 1.0f);
    float rf = 0, gf = 0, bf = 0;

    if      (hue < 60)  { rf = 1; gf = x; }
    else if (hue < 120) { rf = x; gf = 1; }
    else if (hue < 180) { gf = 1; bf = x; }
    else if (hue < 240) { gf = x; bf = 1; }
    else if (hue < 300) { rf = x; bf = 1; }
    else                { rf = 1; bf = x; }

    *r = (Uint8)(rf * 255);
    *g = (Uint8)(gf * 255);
    *b = (Uint8)(bf * 255);
}

// Shift one channel toward black or white. l: 0 = black, 0.5 = unchanged, 1 = white
void apply_lightness(Uint8 *c, float l) {
    float v = *c;
    if (l < 0.5f) {
        v = v * (l / 0.5f);
    } else {
        v = v + (255.0f - v) * ((l - 0.5f) / 0.5f);
    }
    *c = (Uint8)v;
}

int main(void) {
    // 1. Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // 2. Create a normal window (title bar stays on screen)
    SDL_Window *win = SDL_CreateWindow("Choose a color for the feeling you are having",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (!win) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // 3. Create renderer
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (!ren) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    // 4. Event loop
    int running = 1;
    int redraw = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_KEYDOWN &&
                       event.key.keysym.sym == SDLK_ESCAPE) {
                running = 0;
            } else if (event.type == SDL_WINDOWEVENT) {
                redraw = 1;
            }
        }

                if (redraw) {
            for (int y = 0; y < HEIGHT; y++) {
                float l = 1.0f - (float)y / (HEIGHT - 1);   // top = white, bottom = black

                for (int x = 0; x < WIDTH; x++) {
                    float hue = (float)x / WIDTH * 300.0f;

                    Uint8 r, g, b;
                    hue_to_rgb(hue, &r, &g, &b);

                    apply_lightness(&r, l);
                    apply_lightness(&g, l);
                    apply_lightness(&b, l);

                    SDL_SetRenderDrawColor(ren, r, g, b, 255);
                    SDL_RenderDrawPoint(ren, x, y);
                }
            }
            SDL_RenderPresent(ren);
            redraw = 0;
        }

        SDL_Delay(16);
    }
           

    // 5. Cleanup, in reverse order of creation
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
