#include "SDL.h"
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include "app.h"
#include "interfaces/timing.h"
#include "datastructures/linked_list.h"
#include "environment/environment.h"
#include "environment/tick.h"
#include "environment/models.h"
#include "render/render.h"
#include "vector.h"
#include <unistd.h>

#include <time.h> // nanosleep
#include <sys/time.h> // timeval

// TODO: Change everything to fixed size types

// WINDOWS
//#include <windows.h>

//int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevIns, LPSTR lpszArgument, int iShow)
int main(int argc, char *argv[])
{
    // Initialize app properties before loop
    Properties properties;
    properties.RENDER_WIDTH = 50;//320;
    properties.RENDER_HEIGHT = 50;//240;
    properties.RENDER_HALF_WIDTH = properties.RENDER_WIDTH/2;
    properties.RENDER_HALF_HEIGHT = properties.RENDER_HEIGHT/2;
    properties.WINDOW_SCALE = 10.0;

    // used for FPS tracking
    properties.previous_frame = 0;
    properties.previous_tick = 0;

    // Set target fps and frametime (microseconds)
    properties.target_fps = 60;
    properties.target_frametime = 1000000/properties.target_fps;

    properties.camera_rotate_speed = 1;
    
    properties.tickrate = 64;
    properties.ticktime = 1.0/((float)properties.tickrate);

    properties.start_time = interface_get_epoch_time();


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
    
    Entity *pyramid = environment_add_entity(environment, entity_create(
        models_create_pyramid(10), NULL, 'w', color(253,166,58)
    ))->data;
    pyramid->position = vector_create(-20,-10,-10);
    pyramid->velocity.z = 1;
    pyramid->angular_velocity.y = .1;
    Entity *hex = environment_add_entity(environment, entity_create(
        models_create_hexagonal_prism(), NULL, 'w', color(255,90,0)
    ))->data;
    hex->angular_velocity = vector_create(1,.1,-.3);
    hex->position = vector_create(30,50,-10);
    listNode *ent2 = environment_add_entity(environment, entity_create(
        models_create_cube(200), NULL, 'w', color(255,255,0)
    ));
    Entity *ent = environment_add_entity(environment, entity_create(
        NULL, pmodels_create_tetrahedron(), 'p', color(255,255,0)
    ))->data;
    ent->position = vector_create(10,10,10);
    ent->angular_velocity = vector_create(2,2,0);

    Entity *ent1 = environment_add_entity(environment, entity_create(
        NULL, pmodels_create_tetrahedron(), 'p', color(0,255,0)
    ))->data;
    ent1->position = vector_create(-50,10,-30);
    ent1->angular_velocity = vector_create(2,.5,1);
    // end of demo environment setup

    int frame = 0;
    while (true)
    {
        // Get inputs
        InputActions inputs = interface_get_inputs();
        properties.inputs = &inputs;

        if (properties.inputs->quit)
        {
            break;
        }
        tick_handle_inputs(environment, &properties);

        // draw frame
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        tick_run(&properties, environment);

        unsigned long frametime = interface_get_epoch_microseconds()-properties.previous_frame;

        if (properties.target_frametime > frametime) {
            // get time delta in microseconds
            unsigned long d = properties.target_frametime - frametime;
            // convert microseconds to nanoseconds
            struct timespec remaining, request = { 0, d*1000 };
            nanosleep(&request, &remaining);
        }

        // draw frame
        //printf("\x1b[2J");
        render_drawframe(renderer, frame, properties, environment);
        //fflush(stdout);

        properties.previous_frame = interface_get_epoch_microseconds();
        frame++;

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}