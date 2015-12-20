#include <stdlib.h>
#include <iostream>

#include "Maze.h"

using namespace std;


//Creates a pointer to a map with given width and height
//0 is a blank spot, 1 is a wall, 2 is food, 9 is starting location
Map* generateMap(int width, int height, int foodCount) {    
    Map* myMap = new Map();
    
    int totalSpots = width * height;
    
    //Return if we don't think there's enough space for everything
    if(foodCount >= totalSpots){
        cout << "Too much food, not enough map" << endl;
        return myMap;
    }
    
    //For keeping track of what spots are still empty (index goes across row before moving on to the next)
    bool* spotTaken = new bool[totalSpots];
    for(int i = 0; i < totalSpots; i++){
        spotTaken[i] = false;
    }
    
    //Choose a place for the start
    int startPoint = rand() % totalSpots;
    spotTaken[startPoint] = true;
    
    //Now choose spots for the food
    for(int i = 0; i < foodCount; i++){
        //Pick a spot
        int foodPoint = rand() % totalSpots;
        //See if it's taken already
        while(spotTaken[foodPoint]){
            foodPoint = rand() % totalSpots;
        }
        //When we find an empty spot, take it
        spotTaken[foodPoint] = true;
        //Now that we've found an empty spot, add it to the list of food
        Food newFood;
        newFood.ID = i; //Food
        newFood.X = foodPoint % width; //Food
        newFood.Y = foodPoint / width; //Food
        myMap->foodList.push_back(newFood);
    }
    
    
    myMap->startX = startPoint % width;
    myMap->startY = startPoint / width;
    myMap->width = width;
    myMap->height = height;
    
    delete[] spotTaken;
    
    return myMap;
}

void destroyMap(Map* myMap){
    delete myMap;
}

double* getEnergyAmount(Map* myMap){
    int foodCount = myMap->foodList.size();
    double* energyAmount = new double[foodCount];
    for(int i = 0; i < foodCount; i++){
        energyAmount[i] = 5;
    }
    return energyAmount;
}

//Print an initial map
void printMap(Map* myMap){
    printMap(myMap->width, myMap->height, myMap->foodList, myMap->startX, myMap->startY, NORTH);
}

//Print a map when just given a list of food and the position of the mouse (and the size of the map- feature creep!)
void printMap(int width, int height, list<Food> foodList, int x, int y, int direction){
    char* mapArray = new char[width * height];
    //Fill the map with blanks
    for(int i = 0; i < (width * height); i++){
        mapArray[i] = '.';
    }
    //Add all of the foods
    for(Food food: foodList){
        mapArray[food.X + food.Y * width] = 'O';
    }
    //Figure out which direction to point
    char creatureIcon = '^';
    switch(direction){
        case NORTH: creatureIcon = '^'; break;
        case EAST:  creatureIcon = '>'; break;
        case SOUTH: creatureIcon = 'V'; break;
        case WEST:  creatureIcon = '<'; break;
    }
    mapArray[x + y * width] = creatureIcon;
    
    //Print the map
    for(int j = 0; j < height; j++){
        for(int i = 0; i < width; i++){
            cout << mapArray[i + j * width];
        }
        cout << endl;
    }
    
    delete[] mapArray;
}

//Print the location and ID of all food
void printFoodList(Map* myMap){    
    for(Food nextFood: myMap->foodList){
        cout << nextFood.ID << " ";
        cout << nextFood.X << " ";
        cout << nextFood.Y << " ";
        cout << endl;
    }
    
}