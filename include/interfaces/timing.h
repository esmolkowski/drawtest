#ifndef TIMING_H
#define TIMING_H

#include "app.h"

// Returns the current epoch microseconds
// for frame timing, so it is okay if it loops
unsigned long interface_get_epoch_microseconds();

// Gets current time in seconds and microseconds.
// Used to set starting time.
Time interface_get_epoch_time();
// Returns the current milliseconds since program started
// Will reset after 49 days.
unsigned long interface_get_milliseconds(Properties *properties);

#endif