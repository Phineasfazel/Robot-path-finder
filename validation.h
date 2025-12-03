#ifndef VALIDATION_H
#define VALIDATION_H

int validate_inputs(int columns, int rows, int window_width, int window_height, int border_size, int number_of_markers, int number_of_obstacles, int delay_time);
int validate_number_of_obstacles_and_markers(int world_number_of_extra_obstacles, int world_number_of_markers, int inputted_number_of_obstacles, int inputted_number_of_markers);

#endif