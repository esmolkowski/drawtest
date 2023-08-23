#ifndef RENDERING_H
#define RENDERING_H

#include <stdbool.h>
#include "SDL.h"
#include "app.h"

typedef struct DrawColor {
    unsigned int r;
    unsigned int g;
    unsigned int b;
    unsigned int a;
} DrawColor;

typedef struct Renderer {
    SDL_Renderer * sdl_renderer;
    DrawColor * draw_color;
} Renderer;

void renderer_set_color(Renderer * renderer, unsigned int r, unsigned int g, unsigned int b, unsigned int a);
void renderer_draw_point(Renderer * renderer, Properties * properties, int x, int y);

#endif