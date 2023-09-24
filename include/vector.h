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

double vector_distance(Vector v1, Vector v2);

double vector_magnitute(Vector vector);

Vector vector_cross(Vector v1, Vector v2);

double vector_dot(Vector v1, Vector v2);

Vector vector_add(Vector v1, Vector v2);

Vector vector_subtract(Vector v1, Vector v2);

typedef struct Vector2d {
    double x;
    double y;
} Vector2d;

#endif