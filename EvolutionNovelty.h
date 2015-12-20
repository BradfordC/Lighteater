/* 
 * File:   EvolutionNovelty.h
 * Author: Chris
 *
 * Created on December 19, 2015, 7:18 PM
 */

#ifndef EVOLUTIONNOVELTY_H
#define	EVOLUTIONNOVELTY_H

#include "MazeRun.h"
#include "Creature.h"

#include <vector>

#define FOOD_EATEN_N 1
#define TILES_VISITED_N 2

struct EvolutionNoResults{
    std::vector<Creature*> population;
    std::vector<MazeRunResults> results;
    double average;
};

EvolutionNoResults evolveNovelty(int generations, int popSize, int lifespan, Map* map, int compareMode, bool useMutation);
int pickMateTourneyNo(int creatureID, std::vector<MazeRunResults> results, int popSize, int numComp, int compareType);
void deleteEvolutionNoResults(EvolutionNoResults results);

#endif	/* EVOLUTIONNOVELTY_H */

