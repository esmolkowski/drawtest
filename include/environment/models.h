#ifndef MODELS_H
#define MODELS_H

#include "vector.h"

typedef struct Model {
    int vertex_count;
    int edge_count;
    Vector *verticies;
    int *edges;
} Model;

// destroy a model
//void models_free(Model *model);

Model *models_create_pyramid();
Model *models_create_hexagon();
Model *models_create_hexagonal_prism();
Model *models_create_sphere();
//Model *models_create_ground(int size);

#endif