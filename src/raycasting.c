#include "raycasting.h"

const int map[8][8] = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 2},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1}
};

const int mapWidth = 8, mapHeight = 8;
const int displayWidth = 640, displayHeight = 480;
const int fov = 70;

double toRadians(double degrees) {
    return (degrees / 180.0f) * (double) M_PI;
}

double toDegrees(double radians) {
    return (radians / ((double) M_PI)) * 180.0f;
}

double min(double first, double second) {
    return (first < second) ? first : second;
}

double max(double first, double second) {
    return (first > second) ? first : second;
}

bool mapHasBlock(unsigned int x, unsigned int y) {
    return map[y][x] == 1;
}

bool mapHasGoal(unsigned int x, unsigned int y) {
    return map[y][x] == 2;
}

double squareRoot(double value) {
    float casted = (float) value; // Fel värde
    float root = sqrtf(casted); // sqrtf hade fungerat men det går inte att casta från double till float
    double castedBack = (double) root;
    return castedBack;
}

double checkHorizontalCollision(double playerX, double playerY, double xIncreasePerY, int dir) {
	int start = (dir == 1) ? (int) (playerY + 1) : (int) playerY;
	for(int yIndex = start; yIndex < mapHeight && yIndex >= 0; yIndex += dir) {

	    double yIncrease = (double) (yIndex - playerY);
	    double xIncrease = xIncreasePerY * yIncrease;
	    double x = playerX + xIncrease;

        double a2 = 1.0;
        double b2 = 0.177084476f;
        double c2 = a2 * b2;
        printf("%.6f", c2);

	    if((dir == 1 && mapHasBlock((int) x, yIndex)) || (dir == -1 && mapHasBlock((int) x, yIndex - 1))) {
            // Not the goal.
            double valueToSquareRoot = yIncrease * yIncrease + xIncrease * xIncrease;
            double root = squareRoot(valueToSquareRoot);
		    return squareRoot(valueToSquareRoot);
	    } else if ((dir == 1 && mapHasGoal((int) x, yIndex)) || (dir == -1 && mapHasGoal((int) x, yIndex - 1))) {
            // The goal.
		    return squareRoot(yIncrease * yIncrease + xIncrease * xIncrease);
	    }
    }

    return 20;
}

double checkVerticalCollision(double playerX, double playerY, double yIncreasePerX, int dir) {
	int start = (dir == 1) ? (int) (playerX + 1) : (int) playerX;
	for(int xIndex = start; xIndex < mapWidth && xIndex >= 0; xIndex += dir) {

	    double xIncrease = xIndex - playerX;
	    double yIncrease = yIncreasePerX * xIncrease;
	    double y = playerY + yIncrease;

	    if((dir == 1 && mapHasBlock(xIndex, (int) y)) || (dir == -1 && mapHasBlock(xIndex - 1, (int) y))) {
            // Not the goal.
		    return sqrtf(yIncrease * yIncrease + xIncrease * xIncrease);
	    } else if ((dir == 1 && mapHasGoal(xIndex, (int) y)) || (dir == -1 && mapHasGoal(xIndex - 1, (int) y))) {
            // The goal.
		    return sqrtf(yIncrease * yIncrease + xIncrease * xIncrease);
	    }
    }

    return 20;
}

double castRay(double playerX, double playerY, double rayDirection) {
		unsigned int zoneIndex = (int) (rayDirection / 90.0f);

		double xIncreasePerY = tanf(toRadians(rayDirection));
		double yIncreasePerX = 1 / tanf(toRadians(rayDirection));
		
		int dirX, dirY;
		if(zoneIndex == 0 || zoneIndex == 3) {
			dirY = 1;
		} else {
			dirY = -1;
		}
		if(zoneIndex == 0 || zoneIndex == 1) {
			dirX = 1;
		} else {
			dirX = -1;
		}

    double horizontalCollisionDistance = checkHorizontalCollision(playerX, playerY, xIncreasePerY, dirY);
    double verticalCollisionDistance = checkVerticalCollision(playerX, playerY, yIncreasePerX, dirX);
    return min(horizontalCollisionDistance, verticalCollisionDistance);
}

double* castRays(double playerX, double playerY, double playerDirection) {
    double* distances = (double*) malloc(sizeof(double) * 5);

    unsigned int amountOfRays = displayWidth;
    double cameraDistanceToScreen = (double) ((displayWidth / 2) / tan((toRadians(fov / 2))));
    
    for (unsigned int ray = 0; ray < amountOfRays; ray++) {
        double firstDistance = distances[0];

        double screenCollisionOffset = -(((double) amountOfRays - 1) / 2) + ray;
        double angleFromCamera = toDegrees(atan(screenCollisionOffset / cameraDistanceToScreen));
        double rayAngle = playerDirection + angleFromCamera; // In degrees
        while (rayAngle < 0) {
            rayAngle += 360;
        }
        while (rayAngle > 0) {
            rayAngle -= 360;
        }

        double distance = castRay(playerX, playerY, rayAngle);
        distance *= cos(toRadians(max(angleFromCamera, -angleFromCamera)));
        distances[ray] = distance;
    }

    return distances;
}