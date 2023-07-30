#include <math.h>
#include "SDL.h"
#include "app.h"
#include "datastructures/linked_list.h"
#include "environment/environment.h"
#include "environment/models.h"
#include "vector.h"
#include <stdio.h> 
#include <stdbool.h> 

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

int calculate_intercept(int bound, int i1, int j1, int i2, int j2) {
    /*
        Calculates the intercept of an axis for the line between two points.
        To find y intercepts, i=x, j=y
        To find x intercepts, i=y, j=x
    */
    double di1 = (double)i1;
    double dj1 = (double)j1;
    double di2 = (double)i2;
    double dj2 = (double)j2;

    double m = ((dj2-dj1)/(di2-di1));

    return (int)(
        /*
            Essentially just point slope equation of line between two points solved for y with x as
            the boundary. Equation is the same when solved for x, so swapping x and y inputs changes
            the axis.
        */
        (m*(bound-di1))+dj1
    );
}

void draw_offset_line(SDL_Renderer *renderer, Properties properties, int x1, int y1, int x2, int y2)
{
    /*
        Draws a line between two points
    */

    // Calculate which points are out of bounds
    bool x1ob = (abs(x1) > properties.RENDER_HALF_WIDTH);
    bool y1ob = (abs(y1) > properties.RENDER_HALF_HEIGHT);
    bool x2ob = (abs(x2) > properties.RENDER_HALF_WIDTH);
    bool y2ob = (abs(y2) > properties.RENDER_HALF_HEIGHT);

    int bound;
    int nx1 = x1;
    int ny1 = y1;
    int nx2 = x2;
    int ny2 = x2;

    // Don't render if all points are out of bounds
    if (x1ob && y1ob && x2ob && y2ob) {
        return;
    }
    else if (x1ob && x2ob && (x1*x2 > 0))
    {
        // If both x1 and x2 are on one side of the viewframe they should not have any visible parts
        return;
    }
    else if (y1ob && y2ob && (y1*y2 > 0))
    {
        // same with y
        return;
    }
    else if (x1ob || y1ob || x2ob || y2ob)
    {
        // If one of the points is out of bounds
        // we need to calculate where along the viewport a line between those points intercepts.

        // comments on next block because more inuitive to picture y=mx+b
        if (x1ob && y1 != y2) {
            bound = properties.RENDER_HALF_WIDTH;
            if (x1 < properties.RENDER_HALF_WIDTH) {
                bound = -bound;
            }
            ny1 = calculate_intercept(bound, x1, y1, x2, y2);
            if (x1 == nx1) {
                nx1 = bound;
            }
        }

        /*
            If y is out of bounds and the x's are equal, that's
            just a vertical line so we don't need to find the intercept on the y axis.
        */
        if (y1ob && x1 != x2) {
            // Set the bound either as either the bottom or top of the viewport
            bound = properties.RENDER_HALF_HEIGHT;
            if (y1 < properties.RENDER_HALF_HEIGHT) {
                bound = -bound;
            }
            /*
                This is essentially solving for the point between x1,y1 and x2,y2 at the value
                y = viewport_boundary.
                
                m = slope = rise/run = (y2-y1)/(x2-x1)
                y = m(x - x1) + y1

                But we know y. y is the upper/lower boundary of the viewport.
                If we swap the x and y's on this function we are essentially solving for x at point
                y which is what we want.

                As an example, imagine we have two points (0,0) (10,20)
                and our viewport only goes to x = 5. If we draw a line between these two
                points its equation is y = 2x. When x = 5, y = 10, so we should draw the line
                between (0,0) and (5,10) which stay within the bounds of our viewport.
                

                We set the intercept point as a new variable, because if x and y are both
                out of bounds we need to use the original values to
                calculate both of the intercepts.
            */ 
            nx1 = calculate_intercept(bound, y1, x1, y2, x2);
            // only overwrite the new variable if it has not already been overwritten.
            if (y1 == ny1) {
                ny1 = bound;
            }
        }

        if (x2ob && y1 != y2) {
            bound = properties.RENDER_HALF_WIDTH;
            if (x2 < properties.RENDER_HALF_WIDTH) {
                bound = -bound;
            }
            ny2 = calculate_intercept(bound, x1, y1, x2, y2);
            if (x2 == nx2) {
                nx2 = bound;
            }
        }

        if (y2ob && x1 != x2) {
            bound = properties.RENDER_HALF_HEIGHT;
            if (y2 < properties.RENDER_HALF_HEIGHT) {
                bound = -bound;
            }
            nx2 = calculate_intercept(bound, y1, x1, y2, x2);
            if (y2 == ny2) {
                ny2 = bound;
            }
        }
    }

    /*
        Off set axis to match SDL coordinates.
        
        Origin : x = 0, y = 0
        
        =>
        
        SDL Origin : x = width/2, y = height/2
    */
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
    
    listNode *current_node = environment->entities;
    while (current_node != NULL)
    {
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
                vector_rotate(&vertex, entity_rotation);

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