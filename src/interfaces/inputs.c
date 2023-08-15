#include "interfaces/inputs.h"
#include "SDL.h"

InputActions interface_get_inputs()
{
    // Initialize inputs struct to zeroes/false
    InputActions inputs = {0};

    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        if ( event.type == SDL_QUIT ) {
            inputs.quit = true;
        }
    }
    SDL_PumpEvents();
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_W]) {inputs.forward = true;}
    if (keystate[SDL_SCANCODE_S]) {inputs.backward = true;}
    if (keystate[SDL_SCANCODE_A]) {inputs.strafe_left = true;}
    if (keystate[SDL_SCANCODE_D]) {inputs.strafe_right = true;}
    if (keystate[SDL_SCANCODE_SPACE]) {inputs.rise = true;}
    if (keystate[SDL_SCANCODE_LSHIFT]) {inputs.fall = true;}
    if (keystate[SDL_SCANCODE_RIGHT]) {inputs.right = true;}
    if (keystate[SDL_SCANCODE_LEFT]) {inputs.left = true;}
    if (keystate[SDL_SCANCODE_UP]) {inputs.up = true;}
    if (keystate[SDL_SCANCODE_DOWN]) {inputs.down = true;}
    return inputs;
}