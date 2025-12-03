#ifndef ROBOT_H
#define ROBOT_H

#include "world.h"

enum direction {north, east, south, west};
typedef enum direction direction;

typedef struct {
    int columns;
    int rows;
    int x;
    int y;
    int number_of_markers;
    direction direction;
    tile_type **robot_map;
} Robot;

// Required functions
void forward(World *world, Robot *robot);
void left(World *world, Robot *robot);
void right(World *world, Robot *robot);
int at_marker(World *world, Robot *robot);
int can_move_forward(World *world, Robot *robot);
void pick_up_marker(World *world, Robot *robot);
void drop_marker(World *world, Robot *robot);
int marker_count(Robot *robot);

Robot* initialise_robot(World *world, int columns, int rows, direction direction);
void destroy_robot(Robot *robot);
void pick_robot_initial_position(World *world, Robot *robot);
void draw_robot(World *world, Robot *robot);
void redraw_robot_and_markers(World *world, Robot *robot);
void update_robot_map(World *world, Robot *robot);

#endif