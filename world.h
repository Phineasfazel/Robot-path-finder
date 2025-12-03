#ifndef WORLD_H
#define WORLD_H

enum tile_type {empty, marker, obstacle, visited};
typedef enum tile_type tile_type;
 
typedef struct {
    int columns;
    int rows;
    int border_size;
    int window_width;
    int window_height;
    int number_of_markers;
    int number_of_extra_obstacles;
    int delay_time;
    tile_type **grid_array;
} World;

World* initialise_world(int, int, int, int, int, int, int, int);
void destroy_world(World *);
double get_tile_width(World *);
double get_tile_height(World *);
int* get_window_coordinates(int, int, int, double, double);
void free_two_d_array(int **, int);
int get_random_number(int, int);

#endif