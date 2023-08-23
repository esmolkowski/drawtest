#include <math.h>
#include "app.h"
#include "interfaces/renderer.h"
#include "datastructures/linked_list.h"
#include "environment/environment.h"
#include "environment/models.h"
#include "vector.h"
#include <stdio.h> 
#include <stdbool.h>

#include <stdlib.h>

#define M_PI 3.14159265358979323846

Vector2d translate_3d(Properties properties, Camera camera, Vector vertex)
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
    //vertex.y = z;
    //vertex.x = -y;
    Vector2d v;
    v.x = -y;
    v.y = z;

    //return vertex;
    return v;
}

void draw_line(Renderer *renderer, Properties properties, int x1, int y1, int x2, int y2) {
    int low, high;
    if (x1 == x2) {
        // vertical line
        low = y1;
        high = y2;
        if (y1 > y2) {
            low = y2;
            high = y1;
        }
        for (int y = low; y < high; y++)
        {
            renderer_draw_point(renderer, &properties, x1, y);
        }
        return;
    }
    // Else, calculate lines
    double m = (double)(y2-y1) / (double)(x2-x1);
    if (abs(m) < 1) {
        // draw in standard point slope
        //y = m(x-x1) + y1
        low = x1;
        high = x2;
        if (x1 > x2) {
            low = x2;
            high = x1;
        }
        for (int x = low; x < high; x++)
        {
            int y = ((int)( m * (x-x1))) + y1;
            renderer_draw_point(renderer, &properties, x, y);
        }
    } else {
        // iterate by ys
        low = y1;
        high = y2;
        if (y1 > y2) {
            low = y2;
            high = y1;
        }
        for (int y = low; y < high; y++)
        {
            int x = ((int)((y-y1)/m)) + x1;
            renderer_draw_point(renderer, &properties, x, y);
        }
    }
}

void draw_offset_point(Renderer *renderer, Properties properties, int x, int y)
{
    //term_renderer_draw_point(x+properties.RENDER_HALF_WIDTH, properties.RENDER_HALF_HEIGHT-y);
    renderer_draw_point(renderer, &properties,
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

void draw_offset_line(Renderer *renderer, Properties properties, int x1, int y1, int x2, int y2)
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

    draw_line(renderer, properties,
        x1+properties.RENDER_HALF_WIDTH, properties.RENDER_HALF_HEIGHT-y1,
        x2+properties.RENDER_HALF_WIDTH, properties.RENDER_HALF_HEIGHT-y2
    );
}

void draw_circle(Renderer *renderer, Properties properties, double cX, double cY, double radius)
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

void fill_triangle(Renderer *renderer, Properties properties, Vector2d v1, Vector2d v2, Vector2d v3) {
    /*
        Fills in a triangle between three points by drawing vertical lines between:
            The line between the left point and the right most point.
        and
            The line between the left point and the middle point.

        Upon reaching the middle, the 2nd boundary line becomes the line between
        the middle point and the right point.
    */

    // Show wireframe underneath polygon filling for cool glitch effect.
    draw_offset_line(renderer,properties, v1.x, v1.y, v2.x, v2.y);
    draw_offset_line(renderer,properties, v2.x, v2.y, v3.x, v3.y);
    draw_offset_line(renderer,properties, v3.x, v3.y, v1.x, v1.y);

    
    // sort vectors by most left and most high
    Vector2d left, middle, right, t;

    // The current y position and slope of the line between the left and right points
    double ylr, dlr;

    // The current y position and slope of the line between the left and middle point.
    // On the 2nd loop this changes to the middle point and the right point.
    double ym, dm;

    // Sort left to right
    if (v1.x > v2.x) { t = v1; v1 = v2; v2 = t;}
    if (v2.x > v3.x) { t = v2; v2 = v3; v3 = t;}
    if (v1.x > v2.x) { t = v1; v1 = v2; v2 = t;}

    left = v1; middle = v2; right = v3;

    //printf("left: %f,%f middle: %f,%f right: %f,%f\n", left.x, left.y, middle.x, middle.y, right.x, right.y);

    ylr = (double)left.y;
    dlr = (double)(right.y - left.y) / (double)(right.x - left.x);

    ym = (double)left.y;
    dm = (double)(middle.y - left.y) / (double)(middle.x - left.x);

    if (abs(middle.x-left.x) > 2) {
        for (int x = left.x; x < middle.x; x++)
        {
            draw_offset_line(renderer,properties, x, (int)ylr, x, (int)ym);
            ylr += dlr;
            ym += dm;
        }
        // Fill middle to right
        dm = (double)(right.y - middle.y) / (double)(right.x - middle.x);
        for (int x = middle.x; x < right.x; x++)
        {
            draw_offset_line(renderer,properties, x, (int)ylr, x, (int)ym);
            ylr += dlr;
            ym += dm;
        }
    } else {
        // approximate that left.x == middle.x
        // and fill starting at two points rather than diverging from a single
        ym = (double)middle.y;
        dm = (double)(right.y - middle.y) / (double)(right.x - left.x);
        for (int x = left.x; x < right.x; x++)
        {
            draw_offset_line(renderer,properties, x, (int)ylr, x, (int)ym);
            ylr += dlr;
            ym += dm;
        }
    }
}


void render_drawframe(Renderer *renderer, int frame, Properties properties, Environment *environment) {  
    renderer_set_color(renderer, 255,255,0,255);

    //draw_offset_line(renderer, properties, -10,-10,10,50);

    listNode *current_node = environment->entities;
    while (current_node != NULL)
    {
        Entity *entity = current_node->data;
        Vector entity_position = (entity->position);
        Vector entity_rotation = (entity->rotation);
        
        Model *model = entity->model;
        pModel *pmodel = entity->pmodel;
        char type = entity->type;
        Color color = entity->color;
        renderer_set_color(renderer, color.r, color.g, color.b, 255);
        
        // If point entity then
        if (type == '.')
        {
            Vector pos = entity_position;
            Vector2d vertex = translate_3d(properties, *(environment->camera), pos);
            draw_offset_point(renderer, properties, vertex.x, vertex.y);
            double d = vector_distance(pos, environment->camera->position);
            //double r = 100/((sqrt(d))+0.1);
            double r = 1.0/((d*d)/40000);
            //printf("%f %f\n",d,r);
            draw_circle(renderer, properties, vertex.x, vertex.y, r);
        } else if (type == 'p') {
            // set funny colors
            int r = color.r-50+(rand() % 50);
            int g = color.r-50+(rand() % 50);
            int b = color.r-50+(rand() % 50);
            renderer_set_color(renderer, color.r,g,0, 255);
            // render polygonal model
            // translate all of the verticies and then fill in the polygons
            Vector2d *translated_verticies = malloc(pmodel->vertex_count*sizeof(Vector2d));
            for (int i = 0; i< pmodel->vertex_count; i++)
            {
                Vector v = pmodel->verticies[i];
                vector_rotate(&v, entity_rotation);

                // TODO: convert this func to pointer input
                v.x += entity_position.x;
                v.y += entity_position.y;
                v.z += entity_position.z;

                translated_verticies[i] = translate_3d(properties, *(environment->camera), v);
            }
            for (int i = 0; i < pmodel->face_count; i++)
            {
                fill_triangle( renderer, properties,
                    translated_verticies[pmodel->faces[i].v1],
                    translated_verticies[pmodel->faces[i].v2],
                    translated_verticies[pmodel->faces[i].v3]);
            }
            free(translated_verticies);
        } else {
            // rotate and move each vertex in model
            Vector2d verticies[model->vertex_count];
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

                Vector2d v1 = verticies[model->edges[j]];
                Vector2d v2 = verticies[model->edges[j+1]];

                // draw with offset to be centered
                draw_offset_line(renderer, properties,
                    (int)v1.x,(int)v1.y,
                    (int)v2.x,(int)v2.y
                );
            }
        }
        current_node = current_node->next;
    }

    /*
    // test triangle filling
    Vector2d v1,v2,v3;
    v1.x = -50;
    v1.y = 0;
    v2.x = -60;
    v2.y = 50;
    v3.x = 30;
    v3.y = 0;
    //fill_triangle_bottomflat(renderer, properties, v1, v2, v3);
    //v2.y = -50;
    //fill_triangle_topflat(renderer, properties, v1, v2, v3);
    fill_triangle(renderer, properties, v1, v2, v3);
    */
}
