#include "EvolutionObjective.h"

#include <vector>
#include <stdlib.h>
#include <iostream>

#include "Creature.h"
#include "MazeRun.h"

using namespace std;

//Returns the final generation of creatures
//Currently does not return the "MazeResults" portion of "EvolutionObResults"
EvolutionObResults evolveObjective(int generations, int popSize, int lifespan, Map* map, bool useMutation){
    vector<Creature*> population[2];
    int* scores = new int[popSize];
    //Initialize the populations
    for(int gen = 0; gen < 2; gen++) {
        for (int i = 0; i < popSize; i++) {
            population[gen].push_back(generateCreature());
        }
    }
    
    for(int gen = 0; gen < generations; gen++){
        int currentPop = gen%2;
        int nextPop = 1 - currentPop;
        //Run all creatures through the maze
        for(int i = 0; i < popSize; i++){
            vector<int> foodEaten = runCreatureThroughMap(population[currentPop].at(i), map, lifespan).foodEaten;
            scores[i] = foodEaten.size();
        }
        //Now breed the next population
        for(int i = 0; i < popSize; i++){
            Creature* dad = population[currentPop].at(i);
            int mate = pickMateTourney(i, scores, popSize, 4);
            Creature* mom = population[currentPop].at(mate);
            delete population[nextPop].at(i);
            population[nextPop].at(i) = crossCreatures(dad, mom);
        }
        
        //Mutate (some) the creatures as well
        if (useMutation) {
            for (int i = 0; i < popSize; i++) {
                mutateCreature(population[nextPop].at(i));
            }
        }
    }
    
    //Print some info for the current generation
    cout << "Final Generation: ";
    double sum = 0;
    for (int i = 0; i < popSize; i++) {
        sum += scores[i];
    }
    cout << sum / popSize << " - ";
    //Then print the scores of the first 5 creatures
    for (int i = 0; i < 5; i++) {
        std::cout << scores[i] << " ";
    }
    std::cout << std::endl;
    
    //Clean up everything
    int oldGeneration = 1 - generations%2;
    while (!population[oldGeneration].empty()) {
        delete population[oldGeneration].back();
        population[oldGeneration].pop_back();
    }
    delete[] scores;
    
    //And now return the final results
    int finalGeneration = generations%2;
    
    EvolutionObResults finalResults;
    finalResults.population = population[finalGeneration];
    finalResults.average = sum / popSize;
    
    return finalResults;
}

//Tournament selection
int pickMateTourney(int creatureID, int* scores, int popSize, int numComp){
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
        if(scores[mate] > bestScore){
            bestScore = scores[mate];
            bestMate = mate;
        }
    }
    
    return bestMate;
}