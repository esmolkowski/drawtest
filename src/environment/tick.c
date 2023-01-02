#include <stdint.h>
#include "environment/tick.h"
#include "app.h"
#include "vector.h"
#include "datastructures/linked_list.h"
#include "environment/environment.h"
#include "SDL.h"
#include <math.h>
#include <sys/time.h>
#include <stdbool.h>

long long current_timestamp() {
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}

void tick_handle_inputs(Environment *environment, Properties properties, SDL_Event event) {
    //SDL_Event event =;
    switch( event.type ) {
        case SDL_KEYDOWN:
            switch( event.key.keysym.sym ) {
                case SDLK_e:
                    // printf("aaaaaaaaaa\n");
                    // // test deleting 2nd entity in environment
                    // listNode *node = environment->entities->next;
                    // environment_delete_entity(node);
                    // for (size_t i = 0; i < 1000000; i++)
                    // {
                    //     listNode *ent = environment_add_entity(environment, entity_create(
                    //         models_create_hexagonal_prism(10), 'm'
                    //     ));
                    //     environment_delete_entity(ent);
                    // }
                    
                    listNode *projectile_node = environment_add_entity(environment, entity_create(
                        NULL, 'p'
                    ));
                    Entity *projectile = projectile_node->data;
                    Camera *camera = environment->camera;
                    // code below launches entity in direction of viewpoint
                    projectile->position.x = camera->position.x;
                    projectile->position.y = camera->position.y;
                    projectile->position.z = camera->position.z;
                    double x,y,z;
                    x = 100 * sin(M_PI/2 - camera->rotation.x) * cos(camera->rotation.y); // phi, theta
                    y = 100 * cos(M_PI/2 - camera->rotation.x) * cos(camera->rotation.y);
                    z = -100 * sin(camera->rotation.y); // theta
                    projectile->velocity.x = x;
                    projectile->velocity.y = y;
                    projectile->velocity.z = z;
                    break;
            }
            break;
        default:
            break;
    }
}

void set_camera_velocities(Camera *camera, Properties properties, const Uint8 *keystate, int mx, int my)
{
    // forward-back
    if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_S] ) {
        if (keystate[SDL_SCANCODE_W])
        {
            camera->forward_speed = 50;
        }
        if (keystate[SDL_SCANCODE_S])
        {
            camera->forward_speed = -50;
        }
    } else {
        camera->forward_speed = 0.0;
    }
    if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_D] ) {
        if (keystate[SDL_SCANCODE_A])
        {
            camera->strafe_speed = 50.0;
        }
        if (keystate[SDL_SCANCODE_D])
        {
            camera->strafe_speed = -50.0;
        }
    } else {
        camera->strafe_speed = 0.0;
    }
    if (keystate[SDL_SCANCODE_SPACE] || keystate[SDL_SCANCODE_LSHIFT] ) {
        if (keystate[SDL_SCANCODE_SPACE])
        {
            camera->up_speed = 30.0;
        }
        if (keystate[SDL_SCANCODE_LSHIFT])
        {
            camera->up_speed = -30.0;
        }
    } else {
        camera->up_speed = 0.0*properties.ticktime;
    }
    if (keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_LEFT] ) {
        if (keystate[SDL_SCANCODE_RIGHT])
        {
            camera->rotation.x -= 3.14159265359*properties.ticktime;
        }
        if (keystate[SDL_SCANCODE_LEFT])
        {
            camera->rotation.x += 3.14159265359*properties.ticktime;
        }
    }
    if (keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_DOWN] ) {
        if (keystate[SDL_SCANCODE_UP])
        {
            camera->rotation.y -= 3.14159265359*properties.ticktime;
        }
        if (keystate[SDL_SCANCODE_DOWN])
        {
            camera->rotation.y += 3.14159265359*properties.ticktime;
        }
    }
    //camera->phi += 0.39269908169*properties.ticktime*(mx*.25);
    //camera->theta += 0.39269908169*properties.ticktime*(my*.25);
    //printf("%d,%d\n",mx,my);
}

void tick_run(Properties *properties_ptr, Environment *environment) {
    // ticktime is seconds per tick
    Properties properties = *properties_ptr;
    uint32_t deltaT = SDL_GetTicks()-properties.previous_tick;
    if (deltaT >= properties.ticktime*1000)
    {
        properties_ptr->previous_tick = SDL_GetTicks();
        //printf("%lld, %u, %f\n",current_timestamp(),deltaT,properties.ticktime);
        // move camera
        Camera *camera = environment->camera;

        // give camera gravity
        //camera->up_speed += -2*properties.ticktime;
        //printf("%lf\n",camera->up_speed);

        SDL_PumpEvents();
        const Uint8 *keystate = SDL_GetKeyboardState(NULL);
        int mx,my;
        SDL_GetRelativeMouseState(&mx, &my);
        // TODO: move to more generalized function for movement related keys rather than just camera
        set_camera_velocities(camera, properties, keystate, mx, -1*my);

        vector_move_angled(
            &(camera->position),
            camera->rotation.x,
            camera->forward_speed*properties.ticktime
        );
        vector_move_angled(
            &(camera->position),
            camera->rotation.x + 1.57079632679,
            camera->strafe_speed*properties.ticktime
        );
        camera->position.z += camera->up_speed*properties.ticktime;

        // entity tick
        //int entity_count = environment->entity_count;
        //for (int i = 0; i < entity_count; i++)
        listNode *current_node = environment->entities;
        while (current_node != NULL)
        {
            //Entity *entity = environment->entities[i];
            Entity *entity = current_node->data;
            // Apply velocities
            
            // Collision Box Test Code
            int WORLD_SIZE = 100;
            if (entity->position.x < -WORLD_SIZE || entity->position.x > WORLD_SIZE) {
                if (entity->position.x < -WORLD_SIZE) {entity->position.x = -WORLD_SIZE;}
                if (entity->position.x > WORLD_SIZE) {entity->position.x = WORLD_SIZE;}
                entity->velocity.x = entity->velocity.x*-1;
            }
            entity->position.x += entity->velocity.x*properties.ticktime;
            if (entity->position.y < -WORLD_SIZE || entity->position.y > WORLD_SIZE) {
                if (entity->position.y < -WORLD_SIZE) {entity->position.y = -WORLD_SIZE;}
                if (entity->position.y > WORLD_SIZE) {entity->position.y = WORLD_SIZE;}
                entity->velocity.y = entity->velocity.y*-1;
            }
            entity->position.y += entity->velocity.y*properties.ticktime;
            if (entity->position.z < -WORLD_SIZE || entity->position.z > WORLD_SIZE) {
                if (entity->position.z < -WORLD_SIZE) {entity->position.z = -WORLD_SIZE;}
                if (entity->position.z > WORLD_SIZE) {entity->position.z = WORLD_SIZE;}
                entity->velocity.z = entity->velocity.z*-1;
            }
            entity->position.z += entity->velocity.z*properties.ticktime;
            //

            // Apply accelerations
            entity->velocity.x += entity->acceleration.x*properties.ticktime;
            entity->velocity.y += entity->acceleration.y*properties.ticktime;
            entity->velocity.z += entity->acceleration.z*properties.ticktime;

            // calculate interactions
            // not functional after move to LL for entities
            /*
            for (int j = 0; j < entity_count; j++)
            {
                Entity *interaction_entity = environment->entities[j];
                // don't interact with self
                if (i != j)
                {
                    // find position difference
                    double dx = interaction_entity->position.x - entity->position.x;
                    double dy = interaction_entity->position.y - entity->position.y;
                    double dz = interaction_entity->position.z - entity->position.z;

                    //Vector difference = vector_create(dx,dy,dz);
                    //double magnitute = vector_magnitute(difference);
                    double distance = vector_distance(entity->position,interaction_entity->position);
                    if (distance < .25) {
                        // bounce!
                        entity->velocity.x = entity->velocity.x*-1;
                        entity->velocity.y = entity->velocity.y*-1;
                        entity->velocity.z = entity->velocity.z*-1;
                    }
                    if (distance > 0 && entity->fixed == 0) {
                        double GRAVITY = 0.0000001;
                        double mass = interaction_entity->mass;
                        /* Gravity test code
                        entity->acceleration.x += mass*GRAVITY*dx/distance;
                        entity->acceleration.y += mass*GRAVITY*dy/distance;
                        entity->acceleration.z += mass*GRAVITY*dz/distance;
                        /
                    }
                }
            }
            */
            current_node = current_node->next;
        }
    }
}