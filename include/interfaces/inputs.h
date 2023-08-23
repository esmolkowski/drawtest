#ifndef INPUTS_H
#define INPUTS_H

#include <stdbool.h>

// Could use a uint with bitwise operations, but this is easier to read
// and work with for now.
typedef struct InputActions {
    bool forward;
    bool backward;
    bool strafe_left;
    bool strafe_right;
    bool rise;
    bool fall;
    bool right;
    bool left;
    bool up;
    bool down;
    bool quit;
    bool use;
} InputActions;

InputActions interface_get_inputs();

#endif