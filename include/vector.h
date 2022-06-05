#ifndef VECTOR_H
#define VECTOR_H

// x: left-right
// z: up-down
// y: forward-backward (2d y)
typedef struct Vector {
    double x;
    double y;
    double z;
} Vector;

Vector vector_create(double x, double y, double z);

//Vector vector_create_onstack(double x, double y, double z);

Vector vector_new();

void vector_rotate(Vector *point, Vector rotation);

void vector_move_angled(Vector *pos, double angle, double distance);

double vector_distance(Vector *v1, Vector *v2);

#endif