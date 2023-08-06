#include <stdlib.h>
#include "environment/environment.h"
#include "environment/models.h"
#include "vector.h"

Environment *environment_create_environment()
{
    Environment *environment = malloc(sizeof(Environment));
    environment->entity_count = 0;

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

Color color(int r, int g, int b) {
    Color c;
    c.r = r;
    c.g = g;
    c.b = b;
    return c;
}

Entity *entity_create(Model *model, pModel *pmodel, char type, Color color)
{
    Entity *entity = malloc(sizeof(Entity));
    entity->id = 1;
    entity->type = type; // m for model, p for pmodel, . for point
    entity->model = model;
    entity->pmodel = pmodel;
    entity->position = vector_new();
    entity->velocity = vector_new();
    entity->acceleration = vector_new();
    entity->rotation = vector_new();
    entity->angular_velocity = vector_new();
    entity->mass = 1;
    entity->color = color;
    return entity;
}

void entity_delete(Entity *entity) {
    // If entity has a model, delete that too
    if (entity->model != NULL) {
        models_delete(entity->model);
    }
    free(entity);
}

listNode *environment_add_entity(Environment *environment, Entity *entity) {
    listNode *node;
    if (environment->entities == NULL) {
        environment->entities = linked_list_create(entity);
        node = environment->entities;
    } else {
        node = linked_list_add_node(environment->entities, entity);
    }
    return node;
}

void environment_delete_entity(listNode *node) {
    // Deletes a given node along with its entity if it has one
    if (node != NULL) {
        if (node->data != NULL) {
            entity_delete(node->data);
        }
        linked_list_delete_node(node);
    }
}