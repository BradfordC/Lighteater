#ifndef MAZE_H
#define	MAZE_H

#include <list>

#define NORTH 0
#define EAST  1
#define SOUTH 2
#define WEST  3

struct Food{
    int ID;
    int X;
    int Y;
};

struct Map{
    int width;
    int height;
    
    int startX;
    int startY;
    
    std::list<Food> foodList;
};

Map* generateMap(int width, int height, int foodCount);
void destroyMap(Map* myMap);
double* getEnergyAmount(Map* myMap);
void printMap(Map* myMap);
void printMap(int width, int height, std::list<Food> foodList, int x, int y, int direction);
void printFoodList(Map* myMap);

#endif	/* MAZE_H */

