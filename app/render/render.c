#include "SDL.h"
#include "../app.h"

/*
typedef struct Coordinate {
    int x;
    int y;
} Coordinate;

Coordinate render_adjust_center(Properties properties, int x, int y)
{
    Coordinate coordinate;
    coordinate.x = x + properties.RENDER_HALF_WIDTH;
    coordinate.y = properties.RENDER_HALF_HEIGHT - y;
    return coordinate;
}

void render_draw_point(SDL_Renderer *renderer, Properties properties, int x, int y)
{
    printf("drawing: %d,%d\n", x, y);
    Coordinate coordinate = render_adjust_center(properties, x, y);
    SDL_RenderDrawPoint(renderer, coordinate.x, coordinate.y);
}

void drawframe(SDL_Renderer *renderer, int frame, Properties properties, Scene *scene)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    
}
*/

void render_drawframe(SDL_Renderer *renderer, int frame, Properties properties) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderDrawPoint(renderer, 1, 1);
}