# Path-Finder-Robot

This program simulates a robot navigating an arena to collect markers while avoiding obstacles. The robot starts at a random position and markers and obstacles also spawn at random positions. The robot moves towards markers, collecting them, and finally returns to a home position.

## Functions

### `InitializeHome(int robotX, int robotY)`
Initializes the home position of the robot at a random location within the arena, ensuring it does not overlap with the robot's starting position or any obstacles or markers.

### `initializeArenaArray()`
Sets up the arena grid as a 2d array, marking free spaces as 0, markers as 1, obstacles as 2 and border walls as 2.

### `GenerateRandomCoords(int min, int max)`
Generates random numbers within the specified range, ensuring they align with the cell size, this is used to generate random arena size, random position of markers, random position of obstacles and random starting position of the robot.

### `SetBackground()`
Sets the background of the arena, by drawing the outside wall, the inner arena and the grid lines.

### `drawMarkers(int roboX, int roboY)`
Randomly places markers in the arena, ensuring they do not overlap with the robot's starting position or obstacles.

### `drawObstacles(int roboX, int roboY)`
Randomly places obstacles in the arena, ensuring they do not overlap with the robot's starting position or markers.

### `initializeRobot(struct robot *r, int startX, int startY, int direction)`
Initializes the robot's starting position and direction.

### `drawRobot(struct robot *r)`
Draws the robot on the screen based on its current position and direction, by calculating the coordinates of all three edges of the triangle shape from the centre coordinates of the cell it is currently in.

### `eraseRobot(struct robot *r)`
Erases the robot from its current position on the screen by drawing a background colored rectangle over it so that the movement of the robot then looks like a smooth transition.

### `pickupMarker(struct robot *r, int x, int y)`
Simulates the robot picking up a marker by erasing it from the screen and updating the arena 2d array by marking the marker position from 1 to 0.

### `turnLeft(struct robot *r)`
Turns the robot 90 degrees to the left.

### `turnRight(struct robot *r)`
Turns the robot 90 degrees to the right.

### `moveForward(struct robot *r)`
Moves the robot one cell forward in its current direction.

### `faceDirection(struct robot *r, int targetDirection)`
Turns the robot to face the specified direction, makes use of the fact that the directions have been defined as constant integers (North = 0, East = 1, South = 2, West = 3). It makes use of a combination of turn right and left functions to turn the robot to the desired direction. The difference between the target and the current direction is calculated and then 4 is added before doing the %4 to ensure that the difference is non-negative and then the answer determines the number of right turns the robot needs to make. 1 means one right turn. 2 means two right turns and three means 3 right turns (or 1 left turn) 

### `canMoveForward(struct robot *r)`
Checks if the robot can move forward without hitting a wall or obstacle.

### `moveToTarget(struct robot *r, int targetX, int targetY)`
Moves the robot to the specified target coordinates using a breadth-first search algorithm to find the shortest path. The function:
1. Initializes a queue and a visited array to keep track of visited cells.
2. Enqueues the robot's current position and marks it as visited.
3. Dequeues positions from the queue, checking if they match the target coordinates.
4. If the target is found, traces back the path from the target to the robot's starting position, one cell at a time by first turning to the desired direction and then moving forward in that direction
5. If it encounters markers on its way to the target marker, it picks them as well.

### `findAllMarkers(struct robot *r)`
Continuously finds and moves to the nearest marker until all markers are collected. The function:
1. Calculates the distance to each marker from the robot's current position.
2. Moves the robot to the nearest marker using `moveToTarget`.
3. Repeats the process until all markers are collected.

### `main()`
The main function initializes the arena, robot, markers, and obstacles, and then starts the robot's journey to collect all markers and return home.

## Variables
The  minimum and maximum arena width and height are in multiples of 40 because the cell size is 40 so that a whole number of cells fit inside the arena. The arena 2d array is of a sufficient enough size to suffice even the largest possible arena. the robot size to cell size is in the ratio 4 : 5 so that the robot fits neatly in the centre of the grid cell. A wait time of 100 ensures a smooth movement of the robot making it look like an animation.

## Colour Representation
The outer wall and obstacles are of a dark orange colour placed on a very light orange arena background, The green colored boxes are the markers and the blue hollow square is the home cell. The robot is the blue triangle.

## Compile and Run Commands
### Compile

gcc -o "executable_file_name" maincode.c graphics.c

### Run

./"executable_file_name" "direction" | java -jar drawapp-4.0.jar

where the "executable_file_name" is any name that you want the executable file to be called. It has to be written without the quotation marks and shoould be the same for both the run and the compile.
the "direction" is a command line argument char from "N, E, W, S" to tell which direction the robot spawns in initially. It has to be written as a single letter in all caps without quotation marks.
