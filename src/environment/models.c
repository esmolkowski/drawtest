#include <stdlib.h>
#include "environment/models.h"
#include "vector.h"

void models_delete(Model *model) {
    free(model->edges);
    free(model->verticies);
    free(model);
}

Model *models_create_pyramid()
{
    Model *pyramid = malloc(sizeof(Model));
    // 5 verticies
    // [-10,10,0]
    // [10,10,0]
    // [-10,-10,0]
    // [10,-10,0]
    // [0,0,10]

    // 8 edges
    // 01 12 23 30
    // 04 14 24 34

    pyramid->vertex_count = 5;
    pyramid->edge_count = 16;

    pyramid->verticies = malloc(5*sizeof(Vector));
    pyramid->edges = malloc(16*sizeof(int));

    pyramid->verticies[0] = vector_create(-10.0, 10.0, 0.0); // top left
    pyramid->verticies[1] = vector_create( 10.0, 10.0, 0.0); // top right
    pyramid->verticies[2] = vector_create(-10.0,-10.0, 0.0); // bottom left
    pyramid->verticies[3] = vector_create( 10.0,-10.0, 0.0); // bottom right
    pyramid->verticies[4] = vector_create( 0.0,  0.0,  10.0); // middle

    int edges[16] = {
        0,1, // top
        1,3, // right
        3,2, // bottom
        2,0, // left
        0,4,
        1,4,
        2,4,
        3,4
    };
    for (int i = 0; i < 18; i++)
    {
        pyramid->edges[i] = edges[i];
    }

    return pyramid;
}

Model *models_create_hexagon()
{
    Model *hexagon = malloc(sizeof(Model));
    // 12 verticies
    // [-5.0, 8.66025403784, 10.0] top left of top hex
    // [5.0, 8.66025403784, 10.0] top right of top hex
    // [10.0, 0.0, 10.0] middle right of tophex
    // [5.0, -8.66025403784, 10.0] bottom right of tophex
    // [-5.0, -8.66025403784, 10.0] bottom left of tophex
    // [10.0, 0.0, 10.0] middle left of tophex
    // then repeat with -10 as z


    // 8 edges
    // 01 12 23 30
    // 04 14 24 34

    hexagon->vertex_count = 6;
    hexagon->edge_count = 12;

    hexagon->verticies = malloc(hexagon->vertex_count*sizeof(Vector));
    hexagon->edges = malloc(hexagon->edge_count*sizeof(int));

    hexagon->verticies[0] = vector_create(-5.0, 8.66025403784, 0.0); // top left of top hex
    hexagon->verticies[1] = vector_create( 5.0, 8.66025403784, 0.0); // top right of top hex
    hexagon->verticies[2] = vector_create(10.0, 0.0, 0.0); //middle right of tophex
    hexagon->verticies[3] = vector_create(5.0, -8.66025403784, 0.0); //bottom right of tophex
    hexagon->verticies[4] = vector_create(-5.0, -8.66025403784, 0.0); //bottom left of tophex
    hexagon->verticies[5] = vector_create(-10.0, 0.0, 0.0); //middle left of tophex
    

    int edges[12] = {
        0,1,
        1,2,
        2,3,
        3,4,
        4,5,
        5,0 // end of top hex
    };
    for (int i = 0; i < 12; i++)
    {
        hexagon->edges[i] = edges[i];
    }

    return hexagon;
}

Model *models_create_hexagonal_prism()
{
    Model *hexagonal_prism = malloc(sizeof(Model));
    // 12 verticies
    // [-5.0, 8.66025403784, 10.0] top left of top hex
    // [5.0, 8.66025403784, 10.0] top right of top hex
    // [10.0, 0.0, 10.0] middle right of tophex
    // [5.0, -8.66025403784, 10.0] bottom right of tophex
    // [-5.0, -8.66025403784, 10.0] bottom left of tophex
    // [10.0, 0.0, 10.0] middle left of tophex
    // then repeat with -10 as z


    // 8 edges
    // 01 12 23 30
    // 04 14 24 34

    hexagonal_prism->vertex_count = 12;
    hexagonal_prism->edge_count = 36;

    hexagonal_prism->verticies = malloc(hexagonal_prism->vertex_count*sizeof(Vector));
    hexagonal_prism->edges = malloc(hexagonal_prism->edge_count*sizeof(int));

    hexagonal_prism->verticies[0] = vector_create(-5.0, 8.66025403784, 10.0); // top left of top hex
    hexagonal_prism->verticies[1] = vector_create( 5.0, 8.66025403784, 10.0); // top right of top hex
    hexagonal_prism->verticies[2] = vector_create(10.0, 0.0, 10.0); //middle right of tophex
    hexagonal_prism->verticies[3] = vector_create(5.0, -8.66025403784, 10.0); //bottom right of tophex
    hexagonal_prism->verticies[4] = vector_create(-5.0, -8.66025403784, 10.0); //bottom left of tophex
    hexagonal_prism->verticies[5] = vector_create(-10.0, 0.0, 10.0); //middle left of tophex
    hexagonal_prism->verticies[6] = vector_create(-5.0, 8.66025403784, -10.0); // top left of bottomhex
    hexagonal_prism->verticies[7] = vector_create( 5.0, 8.66025403784, -10.0); // top right of bottomhex
    hexagonal_prism->verticies[8] = vector_create(10.0, 0.0, -10.0); //middle right of bottomhex
    hexagonal_prism->verticies[9] = vector_create(5.0, -8.66025403784, -10.0); //bottom right of bottomhex
    hexagonal_prism->verticies[10] = vector_create(-5.0, -8.66025403784, -10.0); //bottom left of bottomhex
    hexagonal_prism->verticies[11] = vector_create(-10.0, 0.0, -10.0); //middle left of bottomhex

    int edges[36] = {
        0,1,
        1,2,
        2,3,
        3,4,
        4,5,
        5,0, // end of top hex
        6,7,
        7,8,
        8,9,
        9,10,
        10,11,
        11,6, // end of bottom hex
        0,6,
        1,7,
        2,8,
        3,9,
        4,10,
        5,11 // end of connecting lines
    };
    for (int i = 0; i < 36; i++)
    {
        hexagonal_prism->edges[i] = edges[i];
    }

    return hexagonal_prism;
}

Model *models_create_sphere()
{
    Model *sphere = malloc(sizeof(Model));

    sphere->vertex_count = 1;
    sphere->edge_count = 2;

    sphere->verticies = malloc(sphere->vertex_count*sizeof(Vector));
    sphere->edges = malloc(sphere->edge_count*sizeof(int));

    sphere->verticies[0] = vector_create(0.0,0.0,0.0);

    int edges[2] = {
        0,0
    };
    for (int i = 0; i < 2; i++)
    {
        sphere->edges[i] = edges[i];
    }

    return sphere;
}

/*
#include <math.h>
Model *models_create_ground(int size)
{
    Model *ground = malloc(sizeof(Model));

    ground->vertex_count = size*size;
    // (width*height-1)*2
    ground->edge_count = (size*size-1)*2;

    ground->verticies = malloc(ground->vertex_count*sizeof(Vector));
    ground->edges = malloc(ground->edge_count*sizeof(int));

    // creates each vertex in vertical lines
    int v = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            double x = (double)i-((double)size/2);
            double y = (double)j-((double)size/2);
            double z = (sin(x/20.0) + sin(y/20.0));
            ground->verticies[v] = vector_create(x,y,0.0);
            v++;
        }
        
    }
    
    v = 0;
    int edges[(size*size-1)*2];
    for (int i = 0; i < size-1; i++)
    {
        for (int j = 0; j < size-1; j=j+2)
        {
            edges[v] = v;
            edges[v] = v+1;
            v++;
        }
        
    }
    
    for (int i = 0; i < (size*size-1)*2; i++)
    {
        ground->edges[i] = edges[i];
    }

    return ground;
}
*/

Model *models_create_cube(int size)
{
    // function takes cube sidelength,
    // but verticies will be size/2 away from the centre
    size = size/2;
    
    Model *cube = malloc(sizeof(Model));
    // 8 verticies
    // [-1,-1,-1] 0
    // [-1, 1,-1] 1
    // [ 1,-1,-1] 2
    // [ 1, 1,-1] 3
    // [-1,-1, 1] 4
    // [-1, 1, 1] 5
    // [ 1,-1, 1] 6
    // [ 1, 1, 1] 7

    // 12 edges
    // 01 02 32 31 bottom
    // 45 46 76 75 top
    // 04 15 26 37 top->bottom

    cube->vertex_count = 8;
    cube->edge_count = 24;

    cube->verticies = malloc(8*sizeof(Vector));
    cube->edges = malloc(24*sizeof(int));

    cube->verticies[0] = vector_create(-1*size,-1*size,-1*size);
    cube->verticies[1] = vector_create(-1*size, size,-1*size);
    cube->verticies[2] = vector_create( size,-1*size,-1*size);
    cube->verticies[3] = vector_create( size, size,-1*size);
    cube->verticies[4] = vector_create(-1*size,-1*size, size);
    cube->verticies[5] = vector_create(-1*size, size, size);
    cube->verticies[6] = vector_create( size,-1*size, size);
    cube->verticies[7] = vector_create( size, size, size);

    int edges[24] = {
        0,1, // top
        0,2,
        3,2,
        3,1,
        4,5,
        4,6,
        7,6,
        7,5,
        0,4,
        1,5,
        2,6,
        3,7
    };
    for (int i = 0; i < 24; i++)
    {
        cube->edges[i] = edges[i];
    }

    return cube;
}

Model *models_create_square(int size)
{
    // function takes square sidelength,
    // but verticies will be size/2 away from the centre
    size = size/2;
    
    Model *square = malloc(sizeof(Model));
    // 8 verticies
    // [-1,-1,-0] 0
    // [-1, 1,-0] 1
    // [ 1,-1,-0] 2
    // [ 1, 1,-0] 3
    // 8
    // 01 02 32 31 bottom

    square->vertex_count = 4;
    square->edge_count = 8;

    square->verticies = malloc(4*sizeof(Vector));
    square->edges = malloc(8*sizeof(int));

    square->verticies[0] = vector_create(-1*size,-1*size,0);
    square->verticies[1] = vector_create(-1*size, size,0);
    square->verticies[2] = vector_create( size,-1*size,0);
    square->verticies[3] = vector_create( size, size,0);

    int edges[8] = {
        0,1, // top
        0,2,
        3,2,
        3,1,
    };
    for (int i = 0; i < 8; i++)
    {
        square->edges[i] = edges[i];
    }

    return square;
}