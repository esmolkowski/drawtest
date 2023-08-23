#ifndef RENDER_H
#define RENDER_H

#include "SDL.h"
#include "app.h"
#include "interfaces/renderer.h"
#include "environment/environment.h"

//void render_drawframe(SDL_Renderer *renderer, int frame);
void render_drawframe(Renderer *renderer, int frame, Properties properties, Environment *environment);

#endif