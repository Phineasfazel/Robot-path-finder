My program generates a grid-based world in which a robot navigates obstacles to collect markers.
A grid is generated of which the number of rows can be chosen.
A circular arrangement of obstacles is drawn on the grid to create an open central area shaped like a circle.
The user can add extra obstacles to the central area of the grid.
The user can choose how many markers they want to place on the grid.
The robot is given a random position and will move around the grid until it finds all the markers. It uses a depth first search algorithm to search for the markers.
An algorithm is used to ensure that all the markers can be reached by the robot, so it is never possible for all the markers not to be collected.
The grid does not have to have the same number of columns as rows. They can be different, and an elliptical shaped central area will form.
The user can choose the number of rows, columns, window width, window height, border size, number of markers and obstacles, and delay time.

I use a struct to represent the world. It contains all the information regarding the world and contains a 2d array representing the grid.
I use a struct to represent the robot. The robot is given the number of rows and columns of the grid. The robot has an internal map which represents what it has seen so far and is updated as the robot moves. The robot is not given the position of the markers or obstacles to start.

Different files:
main.c - initialises the world and robot, draws the grid, obstacles and markers, and calls a function to start the robot marker search algorithm.
validation.c - validates the inputs given by the user.
world.c - contains functions relating to the world like initialisation and various general utility functions.
robot.c - contains the functions related to the robot. Implements initialisation, movement, turning, picking up markers, updating the internal robot map, and drawing the robot.
draw.c - contains the functions that draw the grid, border, obstacles and markers.
stack.c - implements the stack data structure.
algorithm.c - contains the key algorithms required like depth first search, computing the reachable tiles and generating a circular arena.

Running the program:
To compile the files write "gcc main.c world.c graphics.c algorithm.c robot.c stack.c draw.c validation.c -o main" in the terminal.
To run the source code write "./main | java -jar drawapp-4.5.jar".
To input parameters like number of rows or columns add these in the running source code line. For example 20 columns, 25 rows, 700x700 window, 8 border size, 5 markers, 10 obstacles, 100ms delay time: "./main 20 25 700 700 8 5 10 100 | java -jar drawapp-4.5.jar". In the form "./main columns rows window_width window_height border markers obstacles delay_time | java -jar drawapp-4.5.jar" with the parameters replaced by numberes. The user can also just include some of the parameters, not al of them. If none are provided default values are chosen.

Additional comments:
- I chose the world and robot 2d arrays to be of the form array[x][y] -  columns x rows (not the matrix form - rows x columns) to make it more intuitive to access elements in the grid by using their x and y coordinates i.e. array[x_coordinate][y_coordinate] and convert between array indexes/logical coordinates and window coordinates.
- Randomisation seeded with the current time is used to generate the arena so each run generates a random arena.