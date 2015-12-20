#ifndef EVOLUTIONSPECIATION_H
#define	EVOLUTIONSPECIATION_H

#include "MazeRun.h"
#include "Creature.h"

#include <vector>

#define FOOD_EATEN 1
#define TILES_VISITED 2

#define SIMILARITY_THRESHOLD_F .15
#define SIMILARITY_THRESHOLD_T .10

struct EvolutionResults{
    std::vector<Creature*> population;
    std::vector<MazeRunResults> results;
    double average;
};

EvolutionResults evolveSpeciation(int generations, int popSizeInitial, int lifespan, Map* map, int compareMode, bool useMutation);
int pickMateSp(int creatureID, std::vector<MazeRunResults> results, int popSize, int compareMode);
bool goodMateSp(MazeRunResults chooserResults, MazeRunResults chosenResults, int compareMode);
void deleteEvolutionResults(EvolutionResults results);

#endif	/* EVOLUTIONSPECIATION_H */

