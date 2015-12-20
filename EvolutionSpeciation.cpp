#include "EvolutionSpeciation.h"

#include <vector>
#include <stdlib.h>
#include <iostream>
#include <algorithm>

#include "Creature.h"
#include "MazeRun.h"

using namespace std;

//Returns the final generation of creatures
EvolutionResults evolveSpeciation(int generations, int popSizeInitial, int lifespan, Map* map, int compareMode, bool useMutation){
    vector<Creature*> population;
    vector<MazeRunResults> results;
    double* energyLeft;
    
    //Initialize the population
    MazeRunResults blankResult;
    for (int i = 0; i < popSizeInitial; i++) {
        population.push_back(generateCreature());
        results.push_back(blankResult);
    }
    
    for(int gen = 0; gen < generations; gen++){
        energyLeft = getEnergyAmount(map);
        int popSize = population.size();
        
        //Run all creatures through the maze
        for(int i = 0; i < popSize; i++){
            MazeRunResults creatureResult = runCreatureThroughMap(population.at(i), map, lifespan);
            results.at(i) = creatureResult;
        }
        
        //See how much food the creatures ate
        //Start at a random creature, so that all have an equal chance of eating
        int creatureIndex = rand() % popSize;
        for(int i = 0; i < popSize; i++){
            vector<int> foodEaten = results.at(creatureIndex).foodEaten;
            for(int foodID: foodEaten){
                double energyGain = std::min(energyLeft[foodID], 1.0);
                energyLeft[foodID] -= energyGain;
                population.at(creatureIndex)->energy += energyGain;
            }
            population.at(creatureIndex)->energy -= 1;
            creatureIndex = (creatureIndex + 1) % popSize;
        }
        
        //Remove any starving creatures
        //Start at the end so we don't skip over anything after removing a creature
        for(int i = popSize - 1; i >= 0; i--){
            if(population.at(i)->energy <= 0){
                deleteMazeResults(results.at(i));
                results.erase(results.begin() + i);
                delete population.at(i);
                population.erase(population.begin() + i);
            }
        }
        popSize = population.size();
        
        //Now breed the next population
        for(int i = 0; i < popSize; i++){
            Creature* dad = population.at(i);
            int mate = pickMateSp(i, results, popSize, compareMode);
            if(mate >= 0){
                Creature* mom = population.at(mate);
                population.push_back(crossCreatures(dad, mom));
                results.push_back(blankResult);
                if(useMutation){
                    mutateCreature(population.back());
                }
            }
        }
        
        delete[] energyLeft;
    }
    
    //Print some info for the current generation
    cout << "Final Generation: ";
    cout << population.size() << " - ";
    double sum = 0;
    for (int i = 0; i < population.size(); i++) {
        sum += results.at(i).foodEaten.size();
    }
    cout << sum / population.size() << " - ";
    //Then print the scores of the first 5 creatures
    for (int i = 0; i < std::min((int)results.size(), 5); i++) {
        std::cout << results.at(i).foodEaten.size() << " ";
    }
    std::cout << std::endl;
    
    EvolutionResults finalResults;
    finalResults.results = results;
    finalResults.population = population;
    finalResults.average = sum / population.size();
    
    //And now return the final generation
    return finalResults;
}

int pickMateSp(int creatureID, std::vector<MazeRunResults> results, int popSize, int compareMode){
    MazeRunResults creatureResults = results[creatureID];
    
    //Pick a potential mate
    int mate = rand() % popSize;
    int startMate = mate;
    //Make sure it's not trying to mate with itself
    while(mate == creatureID){
        mate = rand() % popSize;
    }
    
    int mateAttemptCount = 0;
    //See if the mate is good enough
    while(!goodMateSp(creatureResults, results[mate], compareMode)
            && ++mateAttemptCount < popSize){
        mate = (mate + 1) % popSize;
        //Stop trying to mate with itself. Again.
        if(mate == creatureID){
            mate = (mate + 1) % popSize;
        }
    }
    //If we've tried and failed to mate with everything
    if(mateAttemptCount == popSize){
        mate = -1;
    }
    return mate;
}

bool goodMateSp(MazeRunResults chooserResults, MazeRunResults chosenResults, int compareMode){
    if (compareMode == FOOD_EATEN) {
        int maxSimilarity = max(chooserResults.foodEaten.size(), chosenResults.foodEaten.size());
        int sharedFood = 0;

        for (int i = 0; i < chooserResults.foodEaten.size(); i++) {
            int foodEaten = chooserResults.foodEaten.at(i);
            for (int j = 0; j < chosenResults.foodEaten.size(); j++) {
                if (chosenResults.foodEaten.at(j) == foodEaten) {
                    sharedFood++;
                }
            }
        }

        if(maxSimilarity > 0 && sharedFood / (double) maxSimilarity > SIMILARITY_THRESHOLD_F){
            return true;
        }
    }
    
    if(compareMode == TILES_VISITED){
        int sharedTiles = 0;
        for(int i = 0; i < chooserResults.mapSize; i++){
            if(chooserResults.tilesVisited[i] && chosenResults.tilesVisited[i]){
                sharedTiles++;
            }
        }
        
        if(sharedTiles / (double) chooserResults.mapSize >= SIMILARITY_THRESHOLD_T){
            return true;
        }
    }
}

void deleteEvolutionResults(EvolutionResults results){
    while(!results.population.empty()){
        delete results.population.back();
        results.population.pop_back();
    }
    
    for(MazeRunResults mazeResults: results.results){
        deleteMazeResults(mazeResults);
    }
}
