#include <stdio.h>
#include "validation.h"

// check if inputted number of obstacles and markers is logically possible
int validate_number_of_obstacles_and_markers(int world_number_of_extra_obstacles, int world_number_of_markers, int inputted_number_of_obstacles, int inputted_number_of_markers){
    // the add_extra_obstacles_in_reachable_area function in algorithm.c will change the world struct number of obstacles if its not possible to place all the obstacles
    if (world_number_of_extra_obstacles != inputted_number_of_obstacles){
        fprintf(stderr, "Decrease the number of obstacles\n");
        return 0;
    // the place_markers_on_reachable function in algorithm.c will change the world struct number of makers if its not possible to place all the markers
    } else if (world_number_of_markers != inputted_number_of_markers){
        fprintf(stderr, "Decrease the number of obstacles and/or number of markers\n");
        return 0;
    } else {
        return 1;
    }
}

int validate_inputs(int columns, int rows, int window_width, int window_height, int border_size, int number_of_markers, int number_of_obstacles, int delay_time){
    if (columns < 1){
        fprintf(stderr, "There must be at least 1 column\n");
        return 0;
    } else if (rows < 1){
        fprintf(stderr, "There must be at least 1 row\n");
        return 0;
    } else if (window_width < 100){
        fprintf(stderr, "Window width must be at least 100 pixels\n");
        return 0;
    } else if (window_height < 100){
        fprintf(stderr, "Window height must be at least 100 pixels\n");
        return 0;
    } else if (border_size < 0){
        fprintf(stderr, "Border size can't be negative\n");
        return 0;
    } else if (delay_time < 1){
        fprintf(stderr, "Delay time must be at least 1\n");
        return 0;
    } else if (number_of_obstacles < 0){
        fprintf(stderr, "Number of obstacles can't be negative\n");
        return 0;
    } else if (number_of_markers < 0){
        fprintf(stderr, "Number of markers can't be negative\n");
        return 0;
    } else {
        return 1;
    }
}