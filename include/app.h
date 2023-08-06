#ifndef APP_H
#define APP_H

#include <stdint.h>
#include "environment/models.h"

typedef struct Properties {
    int RENDER_WIDTH;
    int RENDER_HEIGHT;

    int RENDER_HALF_WIDTH;
    int RENDER_HALF_HEIGHT;

    int target_fps;
    int target_frametime;

    float WINDOW_SCALE;
    
    float camera_rotate_speed;

    unsigned long previous_frame;
    // TODO: update ticks to be non sdl
    uint32_t previous_tick;
    int tickrate;
    float ticktime;
} Properties;

#endif