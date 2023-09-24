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
typedef struct Face {
    // The vertex indexes of each face
    unsigned int v1;
    unsigned int v2;
    unsigned int v3;
} Face;

// Polygonal Model
typedef struct pModel {
    int vertex_count;
    int face_count;
    Vector *verticies;
    Face *faces;
} pModel;

pModel *pmodels_create_tetrahedron();
pModel *pmodels_create_triangle();

#endif