#include "SDL.h"
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include "render/render.h"
#include "app.h"
#include "environment/environment.h"
#include "environment/tick.h"
#include "environment/models.h"
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

    // Set up environment
    Environment *environment = environment_create_environment(20);
    environment->entities[0] = environment_create_entity(
        models_create_pyramid()
    );
    environment->entities[0]->velocity.z = .2;
    for (int i = 1; i < 3; i++)
    {
        environment->entities[i] = environment_create_entity(
        models_create_hexagonal_prism()
        );
        environment->entities[i]->position.x = 50;
        environment->entities[i]->position.y = 50;
        environment->entities[i]->position.z = ((i-1)*20)+10;
    }
    for (int i = 3; i < 5; i++)
    {
        environment->entities[i] = environment_create_entity(
        models_create_hexagonal_prism()
        );
        environment->entities[i]->position.x = -30;
        environment->entities[i]->position.y = -20;
        environment->entities[i]->position.z = ((i-3)*20)+10;
    }
    for (int i = 5; i < 6; i++)
    {
        environment->entities[i] = environment_create_entity(
        models_create_hexagonal_prism()
        );
        environment->entities[i]->position.x = 25;
        environment->entities[i]->position.y = -30;
        environment->entities[i]->position.z = ((i-5)*20)+10;
    }
    for (int i = 6; i < 8; i++)
    {
        environment->entities[i] = environment_create_entity(
        models_create_hexagonal_prism()
        );
        environment->entities[i]->position.x = -70;
        environment->entities[i]->position.y = 50;
        environment->entities[i]->position.z = ((i-6)*20)+10;
    }
    for (int i = 8; i < 10; i++)
    {
        environment->entities[i] = environment_create_entity(
        models_create_hexagonal_prism()
        );
        environment->entities[i]->position.x = 0;
        environment->entities[i]->position.y = 70;
        environment->entities[i]->position.z = ((i-8)*20)+10;
    }
    for (int i = 10; i < 13; i++)
    {
        environment->entities[i] = environment_create_entity(
        models_create_hexagonal_prism()
        );
        environment->entities[i]->position.x = -30;
        environment->entities[i]->position.y = 30;
        environment->entities[i]->position.z = ((i-10)*20)+10;
    }
    environment->entities[13] = environment_create_entity(
        models_create_hexagon()
    );
    environment->entities[13]->position.x = 100;
    environment->entities[13]->position.y = 100;
    environment->entities[14] = environment_create_entity(
        models_create_hexagon()
    );
    environment->entities[14]->position.x = 100+15.0;
    environment->entities[14]->position.y = 100+8.66025403785;
    environment->entities[15] = environment_create_entity(
        models_create_hexagon()
    );
    environment->entities[15]->position.x = 100-15.0;
    environment->entities[15]->position.y = 100+8.66025403785;
    environment->entities[16] = environment_create_entity(
        models_create_hexagon()
    );
    environment->entities[16]->position.x = 100-15.0;
    environment->entities[16]->position.y = 100-8.66025403785;
    environment->entities[17] = environment_create_entity(
        models_create_hexagon()
    );
    environment->entities[17]->position.x = 100+15.0;
    environment->entities[17]->position.y = 100-8.66025403785;
    environment->entities[18] = environment_create_entity(
        models_create_hexagon()
    );
    environment->entities[18]->position.x = 100;
    environment->entities[18]->position.y = 100+17.5;
    environment->entities[19] = environment_create_entity(
        models_create_hexagon()
    );
    environment->entities[19]->position.x = 100;
    environment->entities[19]->position.y = 100-17.5;
    //    environment->entities[20] = environment_create_entity(
    //        models_create_ground(3)
    //    );
    //    environment->entities[20]->position.x = 0.0;
    //    environment->entities[20]->position.y = 0.0;

    


    int frame = 0;
    while (true)
    {
        SDL_Event event;
        Camera *camera = environment->camera;
        //Uint8 *keystates = SDL_GetKeyState( NULL );
        if (SDL_PollEvent(&event))
        {
            
            switch( event.type ) {
                case SDL_KEYDOWN:
                    switch( event.key.keysym.sym ) {
                        case SDLK_e:
                            environment->entities[0]->position.x = camera->position.x;
                            environment->entities[0]->position.y = camera->position.y;
                            environment->entities[0]->position.z = camera->position.z;
                            double x,y,z;
                            //printf("%lf,%lf,%lf,%lf\n\n\n\n",camera->theta*M_PI/2,x,y,z);
                            x = 100 * sin(M_PI/2 - camera->rotation.x) * cos(camera->rotation.y); // phi, theta
                            y = 100 * cos(M_PI/2 - camera->rotation.x) * cos(camera->rotation.y);
                            z = -100 * sin(camera->rotation.y); // theta
                            environment->entities[0]->velocity.x = x;
                            environment->entities[0]->velocity.y = y;
                            environment->entities[0]->velocity.z = z;
                            break;
                    }
                    break;
                default:
                    break;
            }
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

        /*
        Vector *vector = vector_create(5.0,5.0,5.0);
        printf("---%lf\n",vector.x);
        Vector v1 = *vector;
        printf("%lf\n",v1.x);
        v1.x = 6.0;
        printf("%lf\n----",vector.x);
        */

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}