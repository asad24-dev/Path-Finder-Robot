#include "graphics.h"
#include <stdlib.h>  
#include <stdio.h>   
#include <string.h>  
#include <time.h>     

#define minArenaWidth 480   
#define maxArenaWidth 1000  
#define minArenaHeight 480  
#define maxArenaHeight 600
#define maxMarkers 15
#define maxObstacles 25
#define minMarkers 10
#define minObstacles 10
#define waitTime 100   
#define moveDistance 40
#define cellSize 40
#define robotSize 32

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

int arenaHeight;
int arenaWidth;
int arenaRows;
int arenaCols;
int allMarkersCollected = 0;
int arena[100][100]; // Ensure the size is sufficient
int homeX, homeY;
int numMarkers;
int numObstacles;

void InitializeHome(int robotX, int robotY) 
{
    do {
            homeX = ((rand() % (((arenaWidth - cellSize) - cellSize) / cellSize)) * cellSize) + cellSize;
            homeY = ((rand() % (((arenaHeight - cellSize) - cellSize) / cellSize)) * cellSize) + cellSize;
    } while ((homeX == robotX && homeY == robotY) || arena[homeY / cellSize][homeX / cellSize] != 0);

    setRGBColour(18, 30, 148);
    fillRect(homeX, homeY, cellSize, cellSize);
    setRGBColour(249, 184, 170);
    fillRect(homeX + 4 , homeY + 4, cellSize - 8, cellSize - 8);
}

struct robot {
    int x;
    int y;
    int direction;
};

void initializeArenaArray() {
    arenaRows = arenaHeight / cellSize;
    arenaCols = arenaWidth / cellSize;
    for (int i = 0; i < arenaRows; i++) {
        for (int j = 0; j < arenaCols; j++) {
            arena[i][j] = 0; // 0 represents free space
        }
    }
    // Set the border walls as (-1)
    for (int i = 0; i < arenaRows; i++) {
        arena[i][0] = -1;
        arena[i][arenaCols - 1] = -1;
    }
    for (int j = 0; j < arenaCols; j++) {
        arena[0][j] = -1;
        arena[arenaRows - 1][j] = -1;
    }
}

int GenerateRandomCoords(int min, int max) {
    return ((rand() % ((max - min) / cellSize)) * cellSize) + min;
}

void SetBackground() {
    setWindowSize(arenaWidth, arenaHeight);
    setRGBColour(173, 37, 8); 
    fillRect(0, 0, arenaWidth, arenaHeight);
    setRGBColour(249, 184, 170);
    fillRect(cellSize, cellSize, arenaWidth - 2 * cellSize, arenaHeight - 2 * cellSize);
    setColour(white);
    // Draw grid lines
    for (int x = cellSize; x <= arenaWidth - cellSize; x += cellSize) {
        drawLine(x, cellSize, x, arenaHeight - cellSize);
    }
    for (int y = cellSize; y <= arenaHeight - cellSize; y += cellSize) {
        drawLine(cellSize, y, arenaWidth - cellSize, y);
    }
}

void drawMarkers(int roboX, int roboY) {
    numMarkers = ((rand() % ((maxMarkers - minMarkers + 1) / 1)) * 1) + minMarkers;
    for (int i = 0; i < numMarkers; i++) {
        int markerX, markerY;
        do {
            markerX = GenerateRandomCoords(cellSize, arenaWidth - 2 * cellSize);
            markerY = GenerateRandomCoords(cellSize, arenaWidth - 2 * cellSize);
        } while ((markerX == roboX && markerY == roboY) || arena[markerY / cellSize][markerX / cellSize] != 0);
        arena[markerY / cellSize][markerX / cellSize] = 1;  
        setRGBColour(8, 139, 129);
        fillRect(markerX, markerY, cellSize, cellSize);
    }
}

void drawObstacles(int roboX, int roboY) {
    numObstacles = minObstacles + rand() % (maxObstacles - minObstacles + 1);
    for (int i = 0; i < numObstacles; i++) {
        int obstacleX, obstacleY;
        do {
            obstacleX = GenerateRandomCoords(cellSize, arenaWidth - 2 * cellSize);
            obstacleY = GenerateRandomCoords(cellSize, arenaHeight - 2 * cellSize);
        } while ((obstacleX == roboX && obstacleY == roboY) || arena[obstacleY / cellSize][obstacleX / cellSize] != 0);
        arena[obstacleY / cellSize][obstacleX / cellSize] = 2;
        setRGBColour(173, 37, 8);
        fillRect(obstacleX, obstacleY, cellSize, cellSize);
    }
}

void initializeRobot(struct robot *r, int startX, int startY, int direction) {
    r->x = startX;
    r->y = startY;
    r->direction = direction;
}

void drawRobot(struct robot *r) {
    int xPoints[3], yPoints[3];
    int centerX = r->x;
    int centerY = r->y;

    switch (r->direction) {
        case NORTH:
            xPoints[0] = centerX;
            yPoints[0] = centerY - robotSize / 2;
            xPoints[1] = centerX - robotSize / 2;
            yPoints[1] = centerY + robotSize / 2;
            xPoints[2] = centerX + robotSize / 2;
            yPoints[2] = centerY + robotSize / 2;
            break;
        case EAST:
            xPoints[0] = centerX + robotSize / 2;
            yPoints[0] = centerY;
            xPoints[1] = centerX - robotSize / 2;
            yPoints[1] = centerY - robotSize / 2;
            xPoints[2] = centerX - robotSize / 2;
            yPoints[2] = centerY + robotSize / 2;
            break;
        case SOUTH:
            xPoints[0] = centerX;
            yPoints[0] = centerY + robotSize / 2;
            xPoints[1] = centerX + robotSize / 2;
            yPoints[1] = centerY - robotSize / 2;
            xPoints[2] = centerX - robotSize / 2;
            yPoints[2] = centerY - robotSize / 2;
            break;
        case WEST:
            xPoints[0] = centerX - robotSize / 2;
            yPoints[0] = centerY;
            xPoints[1] = centerX + robotSize / 2;
            yPoints[1] = centerY - robotSize / 2;
            xPoints[2] = centerX + robotSize / 2;
            yPoints[2] = centerY + robotSize / 2;
            break;
    }
    setRGBColour(18, 30, 148);
    fillPolygon(3, xPoints, yPoints);
}

void eraseRobot(struct robot *r) {
    int x = r->x - robotSize / 2;
    int y = r->y - robotSize / 2;
    setRGBColour(249, 184, 170 );
    fillRect(x, y, robotSize, robotSize);
}

void pickUpMarker(struct robot *r, int x, int y) {
    setRGBColour(249, 184, 170);
    fillRect(x, y, cellSize + 1, cellSize + 1);
    //draw lines that might be erased when picking up marker
    setColour(white);
    drawLine(x, y, x, y + cellSize);
    drawLine(x + cellSize, y, x + cellSize, y + cellSize);
    drawLine(x, y, x + cellSize, y);
    drawLine(x, y + cellSize, x + cellSize, y + cellSize);
}

void turnLeft(struct robot *r) {
    eraseRobot(r);
    r->direction = (r->direction + 3) % 4;
    drawRobot(r);
    sleep(waitTime);
}

void turnRight(struct robot *r) {
    eraseRobot(r);
    r->direction = (r->direction + 1) % 4;
    drawRobot(r);
    sleep(waitTime);
}

void moveForward(struct robot *r) {
    eraseRobot(r);
    switch (r->direction) {
        case NORTH:
            r->y -= cellSize;
            break;
        case EAST:
            r->x += cellSize;
            break;
        case SOUTH:
            r->y += cellSize;
            break;
        case WEST:
            r->x -= cellSize;
            break;
    }
    drawRobot(r);
    sleep(waitTime);
}

void faceDirection(struct robot *r, int targetDirection) {
    int diff = (targetDirection - r->direction + 4) % 4;
    if (diff == 1) {
        turnRight(r);
    } else if (diff == 2) {
        turnRight(r);
        turnRight(r);
    } else if (diff == 3) {
        turnLeft(r);
    }
}

int canMoveForward(struct robot *r) {
    int nextX = r->x;
    int nextY = r->y;
    switch (r->direction) {
        case NORTH:
            nextY -= cellSize;
            break;
        case EAST:
            nextX += cellSize;
            break;
        case SOUTH:
            nextY += cellSize;
            break;
        case WEST:
            nextX -= cellSize;
            break;
    }
    int cellX = nextX / cellSize;
    int cellY = nextY / cellSize;
    if (nextX < 0 || nextX >= arenaWidth || nextY < 0 || nextY >= arenaHeight) {
        return 0;
    }
    if (arena[cellY][cellX] == -1 || arena[cellY][cellX] == 2) {
        return 0;
    }
    return 1;
}

void moveToTarget(struct robot *r, int targetX, int targetY) {

    // studied the structure and functionality of a particular breadth first search algorithm from the following link: "https://www.hackerearth.com/practice/algorithms/graphs/breadth-first-search/tutorial/"

    int directions[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}}; //North, East, South, West
    int visited[arenaRows][arenaCols];
    for (int i = 0; i < arenaRows; i++) {
        for (int j = 0; j < arenaCols; j++) {
            visited[i][j] = 0;
        }
    }
    
    struct position {
        int x, y;
        int prevX, prevY;
    };
    
    struct position queue[arenaRows * arenaCols];
    int head = 0, tail = 0;
    
    queue[tail++] = (struct position){r->x / cellSize, r->y / cellSize, -1, -1};
    visited[r->y / cellSize][r->x / cellSize] = 1;
    
    int found = 0;
    struct position end;
    
    while (head < tail) {
        struct position curr = queue[head++];
        
        if (curr.x == targetX / cellSize && curr.y == targetY / cellSize) {
            end = curr;
            found = 1;
            break;
        }
        
        for (int i = 0; i < 4; i++) {
            int nextX = curr.x + directions[i][0];
            int nextY = curr.y + directions[i][1];
            
            if (nextX >= 0 && nextX < arenaCols && nextY >= 0 && nextY < arenaRows && !visited[nextY][nextX] && arena[nextY][nextX] != 2 && arena[nextY][nextX] != -1) {
                visited[nextY][nextX] = 1;
                queue[tail++] = (struct position){nextX, nextY, curr.x, curr.y};
            }
        }
    }
    
    if (found) {
        int pathX[arenaRows * arenaCols];
        int pathY[arenaRows * arenaCols];
        int pathLength = 0;
        
        while (end.prevX != -1 && end.prevY != -1) {
            pathX[pathLength] = end.x;
            pathY[pathLength] = end.y;
            pathLength++;
            
            for (int i = 0; i < tail; i++) {
                if (queue[i].x == end.prevX && queue[i].y == end.prevY) {
                    end = queue[i];
                    break;
                }
            }
        }
        
        for (int i = pathLength - 1; i >= 0; i--) {
            int dx = pathX[i] - (r->x / cellSize);
            int dy = pathY[i] - (r->y / cellSize);
            
            if (dx == 1) {
                faceDirection(r, EAST);
            } else if (dx == -1) {
                faceDirection(r, WEST);
            } else if (dy == 1) {
                faceDirection(r, SOUTH);
            } else if (dy == -1) {
                faceDirection(r, NORTH);
            }
            
            moveForward(r);
            
            int cellX = r->x / cellSize;
            int cellY = r->y / cellSize;
            
            if (arena[cellY][cellX] == 1) {
                pickUpMarker(r, cellX * cellSize, cellY * cellSize);
                arena[cellY][cellX] = 0;
                numMarkers--;
                if (numMarkers == 0) {
                    allMarkersCollected = 1;
                    break;
                }
            }
        }
    } 
}

void findAllMarkers(struct robot *r) {
    while (numMarkers > 0) {
        int minDistance = -1;
        int targetX = -1, targetY = -1;
        for (int i = 0; i < arenaRows; i++) {
            for (int j = 0; j < arenaCols; j++) {
                if (arena[i][j] == 1) {
                    int dist = abs(r->x / cellSize - j) + abs(r->y / cellSize - i);
                    if (minDistance == -1 || dist < minDistance) {
                        minDistance = dist;
                        targetX = j * cellSize;
                        targetY = i * cellSize;
                    }
                }
            }
        }
        if (targetX != -1 && targetY != -1) {
            moveToTarget(r, targetX, targetY);
            if (allMarkersCollected) {
                break; 
            }
        } else {
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    int direction;
    if (argc > 1) {
        switch (argv[1][0]) {
            case 'N':
                direction = 0; // Define constant for NORTH
                break;
            case 'E':
                direction = 1; // Define constant for EAST
                break;
            case 'S':
                direction = 2; // Define constant for SOUTH
                break;
            case 'W':
                direction = 3; // Define constant for WEST
                break;
            default:
                direction = 0; // Default to NORTH
                break;
        }
    } 
    else 
    {
        direction = 0;  // Default direction if no argument is provided
    }
    srand(time(NULL));
    arenaWidth = GenerateRandomCoords(minArenaWidth, maxArenaWidth);
    arenaHeight = GenerateRandomCoords(minArenaHeight, maxArenaHeight);
    initializeArenaArray();
    SetBackground();
    int roboStartX = GenerateRandomCoords(cellSize * 2, arenaWidth - cellSize * 2);
    int roboStartY = GenerateRandomCoords(cellSize * 2, arenaHeight - cellSize * 2);
    struct robot r1;
    initializeRobot(&r1, roboStartX + cellSize / 2, roboStartY + cellSize / 2, direction);
    drawRobot(&r1);
    drawMarkers(roboStartX, roboStartY);
    drawObstacles(roboStartX, roboStartY);
    InitializeHome(roboStartX, roboStartY);
    findAllMarkers(&r1);
    moveToTarget(&r1, homeX, homeY);
    return 0;
}
