/* 
 * File:   main.cpp
 * Author: Chris
 *
 * Created on August 19, 2015, 6:25 PM
 */

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <random>
#include <windows.h> 

#include "EvolutionObjective.h"
#include "EvolutionSpeciation.h"
#include "EvolutionNovelty.h"
#include "MazeRun.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    srand(time(0));
    
    ///*
    int numGenerations = 100;
    int numCreatures = 100;
    int lifespan = 100;
    
    Map* myMap = generateMap(20, 20, 20);
    
    double obAvSum = 0;
    double spfAvSum = 0;
    double sptAvSum = 0;
    double nofAvSum = 0;
    double notAvSum = 0;
    
    for(int i = 0; i < 100; i++){
        cout << i << endl;
        EvolutionObResults finalResultsOb = evolveObjective(numGenerations, numCreatures, lifespan, myMap, true);
        obAvSum += finalResultsOb.average;
        EvolutionResults finalResultsSpF = evolveSpeciation(numGenerations, numCreatures, lifespan, myMap, FOOD_EATEN, true);
        spfAvSum += finalResultsSpF.average;
        EvolutionResults finalResultsSpT = evolveSpeciation(numGenerations, numCreatures, lifespan, myMap, TILES_VISITED, true);
        sptAvSum += finalResultsSpT.average;
        EvolutionNoResults finalResultsNoF = evolveNovelty(numGenerations, numCreatures, lifespan, myMap, FOOD_EATEN, true);
        nofAvSum += finalResultsNoF.average;
        EvolutionNoResults finalResultsNoT = evolveNovelty(numGenerations, numCreatures, lifespan, myMap, TILES_VISITED, true);
        notAvSum += finalResultsNoT.average;
        
        deleteEvolutionResults(finalResultsSpF);
        deleteEvolutionResults(finalResultsSpT);
        deleteEvolutionNoResults(finalResultsNoF);
        deleteEvolutionNoResults(finalResultsNoT);
    }
    
    cout << "Objective Average:         " << obAvSum  / 100<< endl;
    cout << "Speciation (Food) Average: " << spfAvSum / 100<< endl;
    cout << "Speciation (Tile) Average: " << sptAvSum / 100<< endl;
    cout << "Novelty (Food) Average:    " << nofAvSum / 100<< endl;
    cout << "Novelty (Tile) Average:    " << notAvSum / 100<< endl;
   
}

