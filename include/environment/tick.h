#ifndef TICK_H
#define TICK_H

#include "app.h"
#include "vector.h"
#include "environment/environment.h"

void tick_run(Properties *properties, Environment *environment);
void tick_handle_inputs();

#endif