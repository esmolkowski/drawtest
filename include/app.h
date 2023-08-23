#ifndef APP_H
#define APP_H

#include <stdint.h>
#include "environment/models.h"
#include "interfaces/inputs.h"

typedef struct Time {
    unsigned long seconds;
    unsigned long microseconds;
} Time;

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

    Time start_time;

    InputActions *previous_inputs;
    InputActions *inputs;
} Properties;

#endif