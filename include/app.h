#ifndef APP_H
#define APP_H

#include <stdint.h>
#include "environment/models.h"

typedef struct Properties {
    int RENDER_WIDTH;
    int RENDER_HEIGHT;

    int RENDER_HALF_WIDTH;
    int RENDER_HALF_HEIGHT;

    float WINDOW_SCALE;

    uint32_t previous_frame;
    uint32_t previous_tick;
    int tickrate;
    float ticktime;
} Properties;

#endif