#ifndef MAZERUN_H
#define	MAZERUN_H

#include <vector>

#include "Maze.h"
#include "Creature.h"

struct MazeRunResults{
    std::vector<int> foodEaten;
    bool* tilesVisited;
    int mapSize;
    double fitness;
};

MazeRunResults runCreatureThroughMap(Creature* myCreature, Map* myMap, int steps);
MazeRunResults runCreatureThroughMap(Creature* myCreature, Map* myMap, int steps, bool showRun);
void printVisited(MazeRunResults results);
void deleteMazeResults(MazeRunResults results);

#endif	/* MAZERUN_H */

