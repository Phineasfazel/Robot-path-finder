#ifndef DRAW_H
#define DRAW_H

#include "world.h"

void draw_grid(World *world, int columns, int rows, int window_width, int window_height, int border_size);
void render_obstacles(World *world);
void render_markers(World *world);
void place_marker(World *world, int logical_x, int logical_y);
void place_obstacle(World *world, int logical_x, int logical_y);

#endif