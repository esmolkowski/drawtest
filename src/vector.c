#include <stdlib.h>
#include <math.h>
#include "vector.h"

Vector vector_create(double x, double y, double z)
{
    Vector vector;//; = malloc(sizeof(Vector));
    vector.x = x;
    vector.y = y;
    vector.z = z;

    return vector;
}

Vector vector_new()
{
    Vector vector;// = malloc(sizeof(Vector));
    vector.x = 0.0;
    vector.y = 0.0;
    vector.z = 0.0;

    return vector;
}

// Creates vector on the stack only
/*
Vector vector_create_onstack(double x, double y, double z)
{
    Vector vector;
    vector.x = x;
    vector.y = y;
    vector.z = z;

    return vector;
}
*/

void vector_rotate(Vector *point, Vector rotation)
{
    // https://stackoverflow.com/questions/34050929/3d-point-rotation-algorithm/34060479
    // is possibly more efficient than this.

    Vector sines;// = vector_create(
    sines.x = sin(rotation.x);
    sines.y = sin(rotation.y);
    sines.z = sin(rotation.z);

    Vector cosines;// = vector_create(
    cosines.x = cos(rotation.x);
    cosines.y = cos(rotation.y);
    cosines.z = cos(rotation.z);

    double t1,t2; // temporary variables

    t1 = (cosines.x * point->y) + (sines.x * point->z);
    t2 = (-1.0 * sines.x * point->y) + (cosines.x * point->z);
    point->y = t1;
    point->z = t2;

    t1 = (cosines.y * point->x) + (sines.y * point->z);
    t2 = (-1.0 * sines.y * point->x) + (cosines.y * point->z);
    point->x = t1;
    point->z = t2;

    t1 = (cosines.z * point->x) + (sines.z * point->y);
    t2 = (-1.0 * sines.z * point->x) + (cosines.z * point->y);
    point->x = t1;
    point->y = t2;

    /*
    let temp1, temp2;

    temp1 = cos.x * point.y + sin.x * point.z;
    temp2 = -sin.x * point.y + cos.x * point.z;
    point.y = temp1;
    point.z = temp2;

    temp1 = cos.y * point.x + sin.y * point.z;
    temp2 = -sin.y * point.x + cos.y * point.z;
    point.x = temp1;
    point.z = temp2;

    temp1 = cos.z * point.x + sin.z * point.y;
    temp2 = -sin.z * point.x + cos.z * point.y;
    point.x = temp1;
    point.y = temp2;
    */
}

void vector_move_angled(Vector *pos, double angle, double distance)
{
    double x = (distance*cos(angle))+pos->x;
    double y = (distance*sin(angle))+pos->y;
    pos->x = x;
    pos->y = y;
}

double vector_distance(Vector v1, Vector v2)
{
    return sqrt( pow(v1.x-v2.x,2)+pow(v1.y-v2.y,2)+pow(v1.z-v2.z,2) );
}

double vector_magnitute(Vector vector)
{
    return sqrt( pow(vector.x,2) + pow(vector.y,2) + pow(vector.z,2) );
}

Vector vector_cross(Vector v1, Vector v2)
// cross product
{
    double i,j,k;
    i = (v1.y*v2.z)-(v1.z-v2.y);
    j = -((v1.x*v2.z)-(v1.z*v2.x));
    k = (v1.x*v2.y)-(v1.y*v2.x);
    return vector_create(i,j,k);
}

Vector vector_add(Vector v1, Vector v2)
{
    return vector_create(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
}

Vector vector_subtract(Vector v1, Vector v2)
{
    return vector_create(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
}

double vector_dot(Vector v1, Vector v2)
{
    return (v1.x*v2.x) + (v1.y*v2.y) + (v1.z*v2.z);
}