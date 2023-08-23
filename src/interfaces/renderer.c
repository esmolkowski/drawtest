#include "SDL.h"
#include "interfaces/renderer.h"

void renderer_set_color(Renderer * renderer, unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
    renderer->draw_color->r = r;
    renderer->draw_color->g = g;
    renderer->draw_color->b = b;
    renderer->draw_color->a = a;

    SDL_SetRenderDrawColor(renderer->sdl_renderer, r, g, b, a);
}

void term_draw_point(int x, int y) {
    // make x twice as wide for a more square looking display.
    x = x*2;
    //printf("\x1b[%d;%dH##",y,x);
}

void renderer_draw_point(Renderer * renderer, Properties * properties, int x, int y)
{
    // this should be handled by draw_offline_line?? but it's not working right so
    // not drawing if out of bounds as a temporary fix.
    if (abs(x) > properties->RENDER_WIDTH || abs(y) > properties->RENDER_HEIGHT) {
        return;
    }
    if (x < 0 || y < 0) {
        return;
    }

    // Draw points
    SDL_RenderDrawPoint(renderer->sdl_renderer,
        x, y
    );

    term_draw_point(x, y);
}