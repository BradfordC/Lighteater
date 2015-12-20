#include "EvolutionNovelty.h"

#include <vector>
#include <stdlib.h>
#include <iostream>

#include "Creature.h"
#include "MazeRun.h"

using namespace std;

EvolutionNoResults evolveNovelty(int generations, int popSize, int lifespan, Map* map, int compareMode, bool useMutation){
    vector<Creature*> population;
    vector<MazeRunResults> results;
    
    //Initialize the population
    MazeRunResults blankResult;
    for (int i = 0; i < popSize; i++) {
        population.push_back(generateCreature());
        results.push_back(blankResult);
    }
    
    for(int gen = 0; gen < generations; gen++){
        int popSize = population.size();
        
        //Run all creatures through the maze
        for(int i = 0; i < popSize; i++){
            MazeRunResults creatureResult = runCreatureThroughMap(population.at(i), map, lifespan);
            results.at(i) = creatureResult;
        }
        
        
        //See how many creatures ate certain food or visited certain tiles
        int* numFoodEaten = new int[map->foodList.size()]();
        int* numVisitedTile = new int[map->height*map->width]();
        for(MazeRunResults runResult: results){
            for(int foodEatenID: runResult.foodEaten){
                numFoodEaten[foodEatenID]++;
            }
            for(int i = 0; i < map->height*map->width; i++){
                if(runResult.tilesVisited[i]){
                    numVisitedTile[i]++;
                }
            }
            
        }
        //Now calculate the fitness of each creature
        for(int i = 0; i < popSize; i++){
            results.at(i).fitness = 0;
            if(compareMode == FOOD_EATEN_N){
                for(int foodEatenID: results.at(i).foodEaten){
                    results.at(i).fitness += 1.0 / (double) numFoodEaten[foodEatenID];
                }
            }
            if(compareMode == TILES_VISITED_N){
                for(int tile = 0; tile < map->height * map->width; tile++){
                    if(results.at(i).tilesVisited[tile]){
                        results.at(i).fitness += 1 / (double) numVisitedTile[tile];
                    }
                }
            }
        }
        
        //Now breed the next population (if this isn't the final round)
        if (gen < generations - 1) {
            for (int i = 0; i < popSize; i++) {
                Creature* dad = population.at(i);
                int mate = pickMateTourneyNo(i, results, popSize, 4, compareMode);
                Creature* mom = population.at(mate);
                Creature* newCreature = crossCreatures(dad, mom);
                if (useMutation) {
                    mutateCreature(newCreature);
                }
                delete population.at(i);
                population.at(i) = newCreature;
                deleteMazeResults(results.at(i));
                results.at(i) = blankResult;
            }
        }
        
        delete[] numFoodEaten;
        delete[] numVisitedTile;
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
    
    EvolutionNoResults finalResults;
    finalResults.results = results;
    finalResults.population = population;
    finalResults.average = sum / population.size();
    
    //And now return the final generation
    return finalResults;
    
}

int pickMateTourneyNo(int creatureID, std::vector<MazeRunResults> results, int popSize, int numComp, int compareType){
    vector<int> comp;
    
    //Prepare the tournament
    while(comp.size() < numComp){
        //Pick a potential mate
        int mate;
        bool validMate = false;
        while (!validMate) {
             mate = rand() % popSize;
             validMate = true;
            //Make sure it doesn't choose itself
            if(mate == creatureID) {
                validMate = false;
            }
            else{
                //Make sure it doesn't choose a creature already in the tournament
                for (int i = 0; i < comp.size(); i++) {
                    if(mate == comp.at(i)){
                        validMate = false;
                    }
                }
            }
        }
        comp.push_back(mate);
    }
    
    //Now, pick the best mate
    double bestScore = -1;
    int bestMate = -1;
    for(int mate: comp){
        if(results.at(mate).fitness > bestScore){
            bestScore = results.at(mate).fitness;
            bestMate = mate;
        }
    }
    
    return bestMate;
}

void deleteEvolutionNoResults(EvolutionNoResults results){
    while(!results.population.empty()){
        delete results.population.back();
        results.population.pop_back();
    }
    
    for(MazeRunResults mazeResults: results.results){
        deleteMazeResults(mazeResults);
    }
}
