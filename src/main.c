#include "SDL.h"
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include "app.h"
#include "datastructures/linked_list.h"
#include "environment/environment.h"
#include "environment/tick.h"
#include "environment/models.h"
#include "render/render.h"
#include "vector.h"
// WINDOWS
//#include <windows.h>

//int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevIns, LPSTR lpszArgument, int iShow)
int main(int argc, char *argv[])
{
    Properties properties;
    properties.RENDER_WIDTH = 320;
    properties.RENDER_HEIGHT = 240;
    properties.RENDER_HALF_WIDTH = 320/2;
    properties.RENDER_HALF_HEIGHT = 240/2;
    properties.WINDOW_SCALE = 2.0;

    // used for FPS tracking
    properties.previous_frame = 0;
    properties.previous_tick = 0;
    
    properties.tickrate = 64;
    properties.ticktime = 1.0/((float)properties.tickrate);


    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "drawtest",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, // not sure if the float casting is needed..
        (int)((float)properties.RENDER_WIDTH*properties.WINDOW_SCALE),
        (int)((float)properties.RENDER_HEIGHT*properties.WINDOW_SCALE),
        0
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_RenderSetScale(renderer, properties.WINDOW_SCALE, properties.WINDOW_SCALE);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    // Set up demo environment
    Environment *environment = environment_create_environment();
    environment_add_entity(environment, entity_create(
        models_create_pyramid(10), 'm'
    ));
    listNode *ent2 = environment_add_entity(environment, entity_create(
        models_create_cube(200), 'm'
    ));

    int frame = 0;
    while (true)
    {
        SDL_Event event;
        Camera *camera = environment->camera;
        if (SDL_PollEvent(&event))
        {
            tick_handle_inputs(environment, properties, event);
            // Seperate check for quit
            if (event.type == SDL_QUIT)
            {
                break;
            }
        }
        // draw frame
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        tick_run(&properties, environment);

        uint32_t frametime = SDL_GetTicks()-properties.previous_frame;
        //printf("FRAMETIME: %d FPS:%lf\n",frametime,1000*(1.0/(double)frametime));
        render_drawframe(renderer, frame, properties, environment);
        properties.previous_frame = SDL_GetTicks();
        frame++;

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}