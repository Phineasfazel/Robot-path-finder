#include "stdio.h"
#include "stdlib.h"
#include "graphics.h"
#include "time.h"
#include "math.h"

#include "world.h"
#include "robot.h"
#include "algorithm.h"
#include "stack.h"
#include "draw.h"
#include "validation.h"

int main(int argc, char **argv){
    // default values
    int columns = 20;
    int rows = 20;
    int window_width = 700;
    int window_height = 700;
    int border_size = 10;
    int number_of_markers = 10;
    int number_of_obstacles = 20;
    int delay_time = 110;

    // overriding default values
    if (argc > 1){
        columns = atoi(argv[1]);
    }
    if (argc > 2){
        rows = atoi(argv[2]);
    }
    if (argc > 3){
        window_width = atoi(argv[3]);
    }
    if (argc > 4){
        window_height = atoi(argv[4]);
    }
    if (argc > 5){
        border_size = atoi(argv[5]);
    }
    if (argc > 6){
        number_of_markers = atoi(argv[6]);
    }
    if (argc > 7){
        number_of_obstacles = atoi(argv[7]);
    }
    if (argc > 8){
        delay_time = atoi(argv[8]);
    }

    if (validate_inputs(columns, rows, window_width, window_height, border_size, number_of_markers, number_of_obstacles, delay_time) == 0){
        return 0;
    }

    // picking a seed for random number generation
    srand(time(NULL));

    World *world = initialise_world(columns, rows, border_size, window_width, window_height, number_of_markers, number_of_obstacles, delay_time);
    Robot *robot = initialise_robot(world, columns, rows, north);

    generate_circular_arena(world);
    pick_robot_initial_position(world, robot);
    update_robot_map(world, robot);

    // determining the reachable tiles from the robot position and adding obstacles and markers
    int reachable_count = 0;
    int **reachable_array = compute_reachable(world, robot->x, robot->y, &reachable_count);
    // we can use the reachable array here since it will return all the empty tiles that don't already have an obstacle or the robot on them
    add_extra_obstacles_in_reachable_area(world, robot, reachable_array, number_of_obstacles);
    free_two_d_array(reachable_array, world->columns);
    // recomputing the reachable array since obstacles have been added
    reachable_count = 0;
    reachable_array = compute_reachable(world, robot->x, robot->y, &reachable_count);
    place_markers_on_reachable(world, robot, reachable_array);
    free_two_d_array(reachable_array, world->columns);

    // checking if the number of inputted obstacles and markers are logically possible
    if (validate_number_of_obstacles_and_markers(world->number_of_extra_obstacles, world->number_of_markers, number_of_obstacles, number_of_markers) == 0){
        return 0;
    }

    background();
    draw_grid(world, columns, rows, window_width, window_height, border_size);
    render_obstacles(world);

    foreground();
    draw_robot(world, robot);
    render_markers(world);

    dfs_find_markers(world, robot);

    destroy_robot(robot);
    destroy_world(world);

    return 0;
}