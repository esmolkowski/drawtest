#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <stdbool.h>
#include "vector.h"
#include "environment/models.h"

typedef struct Entity {
    int id;
    char type;

    Model *model;

    Vector position;
    Vector velocity;
    Vector acceleration;

    Vector rotation;
    Vector angular_velocity;

    bool fixed;
    float mass;
} Entity;

typedef struct Camera {
    Vector position;
    Vector velocity;
    Vector rotation; // [phy, theta, roll]
    Vector angular_velocity;
    //double phi;
    //double theta;
    double fov;
    double forward_speed;
    double strafe_speed; // right is positive
    double up_speed;
    //double phi_speed;
    //double theta_speed;
} Camera;

typedef struct Environment {
    int entity_count;
    Entity **entities;
    // Could be in stack, but might be fun to add
    // multiple cameras in the future.
    Camera *camera;
} Environment;

Environment* environment_create_environment(int entity_count);
Entity* environment_create_entity(Model *model, char type);

#endif