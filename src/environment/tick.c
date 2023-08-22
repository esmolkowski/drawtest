#include <stdint.h>
#include "environment/tick.h"
#include "app.h"
#include "vector.h"
#include "datastructures/linked_list.h"
#include "environment/environment.h"
#include <math.h>
#include <sys/time.h>
#include <stdbool.h>
#include "interfaces/inputs.h"
#include "interfaces/timing.h"

#define M_PI 3.14159265358979323846

void tick_handle_inputs(Environment *environment, Properties *properties) {
    // TODO: Re-add projectile shooting :)
    /*
                    // Fire projectile
                    listNode *projectile_node = environment_add_entity(environment, entity_create(
                        NULL, NULL, '.', color(255,255,0)
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
    */
}

void set_camera_velocities(Camera *camera, Properties *properties)
{
    InputActions inputs = *(properties->inputs);
    // forward-back
    camera->forward_speed = 0;
    camera->strafe_speed = 0;
    camera->up_speed = 0;
    if ( inputs.forward ) { camera->forward_speed += 50; }
    if ( inputs.backward ) { camera->forward_speed += -50; }
    if ( inputs.strafe_left ) { camera->strafe_speed += 50; }
    if ( inputs.strafe_right ) { camera->strafe_speed += -50; }
    if ( inputs.rise ) { camera->up_speed += 30; }
    if ( inputs.fall ) { camera->up_speed += -30; }

    // TODO: add angular velocities to camera
    if (inputs.right)
    {
        camera->rotation.x -= 3.14159265359*properties->ticktime*properties->camera_rotate_speed;
    }
    if (inputs.left)
    {
        camera->rotation.x += 3.14159265359*properties->ticktime*properties->camera_rotate_speed;
    }

    if (inputs.up)
    {
        camera->rotation.y -= 3.14159265359*properties->ticktime*properties->camera_rotate_speed;
    }
    if (inputs.down)
    {
        camera->rotation.y += 3.14159265359*properties->ticktime*properties->camera_rotate_speed;
    }
}

void tick_run(Properties *properties, Environment *environment) {
    // ticktime is seconds per tick
    uint32_t deltaT = interface_get_milliseconds(properties)-properties->previous_tick;
    if (deltaT >= properties->ticktime*1000)
    {
        properties->previous_tick = interface_get_milliseconds(properties);

        // move camera
        Camera *camera = environment->camera;

        // TODO: move to more generalized function for movement related keys rather than just camera
        set_camera_velocities(camera, properties);

        // Move camera in space
        vector_move_angled(
            &(camera->position),
            camera->rotation.x,
            camera->forward_speed*properties->ticktime
        );
        vector_move_angled(
            &(camera->position),
            camera->rotation.x + 1.57079632679,
            camera->strafe_speed*properties->ticktime
        );
        camera->position.z += camera->up_speed*properties->ticktime;

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
            entity->position.x += entity->velocity.x*properties->ticktime;
            if (entity->position.y < -WORLD_SIZE || entity->position.y > WORLD_SIZE) {
                if (entity->position.y < -WORLD_SIZE) {entity->position.y = -WORLD_SIZE;}
                if (entity->position.y > WORLD_SIZE) {entity->position.y = WORLD_SIZE;}
                entity->velocity.y = entity->velocity.y*-1;
            }
            entity->position.y += entity->velocity.y*properties->ticktime;
            if (entity->position.z < -WORLD_SIZE || entity->position.z > WORLD_SIZE) {
                if (entity->position.z < -WORLD_SIZE) {entity->position.z = -WORLD_SIZE;}
                if (entity->position.z > WORLD_SIZE) {entity->position.z = WORLD_SIZE;}
                entity->velocity.z = entity->velocity.z*-1;
            }
            entity->position.z += entity->velocity.z*properties->ticktime;
            
            entity->rotation.x += entity->angular_velocity.x*properties->ticktime;
            entity->rotation.y += entity->angular_velocity.y*properties->ticktime;
            entity->rotation.z += entity->angular_velocity.z*properties->ticktime;

            // Apply accelerations
            entity->velocity.x += entity->acceleration.x*properties->ticktime;
            entity->velocity.y += entity->acceleration.y*properties->ticktime;
            entity->velocity.z += entity->acceleration.z*properties->ticktime;

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