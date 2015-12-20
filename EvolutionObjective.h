/* 
 * File:   EvolutionObjective.h
 * Author: Chris
 *
 * Created on September 16, 2015, 9:30 AM
 */

#ifndef EVOLUTIONOBJECTIVE_H
#define	EVOLUTIONOBJECTIVE_H

#include "MazeRun.h"
#include "Creature.h"

#include <vector>

struct EvolutionObResults{
    std::vector<Creature*> population;
    std::vector<MazeRunResults> results;
    double average;
};

EvolutionObResults evolveObjective(int generations, int popSize, int lifespan, Map* map, bool useMutation);
int pickMateTourney(int creatureID, int* scores, int popSize, int numComp);
#endif	/* EVOLUTIONOBJECTIVE_H */

