#include <stdlib.h>
#include <math.h>
#include "algorithm.h"
#include "graphics.h"

static int **allocate_two_d_array(int columns, int rows){
    int **reachable_array = malloc(columns * sizeof(int*));
    for (int i = 0; i < columns; i++){
        reachable_array[i] = calloc(rows, sizeof(int));
    }
    return reachable_array;
}

// performs depth first search assigning 1 to the reachable tiles in the array
static void reachable_tiles_dfs(World *world, Stack *stack, int **reachable_array, int *count){
    int directions[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}}; // NESW respectively
    // depth first search
    while (stack->number_of_elements > 0){
        Position top_position = pop_from_stack(stack);
        // check all four neighbours of the current tile
        for (int direction = 0; direction < 4; direction++) {
            int neighbour_x = top_position.x + directions[direction][0];
            int neighbour_y = top_position.y + directions[direction][1];

            // skip tiles that are outside the grid
            if (neighbour_x < 0 || neighbour_x >= world->columns || neighbour_y < 0 || neighbour_y >= world->rows){
                continue;
            }
            // skip tiles that are already visited
            if (reachable_array[neighbour_x][neighbour_y] == 1){
                continue;
            }
            if (world->grid_array[neighbour_x][neighbour_y] == obstacle){
                continue;
            }

            // if tile hasn't been skipped its reachable, so mark it as reachable and add to the stack
            reachable_array[neighbour_x][neighbour_y] = 1;
            (*count)++;
            push_onto_stack(stack, (Position){neighbour_x, neighbour_y});
        }
    }
}

// returns a columns x rows 2d array of 1s and 0s where 1 means the tile is reachable and 0 means the tile is not reachable
int **compute_reachable(World *world, int start_x, int start_y, int *reachable_count) {
    int **reachable_array = (allocate_two_d_array(world->columns, world->rows));
    Stack *stack = create_stack();
    push_onto_stack(stack, (Position){start_x, start_y});
    // marking the starting tile as reachable
    reachable_array[start_x][start_y] = 1;
    int count = 1;

    // changes the reachable array to have 1s in all the reachable tiles and 0s in the rest
    reachable_tiles_dfs(world, stack, reachable_array, &count);

    stack_free(stack);
    *reachable_count = count;
    return reachable_array;
}

static void orient_robot_given_direction(int d, World *world, Robot *robot){
    if (d == 0){ 
        while (robot->direction != north){
            right(world, robot);
        }
    } else if (d == 1){
        while (robot->direction != east) {
            right(world, robot); 
        }
    } else if (d == 2){
        while (robot->direction != south){
            right(world, robot); 
        }
    } else if (d == 3){ 
        while (robot->direction != west){
            right(world, robot);
        }
    }
}

static void orient_robot_given_direction_x_y(World *world, Robot *robot, int direction_x, int direction_y){
    if (direction_x == 1 && direction_y == 0){
        while (robot->direction != east){
            right(world, robot);
        }
    } else if (direction_x == -1 && direction_y == 0){
        while (robot->direction != west){
            right(world, robot);
        }
    } else if (direction_x == 0 && direction_y == 1){
        while (robot->direction != south){
            right(world, robot);
        }
    } else if (direction_x == 0 && direction_y == -1){
        while (robot->direction != north){
            right(world, robot);
        }
    }
}

static void dfs_move_robot_forward(World *world, Robot *robot, Stack *stack, int new_x, int new_y, int *found_unvisited){
    forward(world, robot);
    update_robot_map(world, robot);
    // adding this position to the stack
    Position visited_position = {new_x, new_y};
    push_onto_stack(stack, visited_position);

    *found_unvisited = 1;
    sleep(world->delay_time);
}

static void dfs_explore_neighbours(World *world, Robot *robot, Stack *stack, int x, int y, int *found_unvisited){
    int directions[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}}; // NESW respectively
    // looping through each neighbour
    for (int direction = 0; direction < 4; direction++){
        int neighbour_x = x + directions[direction][0];
        int neighbour_y = y + directions[direction][1];

        // if the next coordinates are out of bounds skip to the next loop iteration
        if (neighbour_x < 0 || neighbour_x >= world->columns || neighbour_y < 0 || neighbour_y >= world->rows)
            continue;

        if (robot->robot_map[neighbour_x][neighbour_y] == empty || robot->robot_map[neighbour_x][neighbour_y] == marker){
            orient_robot_given_direction(direction, world, robot);
            sleep(world->delay_time);

            if (can_move_forward(world, robot) == 1){
                dfs_move_robot_forward(world, robot, stack, neighbour_x, neighbour_y, found_unvisited);
                break;
            } else {
                robot->robot_map[neighbour_x][neighbour_y] = obstacle;
            }
        }
    }
}

static void dfs_backtrack(World *world, Robot *robot, Stack *stack){
    pop_from_stack(stack);
    if (stack->number_of_elements > 0){
        // move the robot back to its last position
        Position last_position = peek(stack);
        int direction_x = last_position.x - robot->x;
        int direction_y = last_position.y - robot->y;
        orient_robot_given_direction_x_y(world, robot, direction_x, direction_y);
        forward(world, robot);
        sleep(world->delay_time);
    }
}

void dfs_find_markers(World *world, Robot *robot) {
    Stack *stack = create_stack();
    Position start_position = {robot->x, robot->y};
    push_onto_stack(stack, start_position);
    robot->robot_map[robot->x][robot->y] = visited;

    while (stack->number_of_elements > 0){
        Position current_position = peek(stack);
        int x = current_position.x;
        int y = current_position.y;
        int found_unvisited = 0;

        dfs_explore_neighbours(world, robot, stack, x, y, &found_unvisited);

        // if there are no unvisited neighbours then backtrack
        if (found_unvisited == 0){
            dfs_backtrack(world, robot, stack);
        }

        if (robot->number_of_markers == world->number_of_markers)
            break;
        if (stack->number_of_elements == 0)
            break;
    }
    stack_free(stack);
}

static void assign_obstacles_for_circular_arena(World *world, double tile_width, double tile_height, double grid_centre_x, double grid_centre_y, double radius_pixels){
    // looping through each element of the grid and seeing if it is inside the circle
    for (int x = 0; x < world->columns; x++){
        for (int y = 0; y < world->rows; y++){
            double tile_centre_x = world->border_size + ((x + 0.5) * tile_width);
            double tile_centre_y = world->border_size + ((y + 0.5) * tile_height);
            double distance_x = tile_centre_x - grid_centre_x;
            double distance_y = tile_centre_y - grid_centre_y;
            // use pythagoras to compute the straight line distane between the cell
            double distance = sqrt(distance_x * distance_x + distance_y * distance_y);

            // if tile is inside the circle make it empty, outside the circle make it an obstacle
            if (distance < radius_pixels){
                world->grid_array[x][y] = empty;
            } else{
                world->grid_array[x][y] = obstacle;
            }
        }
    }
}

void generate_circular_arena(World *world) {
    double grid_width = world->window_width - (2 * world->border_size);
    double grid_height = world->window_height - (2 * world->border_size);
    double tile_width = grid_width / world->columns;
    double tile_height = grid_height / world->rows;
    double grid_centre_x = world->border_size + (grid_width / 2.0);
    double grid_centre_y = world->border_size + (grid_height / 2.0);

    // choose radius as a fraction of the smaller arena dimension (0.4 here is the fill ratio but can be changed)
    double radius_pixels;
    if (grid_width < grid_height){
        radius_pixels = grid_width * 0.4;
    } else {
        radius_pixels = grid_height * 0.4;
    }

    assign_obstacles_for_circular_arena(world, tile_width, tile_height, grid_centre_x, grid_centre_y, radius_pixels);
}

// collects all the reachable empty tiles in a 1d array of positions from a 2d array
static Position* collect_reachable_empty_tiles(World *world, Robot *robot, int **reachable, int *reachable_tile_count){
    Position *reachable_tiles = malloc(world->columns * world->rows * sizeof(Position));
    int tile_count = 0;

    for (int x = 0; x < world->columns; x++) {
        for (int y = 0; y < world->rows; y++) {
            // if a tile is reachable, empty and robot isnt on it, its added to the reachable tiles 1d array
            if (reachable[x][y] == 1 && world->grid_array[x][y] == empty) {
                if (x == robot->x && y == robot->y)
                    continue;
                reachable_tiles[tile_count] = (Position){x, y};
                tile_count++;
            }
        }
    }
    *reachable_tile_count = tile_count;
    return reachable_tiles;
}

static void pick_random_positions(World *world, Position *reachable_tiles, int *reachable_tile_count, int number_to_pick, tile_type tile_type){
    for (int i = 0; i < number_to_pick; i++){
        int random_index = get_random_number(0, *reachable_tile_count - 1);

        int position_x = reachable_tiles[random_index].x;
        int position_y = reachable_tiles[random_index].y;
        // place tile type
        world->grid_array[position_x][position_y] = tile_type;

        // remove this option its not used again
        reachable_tiles[random_index] = reachable_tiles[*reachable_tile_count - 1];
        (*reachable_tile_count)--;
    }
}

void place_markers_on_reachable(World *world, Robot *robot, int **reachable) {
    int reachable_tile_count = 0;
    Position *reachable_tiles = collect_reachable_empty_tiles(world, robot, reachable, &reachable_tile_count);

    // when number of markers is greater than number of reachable tiles
    if (reachable_tile_count < world->number_of_markers){
        if (reachable_tile_count == 0){
            world->number_of_markers = 0;
            free(reachable_tiles);
            return;
        }
        world->number_of_markers = reachable_tile_count;
    }

    pick_random_positions(world, reachable_tiles, &reachable_tile_count, world->number_of_markers, marker);
    free(reachable_tiles);
}

void add_extra_obstacles_in_reachable_area(World *world, Robot *robot, int **reachable, int number_of_extra_obstacles){
    int reachable_tile_count = 0;
    Position *reachable_tiles = collect_reachable_empty_tiles(world, robot, reachable, &reachable_tile_count);

    // when number of obstacles is greater than number of reachable tiles
    if (reachable_tile_count < number_of_extra_obstacles){
        if (reachable_tile_count == 0){
            world->number_of_extra_obstacles = 0;
            free(reachable_tiles);
            return;
        }
        number_of_extra_obstacles = reachable_tile_count;
        world->number_of_extra_obstacles = number_of_extra_obstacles;
    }

    pick_random_positions(world, reachable_tiles, &reachable_tile_count, number_of_extra_obstacles, obstacle);
    free(reachable_tiles);
}