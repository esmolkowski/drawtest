#include <math.h>
#include "SDL.h"
#include "app.h"
#include "datastructures/linked_list.h"
#include "environment/environment.h"
#include "environment/models.h"
#include "vector.h"
#include <stdio.h> 

#define M_PI 3.14159265358979323846

Vector translate_3d(Properties properties, Camera camera, Vector vertex)
{
    double x,y,z,a; // temporary varaibles to do math with

    Vector camera_position = (camera.position);
    double camera_phi = (camera.rotation.x);
    double camera_theta = (camera.rotation.y);
    double camera_fov = (camera.fov);
    
    // Offset from camera & viewplane
    vertex.x += (-1.0*camera_position.x);//+160;
    vertex.y += -1.0*camera_position.y;
    vertex.z += -1.0*camera_position.z;

    // shift so 90deg is the origin.. idk if this will cause issues down the road
    // because camera_phi has origin pointing right.. 
    
    a = -camera_phi;
    x = cos(a)*vertex.x - sin(a)*vertex.y;
    vertex.y = cos(a)*vertex.y + sin(a)*vertex.x;
    vertex.x = x;
    
    // 3d transform

    // rotate vertically
    x = cos(camera_theta)*vertex.x - sin(camera_theta)*vertex.z;
    vertex.z = sin(camera_theta)*vertex.x + cos(camera_theta)*vertex.z;
    vertex.x = x;

    // prevent divide by zero
    if (vertex.x <= 0.001)
    {
        vertex.x = .1;
    }

    y = (vertex.y)/vertex.x * (double)properties.RENDER_WIDTH;
    z = (vertex.z)/vertex.x * (double)properties.RENDER_HEIGHT;
    vertex.y = z;
    vertex.x = -y;

    return vertex;
}

void draw_offset_point(SDL_Renderer *renderer, Properties properties, int x, int y)
{
    SDL_RenderDrawPoint(renderer,
        x+properties.RENDER_HALF_WIDTH, properties.RENDER_HALF_HEIGHT-y
    );
}

void draw_offset_line(SDL_Renderer *renderer, Properties properties, int x1, int y1, int x2, int y2)
{
    /*
        TODO: interpolate where line would intercept border of render window in the case
        that one or more points are outside of the render window. Current approach is lazy.
    */

    if (x1 < -properties.RENDER_HALF_WIDTH-100 || x1 > properties.RENDER_HALF_WIDTH+100){
        return;
    }
    if (x2 < -properties.RENDER_HALF_WIDTH-100 || x2 > properties.RENDER_HALF_WIDTH+100){
        return;
    }
    if (y1 < -properties.RENDER_HEIGHT-100 || y1 > properties.RENDER_HEIGHT+100){
        return;
    }
    if (y2 < -properties.RENDER_HEIGHT-100 || y2 > properties.RENDER_HEIGHT+100){
        return;
    }
    SDL_RenderDrawLine(renderer,
        x1+properties.RENDER_HALF_WIDTH, properties.RENDER_HALF_HEIGHT-y1,
        x2+properties.RENDER_HALF_WIDTH, properties.RENDER_HALF_HEIGHT-y2
    );
}

void draw_circle(SDL_Renderer *renderer, Properties properties, double cX, double cY, double radius)
{
    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - (radius*2));
    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        draw_offset_point(renderer, properties, cX + x, cY - y);
        draw_offset_point(renderer, properties, cX + x, cY + y);
        draw_offset_point(renderer, properties, cX - x, cY - y);
        draw_offset_point(renderer, properties, cX - x, cY + y);
        draw_offset_point(renderer, properties, cX + y, cY - x);
        draw_offset_point(renderer, properties, cX + y, cY + x);
        draw_offset_point(renderer, properties, cX - y, cY - x);
        draw_offset_point(renderer, properties, cX - y, cY + x);
        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }
        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - (radius*2));
        }
    }
}

void render_drawframe(SDL_Renderer *renderer, int frame, Properties properties, Environment *environment) {  
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    
    //for (int i = 0; i < environment->entity_count; i++)
    listNode *current_node = environment->entities;
    while (current_node != NULL)
    {
        //Entity *entity = environment->entities[i];
        Entity *entity = current_node->data;
        Vector entity_position = (entity->position);
        Vector entity_rotation = (entity->rotation);
        
        Model *model = entity->model;
        
        // If point entity then
        if (model == NULL)
        {
            Vector vertex = entity_position;
            vertex = translate_3d(properties, *(environment->camera), vertex);
            draw_offset_point(renderer, properties, vertex.x, vertex.y);
            draw_circle(renderer, properties, vertex.x, vertex.y, 10);
        } else {
            // rotate and move each vertex in model
            Vector verticies[model->vertex_count];
            for (int i = 0; i < model->vertex_count; i++)
            {
                // rotate first
                Vector vertex = (model->verticies[i]);
                //printf("[%lf,%lf,%lf]->", vertex.x,vertex.y,vertex.y);
                vector_rotate(&vertex, entity_rotation);
                //printf("[%lf,%lf,%lf]->", vertex.x,vertex.y,vertex.y);

                // then move
                vertex.x = vertex.x + entity_position.x;
                vertex.y = vertex.y + entity_position.y;
                vertex.z = vertex.z + entity_position.z;

                // TOP DOWN FIXED VIEW DONE
                // BEGIN TOP DOWN PLAYER VIEW

                verticies[i] = translate_3d(properties, *(environment->camera), vertex);
            }
            
            for (int j = 0; j < model->edge_count; j=j+2)
            {

                Vector v1 = verticies[model->edges[j]];
                Vector v2 = verticies[model->edges[j+1]];

                // draw with offset to be centered
                draw_offset_line(renderer, properties,
                    (int)v1.x,(int)v1.y,
                    (int)v2.x,(int)v2.y
                );
            }
        }
        current_node = current_node->next;
    }
}