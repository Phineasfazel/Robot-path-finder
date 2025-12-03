#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "world.h"
#include "robot.h"
#include "stack.h"

int **compute_reachable(World *world, int start_x, int start_y, int *reachable_count);
void dfs_find_markers(World *world, Robot *robot);
void generate_circular_arena(World *world);
void place_markers_on_reachable(World *world, Robot *robot, int **reachable);
void add_extra_obstacles_in_reachable_area(World *world, Robot *robot, int **reachable, int number_of_extra_obstacles);

#endif 