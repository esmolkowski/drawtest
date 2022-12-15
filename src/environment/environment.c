#include <stdlib.h>
#include "environment/environment.h"
#include "vector.h"

Environment* environment_create_environment(int entity_count)
{
    Environment *environment = malloc(sizeof(Environment));
    environment->entity_count = entity_count;
    environment->entities = malloc(entity_count*sizeof(Entity));

    environment->camera = malloc(sizeof(Camera));
    environment->camera->position = vector_new();
    environment->camera->position.x = -30.0;
    // y:phi, z:theta, x:undecided.
    //environment->camera->rotation = vector_new();
    environment->camera->rotation = vector_new();
    //environment->camera->phi = 0;
    //environment->camera->theta = 0.0;
    environment->camera->fov = 0.0;
    return environment;
}

Entity* environment_create_entity(Model *model, char type)
{
    Entity *entity = malloc(sizeof(Entity));
    entity->id = 1;
    entity->type = type;
    entity->model = model;
    entity->position = vector_new();
    entity->velocity = vector_new();
    entity->acceleration = vector_new();
    entity->rotation = vector_new();
    entity->angular_velocity = vector_new();
    entity->mass = 1;
    return entity;
}