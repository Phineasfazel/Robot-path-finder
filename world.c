#include "stdlib.h"
#include "world.h"

World* initialise_world(int columns, int rows, int border_size, int window_width, int window_height, int number_of_markers, int number_of_obstacles, int delay_time) {
    World *world = malloc(sizeof(World));
    world->columns = columns;
    world->rows = rows;
    world->border_size = border_size;
    world->window_width = window_width;
    world->window_height = window_height;
    world->number_of_markers = number_of_markers;
    world->number_of_extra_obstacles = number_of_obstacles;
    world->delay_time = delay_time;

    // creating an array of row pointers
    world->grid_array = malloc(columns * sizeof(tile_type*));
    for (int x = 0; x < columns; x++) {
        world->grid_array[x] = malloc(rows * sizeof(tile_type));
    }
    for (int x = 0; x < columns; x++) {
        for (int y = 0; y < rows; y++) {
            world->grid_array[x][y] = empty;
        }
    }
    return world;
}
 
void destroy_world(World *world){
    for (int x = 0; x < world->columns; x++) {
        free(world->grid_array[x]);
    }
    free(world->grid_array);
    free(world);
}

double get_tile_width(World *world){
    double arena_width = world->window_width - (2.0*world->border_size);
    double tile_width = (double)(arena_width / world->columns);
    return tile_width;
}

double get_tile_height(World *world){
    double arena_height = world->window_height - (2.0*world->border_size);
    double tile_height = (double)(arena_height / world->rows);
    return  tile_height;
}

// converts logical grid coordinates into actual coordinates on the window
int* get_window_coordinates(int logical_x, int logical_y, int border_size, double tile_width, double tile_height){
    int *window_coordinates = malloc(2 * sizeof(int));
    window_coordinates[0] = border_size + (int)((logical_x * tile_width) + 0.5); 
    window_coordinates[1] = border_size + (int)((logical_y * tile_height) + 0.5);
    return window_coordinates;
}

int get_random_number(int lower_bound, int upper_bound){
    int random_number = (rand() % (upper_bound - lower_bound + 1)) + lower_bound;
    return random_number;
}

void free_two_d_array(int **two_d_array, int columns) {
    for (int i = 0; i < columns; i++){
        free(two_d_array[i]);
    }
    free(two_d_array);
}