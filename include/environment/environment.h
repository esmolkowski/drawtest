#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <stdbool.h>
#include "vector.h"
#include "environment/models.h"
#include "datastructures/linked_list.h"

typedef struct Color {
    int r;
    int g;
    int b;
} Color;

typedef struct Entity {
    int id;
    char type;

    Model *model;
    pModel *pmodel;

    Vector position;
    Vector velocity;
    Vector acceleration;

    Vector rotation;
    Vector angular_velocity;

    Color color;

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
    //Entity **entities;
    listNode *entities;
    // Could be in stack, but might be fun to add
    // multiple cameras in the future.
    Camera *camera;
} Environment;

Environment *environment_create_environment();
void environment_delete_entity(listNode *node);
Entity *entity_create(Model *model, pModel *pmodel, char type, Color color);
listNode *environment_add_entity(Environment *environment, Entity *entity);
void entity_delete(Entity *entity);
Color color(int r, int g, int b);

#endif