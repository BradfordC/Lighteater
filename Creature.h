/* 
 * File:   Creature.h
 * Author: Chris
 *
 * Created on August 20, 2015, 4:32 PM
 */

#ifndef CREATURE_H
#define	CREATURE_H


#define NUM_INPUT_NODES 5
#define NUM_HIDDEN_NODES 4
#define NUM_OUTPUT_NODES 3

#define INPUT_NUM_FOOD_AHEAD 0
#define INPUT_NUM_FOOD_LEFT 1
#define INPUT_NUM_FOOD_RIGHT 2
#define INPUT_FOOD_DIRECTLY_AHEAD 3
#define INPUT_FOOD_REMAINING 4

#define OUTPUT_MOVE 0
#define OUTPUT_LEFT 1
#define OUTPUT_RIGHT 2

#define NUM_WEIGHTS_IH ((NUM_INPUT_NODES + 1) * NUM_HIDDEN_NODES) //+1 for a bias node
#define NUM_WEIGHTS_HO ((NUM_HIDDEN_NODES + 1) * NUM_OUTPUT_NODES) //+1 for a bias node

//A creature stores all the weights for the neural network
struct Creature{
    //The weights of edges between input and hidden nodes
    double w_ih [NUM_WEIGHTS_IH]; 
    //The weights of edges between the hidden nodes and output nodes
    double w_ho [NUM_WEIGHTS_HO];
    //In both cases, the first "row" of values are the weights pointing toward the first receiving node
    //I.e. the first (NUM_INPUT_NODES + 1) values of w_ih are the weights from the inputs to the first hidden node
    double energy;
};

//Gets the output of the neural network for a given set of inputs
//Have to change the inputs manually if you add or remove input nodes
double* feedForward(Creature* mouse, int numFoodAhead, int numFoodLeft, int numFoodRight, int foodDirectlyAhead,
                    int foodRemaining, bool winnerTakeAll);

//Creates a creature with random weights
Creature* generateCreature();

//Changes the weights of the creature
void mutateCreature(Creature* mouse);

//Creates a new creature by crossing traits of two parents
Creature* crossCreatures(Creature* dad, Creature* mom);

//Returns the similarity between two creatures
//0 is two identical creatures
double compareCreatures(Creature* male, Creature* female);

#endif	/* CREATURE_H */

