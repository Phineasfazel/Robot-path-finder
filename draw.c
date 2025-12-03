#include <stdlib.h>
#include "draw.h"
#include "graphics.h"
#include "world.h"
 
static void draw_obstacle(int tile_width, int tile_height, int window_x, int window_y){
    setColour(gray);
    // calculating the margin from the grid borders, so the obstacle doesnt take up the whole square
    int margin_x = (int)(tile_width * 0.25);
    int margin_y = (int)(tile_height * 0.25);

    int obstacle_x = window_x + margin_x / 2;
    int obstacle_y = window_y + margin_y / 2;
    int obstacle_width  = tile_width - margin_x;
    int obstacle_height = tile_height - margin_y;

    drawRect(obstacle_x, obstacle_y, obstacle_width, obstacle_height);
    fillRect(obstacle_x, obstacle_y, obstacle_width, obstacle_height);
}

static void draw_marker(int tile_width, int tile_height, int window_x, int window_y){
    setColour(yellow);
    int oval_size;
    // picking the smaller of the tile width and height for when rows != columns to ensure the marker fits in the tile
    if (tile_width < tile_height){
        oval_size = (int)(tile_width * 0.6);
    } else {
        oval_size = (int)(tile_height * 0.6);
    }
    // calculating offsets in order to centre the oval in the tile
    int offset_x = (int)((tile_width - oval_size) / 2);
    int offset_y = (int)((tile_height - oval_size) / 2);
    fillOval(window_x + offset_x, window_y + offset_y, oval_size, oval_size);
}

void place_marker(World *world, int logical_x, int logical_y){
    world->grid_array[logical_x][logical_y] = marker; 

    double tile_width = get_tile_width(world);
    double tile_height = get_tile_height(world);

    int *window_coordinates = get_window_coordinates(logical_x, logical_y, world->border_size, tile_width, tile_height);
    int window_x = window_coordinates[0];
    int window_y = window_coordinates[1];
    free(window_coordinates);

    // foreground since markers can be picked up (removed from the screen)
    foreground();
    draw_marker(tile_width, tile_height, window_x, window_y);
}

void place_obstacle(World *world, int logical_x, int logical_y){
    world->grid_array[logical_x][logical_y] = obstacle; 

    double tile_width = get_tile_width(world);
    double tile_height = get_tile_height(world);

    int *window_coordinates = get_window_coordinates(logical_x, logical_y, world->border_size, tile_width, tile_height);
    int window_x = window_coordinates[0];
    int window_y = window_coordinates[1];
    free(window_coordinates);

    // drawing on the background since obstacles are permanent
    background();
    draw_obstacle(tile_width, tile_height, window_x, window_y);
}

// draws all the markers that are in the world struct grid_array
void render_markers(World *world){
    foreground(); // foreground since markers can be picked up
    for (int x = 0; x < world->columns; x++) {
        for (int y = 0; y < world->rows; y++) {
            if (world->grid_array[x][y] == marker){
                place_marker(world, x, y);
            }
        }
    }
}

// draws all the obstacles that are in the world struct grid_array
void render_obstacles(World *world){
    background(); // background since obstacles are permanent
    for (int x = 0; x < world->columns; x++) {
        for (int y = 0; y < world->rows; y++) {
            if (world->grid_array[x][y] == obstacle){
                place_obstacle(world, x, y);
            }
        }
    }
}

static void draw_border(int window_width, int window_height, int border_size){
    background();
    setColour(red);
    fillRect(0, 0, window_width, border_size); 
    fillRect(0, 0, border_size, window_height);
    fillRect(window_width-border_size, 0, border_size, window_height); 
    fillRect(0, window_height-border_size, window_width, border_size);
}

static void draw_vertical_lines(int columns, int border_size, double tile_width, int window_height){
    setColour(black);
    for (int i = 0; i <= columns; i++){
        int x_coordinate = border_size + (int)((i * tile_width) + 0.5); // rounding for added accuracy
        drawLine(x_coordinate, border_size, x_coordinate, window_height-border_size);
    }
}

static void draw_horizontal_lines(int rows, int border_size, double tile_height, int window_width){
    setColour(black);
    for (int i = 0; i <= rows; i++){
        int y_coordinate = border_size + (int)((i * tile_height) + 0.5); // rounding for added accuracy
        drawLine(border_size, y_coordinate, window_width-border_size, y_coordinate);
    }
}

void draw_grid(World *world, int columns, int rows, int window_width, int window_height, int border_size){
    setWindowSize(window_width, window_height);

    draw_border(window_width, window_height, border_size);

    double tile_width = get_tile_width(world);
    double tile_height = get_tile_height(world);

    draw_vertical_lines(columns, border_size, tile_width, window_height);
    draw_horizontal_lines(rows, border_size, tile_height, window_width);
}