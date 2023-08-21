#include <stddef.h>
#include "app.h"
#include "interfaces/timing.h"

// Platform dependant 
#include <sys/time.h> // timeval

Time interface_get_epoch_time()
{
    struct timeval t;
    gettimeofday(&t,NULL);
    Time time;
    time.seconds = (unsigned long)t.tv_sec;
    // always less than 1 million
    time.microseconds = (unsigned long)t.tv_usec;
    return time;
}

// Returns the current unix epoch microseconds
unsigned long interface_get_epoch_microseconds()
{
    return interface_get_epoch_time().microseconds;
}

// Returns the number of milliseconds since the program was started
unsigned long interface_get_milliseconds(Properties *properties)
{
    // I think there may be a bug here that causes a segfault
    Time current = interface_get_epoch_time();
    Time started = properties->start_time;
    unsigned long dsec = current.seconds-started.seconds;
    unsigned long dusec; // = current.microseconds-started.microseconds;

    if (current.microseconds > started.microseconds) {
        dusec = current.microseconds-started.microseconds;
    } else {
        // This case will only ever occur is dsec is >= 1
        dusec = current.microseconds+1000000-started.microseconds;
        dsec -= 1;
    }

    // Convert units to milliseconds.
    unsigned long dms = (dsec*1000)+(dusec/1000);

    return dms;
}