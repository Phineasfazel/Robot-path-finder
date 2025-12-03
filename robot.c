#include <stdlib.h>
#include "graphics.h"
#include "robot.h"
#include "algorithm.h"
#include "world.h"
#include "draw.h"

// update triangle coordinates for when robot is facing north
static void get_north_coordinates(int tile_centre_x, int tile_centre_y, int tile_width, int tile_height, int x_coordinates[3], int y_coordinates[3]){
    // using the smaller of tile width and height for the triangle side width
    // so the robot triangle always fits inside the tile/looks proportionate when rows != columns
    double side_length;
    if (tile_width <= tile_height){
        side_length = tile_width/2;
    } else {
        side_length = tile_height/2;
    }

    x_coordinates[0] = (int)(tile_centre_x + 0.5); // rounding coordinates
    x_coordinates[1] = (int)(tile_centre_x + (side_length/2) + 0.5);
    x_coordinates[2] = (int)(tile_centre_x - (side_length/2) + 0.5);
    y_coordinates[0] = (int)(tile_centre_y - (side_length/2) + 0.5);
    y_coordinates[1] = (int)(tile_centre_y + (side_length/2) + 0.5);
    y_coordinates[2] = (int)(tile_centre_y + (side_length/2) + 0.5);
}

// update triangle coordinates for when robot is facing north
static void get_east_coordinates(int tile_centre_x, int tile_centre_y, int tile_width, int tile_height, int x_coordinates[3], int y_coordinates[3]){
    // using the smaller of tile width and height for the triangle side width
    // so the robot triangle always fits inside the tile/looks proportionate when rows != columns
    double side_length;
    if (tile_width <= tile_height){
        side_length = tile_width/2;
    } else {
        side_length = tile_height/2;
    }

    x_coordinates[0] = (int)(tile_centre_x + (side_length/2) + 0.5); // rounding coordinates
    x_coordinates[1] = (int)(tile_centre_x - (side_length/2) + 0.5);
    x_coordinates[2] = (int)(tile_centre_x - (side_length/2) + 0.5);
    y_coordinates[0] = (int)(tile_centre_y + 0.5);
    y_coordinates[1] = (int)(tile_centre_y + (side_length/2) + 0.5);
    y_coordinates[2] = (int)(tile_centre_y - (side_length/2) + 0.5);
}

// update triangle coordinates for when robot is facing north
static void get_south_coordinates(int tile_centre_x, int tile_centre_y, int tile_width, int tile_height, int x_coordinates[3], int y_coordinates[3]){
    // using the smaller of tile width and height for the triangle side width
    // so the robot triangle always fits inside the tile/looks proportionate when rows != columns
    double side_length;
    if (tile_width <= tile_height){
        side_length = tile_width/2;
    } else {
        side_length = tile_height/2;
    }

    x_coordinates[0] = (int)(tile_centre_x + 0.5); // rounding coordinates
    x_coordinates[1] = (int)(tile_centre_x - (side_length/2) + 0.5);
    x_coordinates[2] = (int)(tile_centre_x + (side_length/2) + 0.5);
    y_coordinates[0] = (int)(tile_centre_y + (side_length/2) + 0.5);
    y_coordinates[1] = (int)(tile_centre_y - (side_length/2) + 0.5);
    y_coordinates[2] = (int)(tile_centre_y - (side_length/2) + 0.5);
}

// update triangle coordinates for when robot is facing north
static void get_west_coordinates(int tile_centre_x, int tile_centre_y, int tile_width, int tile_height, int x_coordinates[3], int y_coordinates[3]){
    // using the smaller of tile width and height for the triangle side width
    // so the robot triangle always fits inside the tile/looks proportionate when rows != columns
    double side_length;
    if (tile_width <= tile_height){
        side_length = tile_width/2;
    } else {
        side_length = tile_height/2;
    }

    x_coordinates[0] = (int)(tile_centre_x - (side_length/2) + 0.5); // rounding coordinates
    x_coordinates[1] = (int)(tile_centre_x + (side_length/2) + 0.5);
    x_coordinates[2] = (int)(tile_centre_x + (side_length/2) + 0.5);
    y_coordinates[0] = (int)(tile_centre_y + 0.5);
    y_coordinates[1] = (int)(tile_centre_y - (side_length/2) + 0.5);
    y_coordinates[2] = (int)(tile_centre_y + (side_length/2) + 0.5);
}

Robot *initialise_robot(World *world, int columns, int rows, direction direction){
    Robot *robot = malloc(sizeof(Robot));
    robot->columns = columns;
    robot->rows = rows;
    robot->direction = direction;
    // initially the robot has picked up 0 markers
    robot->number_of_markers = 0;

    // making the robot map the size of the grid
    robot->robot_map = malloc(columns * sizeof(tile_type*));
    for (int x = 0; x < columns; x++) {
        robot->robot_map[x] = malloc(rows * sizeof(tile_type));
    }
    // set all tiles initially to empty 
    for (int x = 0; x < columns; x++) {
        for (int y = 0; y < rows; y++) {
            robot->robot_map[x][y] = empty;
        }
    }
    return robot;
}

void destroy_robot(Robot *robot){
    for (int x = 0; x < robot->columns; x++){
        free(robot->robot_map[x]);
    }
    free(robot->robot_map);
    free(robot);
}

void pick_robot_initial_position(World *world, Robot *robot){
    int x;
    int y;
    int reachable_count = 0;
    int valid_position = 0;

    // calculate the amount of reachable tiles for a random x, y position and if it passes a condition use it
    while (valid_position == 0) {
        x = get_random_number(0, world->columns - 1);
        y = get_random_number(0, world->rows - 1);

        // if the tile is not empty pick a new x and y
        if (world->grid_array[x][y] != empty) {
            continue;
        }

        int **reachable = compute_reachable(world, x, y, &reachable_count);

        // deciding if region is large enough - require at least 20% of tiles to be accessible (changeable)
        int total_tiles = world->rows * world->columns;
        if (reachable_count >= total_tiles * 0.2) {
            valid_position = 1;
        }
        free_two_d_array(reachable, world->columns);
    }
    robot->x = x;
    robot->y = y;
}

// assigns triangle coordinates to x and y coordinate arrays
static void assign_triangle_coordinates(double tile_centre_x, double tile_centre_y, double tile_width, double tile_height, int x_coordinates[3], int y_coordinates[3], direction direction){
    if (direction == north) {
        get_north_coordinates(tile_centre_x, tile_centre_y, tile_width, tile_height, x_coordinates, y_coordinates);
    } else if (direction == east) {
        get_east_coordinates(tile_centre_x, tile_centre_y, tile_width, tile_height, x_coordinates, y_coordinates);
    } else if (direction == south) {
        get_south_coordinates(tile_centre_x, tile_centre_y, tile_width, tile_height, x_coordinates, y_coordinates);
    } else {
        get_west_coordinates(tile_centre_x, tile_centre_y, tile_width, tile_height, x_coordinates, y_coordinates);
    }
}

void draw_robot(World *world, Robot *robot){
    foreground();
    double tile_width = get_tile_width(world);
    double tile_height = get_tile_height(world);

    int *window_coords = get_window_coordinates(robot->x, robot->y, world->border_size, tile_width, tile_height);
    int window_x = window_coords[0];
    int window_y = window_coords[1];
    free(window_coords);

    double tile_centre_x = window_x + (tile_width / 2);
    double tile_centre_y = window_y + (tile_height / 2);

    int x_coordinates[3];
    int y_coordinates[3];
    assign_triangle_coordinates(tile_centre_x, tile_centre_y, tile_width, tile_height, x_coordinates, y_coordinates, robot->direction);

    setColour(blue);
    fillPolygon(3, x_coordinates, y_coordinates);
    // draw triangle back line different colour
    setColour(red);
    drawLine(x_coordinates[1], y_coordinates[1], x_coordinates[2], y_coordinates[2]);
}

static int can_move_north(World *world, Robot *robot){
    // checking if the robot facing a wall
    if (robot->y == 0){
        return 0;
    // checking if the tile above it is empty
    } else if (world->grid_array[robot->x][robot->y - 1] == obstacle) {
        return 0;
    } else {
        return 1;
    }
}

static int can_move_east(World *world, Robot *robot){
    // checking if the robot is facing a wall
    if (robot->x == world->columns - 1){
        return 0;
    // checking if the tile to the right of it is empty
    } else if (world->grid_array[robot->x + 1][robot->y] == obstacle) {
        return 0;
    } else {
        return 1;
    }
}

static int can_move_south(World *world, Robot *robot){
    // checking if the robot is facing a wall
    if (robot->y == world->rows - 1){
        return 0;
    // checking if the tile below it is empty
    } else if (world->grid_array[robot->x][robot->y + 1] == obstacle) {
        return 0;
    } else {
        return 1;
    }
}

static int can_move_west(World *world, Robot *robot){
    // checking if the robot is facing a wall
    if (robot->x == 0){
        return 0;
    // checking if the tile to the left of it is empty
    } else if (world->grid_array[robot->x - 1][robot->y] == obstacle) {
        return 0;
    } else {
        return 1;
    }
}

int can_move_forward(World *world, Robot *robot){
    if (robot->direction == north){
        return can_move_north(world, robot);
    } else if (robot->direction == east){
        return can_move_east(world, robot);
    } else if (robot->direction == south){
        return can_move_south(world, robot);
    } else {
        return can_move_west(world, robot);
    }
}

int at_marker(World *world, Robot *robot){
    if (world->grid_array[robot->x][robot->y] == marker){
        return 1;
    } else {
        return 0;
    }
}

int marker_count(Robot *robot){
    return robot->number_of_markers;
}

void redraw_robot_and_markers(World *world, Robot *robot){
    foreground();
    clear();
    render_markers(world);
    draw_robot(world, robot);
}

void pick_up_marker(World *world, Robot *robot){
     if (world->grid_array[robot->x][robot->y] == marker) {
        robot->number_of_markers = marker_count(robot) + 1;
        // removing the marker from the grid array in the world struct
        world->grid_array[robot->x][robot->y] = empty;
        robot->robot_map[robot->x][robot->y] = visited;

        redraw_robot_and_markers(world, robot);
     }
}

void drop_marker(World *world, Robot *robot){
    if (robot->number_of_markers == 0){
        return;
    }
    robot->number_of_markers = marker_count(robot) - 1;
    world->grid_array[robot->x][robot->y] = marker;

    redraw_robot_and_markers(world, robot);
}

void forward(World *world, Robot *robot){
    robot->robot_map[robot->x][robot->y] = visited;
    int robot_can_move_forward = can_move_forward(world, robot);
    if (robot_can_move_forward == 1){
        // adjusting x, y coordinates according to the direction the robot is pointing
        if (robot->direction == north){
            robot->y = robot->y - 1;
            redraw_robot_and_markers(world, robot);
        } else if (robot->direction == east){
            robot->x = robot->x + 1;
            redraw_robot_and_markers(world, robot);
        } else if (robot->direction == south){
            robot->y = robot->y + 1;
            redraw_robot_and_markers(world, robot);
        } else {
            robot->x = robot->x - 1;
            redraw_robot_and_markers(world, robot);
        }
    }
    if (at_marker(world, robot) == 1){
        pick_up_marker(world, robot);
    }
}

void right(World *world, Robot *robot){
    robot->direction = (robot->direction + 1) % 4;
    redraw_robot_and_markers(world, robot);
}

void left(World *world, Robot *robot){
    right(world, robot);
    right(world, robot);
    right(world, robot);
}

// updating the robot map in the robot struct to store the tiles it has seen
void update_robot_map(World *world, Robot *robot){
    robot->robot_map[robot->x][robot->y] = visited;
    
    // updating the tile in front of the robot (in the direction it is facing), if its not facing a wall
    if (robot->direction == north){
        // checking if the robot facing a wall
        if (robot->y == 0){
            return;
        } else {
            robot->robot_map[robot->x][robot->y-1] = world->grid_array[robot->x][robot->y-1];
        }
    } else if (robot->direction == east){
        // checking if the robot is facing a wall
        if (robot->x == world->columns - 1){
            return;
        } else {
            robot->robot_map[robot->x+1][robot->y] = world->grid_array[robot->x+1][robot->y];
        }
    } else if (robot->direction == south){
        // checking if the robot is facing a wall
        if (robot->y == world->rows - 1){
            return;
        } else {
            robot->robot_map[robot->x][robot->y+1] = world->grid_array[robot->x][robot->y+1];
        }
    } else {
        // checking if the robot is facing a wall
        if (robot->x == 0){
            return;
        } else {
            robot->robot_map[robot->x-1][robot->y] = world->grid_array[robot->x-1][robot->y];
        }
    }
}
