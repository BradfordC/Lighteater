#include "MazeRun.h"

#include <cmath>
#include <windows.h> 
#include <cstring>
#include <iostream>

using namespace std;

//Returns a vector of the food ID in the order they had been eaten
MazeRunResults runCreatureThroughMap(Creature* myCreature, Map* myMap, int lifespan, bool showRun){
    int creatureX = myMap->startX;
    int creatureY = myMap->startY;
    int creatureDirection = NORTH; //What direction the creature is pointed. 0 is north, 1 is east, 2 is south, 3 is west
    
    list<Food> foodLeft = myMap->foodList;
    vector<int> foodEaten;
    
    int mapSize = myMap->width * myMap->height;
    bool* tilesVisited = new bool[mapSize];
    for(int i = 0; i < mapSize; i++){
        tilesVisited[i] = false;
    }
    
    MazeRunResults results;
    
    int foodRemaining = myMap->foodList.size();
    
    for(int i = 0; i < lifespan; i++){
        //Mark the tile the creature is at
        int currentLocation = creatureX + creatureY * myMap->width;
        tilesVisited[currentLocation] = true;
        
        //If we're showing the map, draw it now
        if(showRun){
            cout << string( 80, '\n' );
            cout << i << endl;
            printMap(myMap->width, myMap->height, foodLeft, creatureX, creatureY, creatureDirection);
            //Pause for a moment
            //Need to make this work for Linux as well
            Sleep(250);
        }
        
        //Figure out what food the mouse can see
        int numFoodAhead = 0;
        int numFoodLeft = 0;
        int numFoodRight = 0;
        int foodDirectlyAhead = 0;
        
        //See where the food is
        for(Food food: foodLeft){
            switch(creatureDirection){
                case NORTH:
                    //Make sure the food is ahead of the creature
                    if(food.Y < creatureY){
                        //Next, see if it's in a cone ahead of the creature
                        if((creatureY - food.Y) >= std::abs(creatureX - food.X)){
                            numFoodAhead++;
                            //See if it's directly ahead
                            if(creatureX == food.X){
                                foodDirectlyAhead = 1;
                            }
                        }
                        //If it's not ahead, see which side it's on
                        else{
                            if(creatureX > food.X){
                                numFoodLeft++;
                            }
                            else{
                                numFoodRight++;
                            }
                        }
                    }
                    break;
                case EAST:
                    //Make sure the food is ahead of the creature
                    if(food.X > creatureX){
                        //Next, see if it's in a cone ahead of the creature
                        if((food.X - creatureX) >= std::abs(creatureY - food.Y)){
                            numFoodAhead++;
                            //See if it's directly ahead
                            if(creatureY == food.Y){
                                foodDirectlyAhead = 1;
                            }
                        }
                        //If it's not ahead, see which side it's on
                        else{
                            if(creatureY > food.Y){
                                numFoodLeft++;
                            }
                            else{
                                numFoodRight++;
                            }
                        }
                    }
                    break;
                case SOUTH:
                    //Make sure the food is ahead of the creature
                    if(food.Y > creatureY){
                        //Next, see if it's in a cone ahead of the creature
                        if((food.Y - creatureY) >= std::abs(creatureX - food.X)){
                            numFoodAhead++;
                            //See if it's directly ahead
                            if(creatureX == food.X){
                                foodDirectlyAhead = 1;
                            }
                        }
                        //If it's not ahead, see which side it's on
                        else{
                            if(creatureX > food.X){
                                numFoodRight++;
                            }
                            else{
                                numFoodLeft++;
                            }
                        }
                    }
                    break;
                case WEST:
                    //Make sure the food is ahead of the creature
                    if(food.X < creatureX){
                        //Next, see if it's in a cone ahead of the creature
                        if((creatureX - food.X) >= std::abs(creatureY - food.Y)){
                            numFoodAhead++;
                            //See if it's directly ahead
                            if(creatureY == food.Y){
                                foodDirectlyAhead = 1;
                            }
                        }
                        //If it's not ahead, see which side it's on
                        else{
                            if(creatureY > food.Y){
                                numFoodRight++;
                            }
                            else{
                                numFoodLeft++;
                            }
                        }
                    }
                    break;
            }
        }
        
        //Decide where to move
        double* creatureOutputs = feedForward(myCreature, numFoodAhead, numFoodLeft, numFoodRight, foodDirectlyAhead, foodRemaining, true);
        int chosenMove = OUTPUT_MOVE;
        for(int i = 0; i < NUM_OUTPUT_NODES; i++){
            if(creatureOutputs[i] != 0.0){
                chosenMove = i;
            }
        }
        delete[] creatureOutputs;
        
        //Update the environment (move if possible, remove the food, etc.)
        bool moveEastWest = false;
        int dX = 0; 
        int dY = 0;
        switch(chosenMove){
            case OUTPUT_MOVE:
                moveEastWest = creatureDirection % 2;
                dX = moveEastWest ? 2 - creatureDirection : 0;
                dY = moveEastWest ? 0 : creatureDirection - 1;
                //If it's a valid move, take it
                if(creatureX + dX >= 0
                        && creatureX + dX < myMap->width
                        && creatureY + dY >= 0
                        && creatureY + dY < myMap->height){
                    creatureX += dX;
                    creatureY += dY;
                }
                break;
            case OUTPUT_LEFT:
                creatureDirection = (creatureDirection + 3) % 4;
                break;
            case OUTPUT_RIGHT:
                creatureDirection = (creatureDirection + 1) % 4;
                break;
        }
        
       //Check to see if the creature is on the same spot as food.  If it is, remove the food
        bool foodWasEaten = false;
        std::list<Food>::iterator foodIterator = foodLeft.begin();
        while (foodIterator != foodLeft.end()) {
            Food food = (*foodIterator);
            if (food.X == creatureX && food.Y == creatureY) {
                foodWasEaten = true;
                foodRemaining--;
                foodEaten.push_back(food.ID);
                foodIterator = foodLeft.erase(foodIterator);
                break;
            } 
            else {
                ++foodIterator;
            }
        }
        //TODO: We shouldn't include enough steps to collect everything, but if we do we should stop moving
    }
    
    results.foodEaten = foodEaten;
    results.tilesVisited = tilesVisited;
    results.mapSize = mapSize;
    
    return results;
}

MazeRunResults runCreatureThroughMap(Creature* myCreature, Map* myMap, int lifespan){
    return runCreatureThroughMap(myCreature, myMap, lifespan, false);
}

//Assumes the map is a square
void printVisited(MazeRunResults results){
    cout << endl << endl;
    int squareSize = std::sqrt(results.mapSize);
    for(int i = 0; i < squareSize; i++){
        for(int j = 0; j < squareSize; j++){
            cout << ((results.tilesVisited[j + squareSize * i])?"X":".");
        }
        cout << endl;
    }
}

void deleteMazeResults(MazeRunResults results){
    delete[] results.tilesVisited;
}