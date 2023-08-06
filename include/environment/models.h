#ifndef MODELS_H
#define MODELS_H

#include "vector.h"

// Wireframe Model
typedef struct Model {
    int vertex_count;
    int edge_count;
    Vector *verticies;
    int *edges;
} Model;

// destroy a model
void models_delete(Model *model);

Model *models_create_pyramid();
Model *models_create_hexagon();
Model *models_create_hexagonal_prism();
Model *models_create_sphere();
Model *models_create_cube(int size);
Model *models_create_square(int size);
//Model *models_create_ground(int size);

// Tri
// a 2d triangle in 3d space
typedef struct Tri {
    Vector v1;
    Vector v2;
    Vector v3;
} Tri;

// Polygonal Model
typedef struct pModel {
    int tri_count;
    Tri *tris;
} pModel;

pModel *pmodels_create_tetrahedron();

#endif