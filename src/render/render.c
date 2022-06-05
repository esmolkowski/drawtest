#include <math.h>
#include "SDL.h"
#include "app.h"
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
    
    //printf("[%lf,%lf]--------------%lf\n",vertex.x,vertex.y,camera_phi);
    a = -camera_phi;
    x = cos(a)*vertex.x - sin(a)*vertex.y;
    vertex.y = cos(a)*vertex.y + sin(a)*vertex.x;
    vertex.x = x;
    //printf("[%lf,%lf]--------------DONE\n",vertex.x,vertex.y);
    
    // 3d transform

    // rotate vertically
    // vertex.x = vertex.x

    x = cos(camera_theta)*vertex.x - sin(camera_theta)*vertex.z;
    vertex.z = sin(camera_theta)*vertex.x + cos(camera_theta)*vertex.z;
    vertex.x = x;

    // roll
    //y = cos(camera_theta)*vertex.y - sin(camera_theta)*vertex.z;
    //vertex.z = sin(camera_theta)*vertex.y + cos(camera_theta)*vertex.z;
    //vertex.y = y;

    /*
    if (vertex.y <= 0.001)
    {
        //printf("y less than 0 %lf\n", vertex.y);
        vertex.y = .1;
    }
    */
    if (vertex.x <= 0.001)
    {
        //printf("y less than 0 %lf\n", vertex.y);
        vertex.x = .1;
    }

    //printf("pre divide [%lf,%lf]\n",vertex.x,vertex.y);

    /*
    x = (vertex.x/vertex.y) * (double)properties.RENDER_WIDTH;
    vertex.y = (vertex.z/vertex.y) * (double)properties.RENDER_HEIGHT;
    vertex.x = x;
    */
    y = (vertex.y)/vertex.x * (double)properties.RENDER_WIDTH;//(cos(frame/M_PI/45)+1);
    z = (vertex.z)/vertex.x * (double)properties.RENDER_HEIGHT;//(sin(frame/M_PI/60)+1);
    vertex.y = z;
    vertex.x = -y;


    //printf("post divide #%d [%lf,%lf]\n",i,vertex.x,vertex.y);


    // finally add to array
    //verticies[i] = vertex;
    return vertex;
    //printf("[%lf,%lf,%lf]\n", vertex.x,vertex.y,vertex.y);
}

void draw_offset_point(SDL_Renderer *renderer, Properties properties, int x, int y)
{
    SDL_RenderDrawPoint(renderer,
        x+properties.RENDER_HALF_WIDTH, properties.RENDER_HALF_HEIGHT-y
    );
}

void draw_offset_line(SDL_Renderer *renderer, Properties properties, int x1, int y1, int x2, int y2)
{
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
    // SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    // debug draw a line for camera's view
    //Vector lookpoint = vector_create_onstack(camera_position.x,camera_position.y,camera_position.z);
    //vector_move_angled(&lookpoint, camera_phi, 10);
    //draw_offset_line(renderer,properties,camera_position.x,camera_position.y,lookpoint.x,lookpoint.y);
    // debug draw centerd view
    //draw_offset_line(renderer,properties,0,0,0.0,10.0);
    // fun grid
    
    int fish = 200;
    for (int i = 0; i < fish; i++)
    {
        for (int j = 0; j < fish; j++)
        {
            double x = ( (i-(fish/2))*1) + 150;
            double y = ( (j-(fish/2))*1) - 150;
            double z = (sin(x/20)+sin(y/20))*10*( sin( (double)frame/100 ) );
            Vector v = vector_create( x, y, z);
            //printf("[%lf,%lf]\n",v.x,v.y);
            v = translate_3d(properties, *(environment->camera), v);
            //printf("[%lf,%lf]\n",v.x,v.y);
            int r = (int)((sin(environment->camera->position.x*z/5)+1)*128);
            int g = (int)((sin(environment->camera->position.y*z/5)+1)*128);
            int b = (int)((sin(environment->camera->position.z*z/5)+1)*64);
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);  
            draw_offset_point(renderer, properties, v.x, v.y);
            //draw_offset_line(renderer,properties, )
        }
        
    }
    

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    
    for (int i = 0; i < environment->entity_count; i++)
    {
        Entity *entity = environment->entities[i];
        Model *model = entity->model;
        Vector entity_position = (entity->position);
        Vector entity_rotation = (entity->rotation);
        
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

            //printf("[%lf,%lf]->[%lf,%lf]\n", v1.x,v1.y,v2.x,v2.y);
            // draw with offset to be centered
            draw_offset_line(renderer, properties,
                (int)v1.x,(int)v1.y,
                (int)v2.x,(int)v2.y
            );
        }

        //entity->rotation->z = entity->rotation->z+.0001;
        //entity->rotation->y = entity->rotation->y-.0001;
        //entity->rotation->z = entity->rotation->z+.0001;
        //double d = vector_distance(&camera_position, &verticies[0]);
        //printf("%lf\n",1000/d);
        //draw_circle(renderer, properties, verticies[0].x, verticies[0].y, (20/d)*(double)properties.RENDER_HEIGHT);
        
    }
}