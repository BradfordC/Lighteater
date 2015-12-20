#include "Creature.h"

#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <math.h>
#include <time.h>
#include <random>

#define MUTATION_CHANCE 1000

//For getting a random number from a normal distribution
std::random_device rd;
std::default_random_engine generator(rd());

double* feedForward(Creature* mouse, int numFoodAhead, int numFoodLeft, int numFoodRight,  int foodDirectlyAhead,
                    int foodRemaining, bool winnerTakeAll){
    double* output = new double[NUM_OUTPUT_NODES];
    
    double inputs[NUM_INPUT_NODES + 1] = {numFoodAhead, numFoodLeft, numFoodRight, foodDirectlyAhead, foodRemaining, 1.0};
    double hidden[NUM_HIDDEN_NODES + 1];
    //double output[NUM_OUTPUT_NODES];
    
    //Loop through each hidden node
    for(int h = 0; h < NUM_HIDDEN_NODES; h++){
        double nodeValue = 0;
        //Get the sum of all weights contributing to it
        for(int i = 0; i < NUM_INPUT_NODES + 1; i++){
            nodeValue += inputs[i] * mouse->w_ih[i + (NUM_INPUT_NODES + 1) * h];
        }
        hidden[h] = nodeValue;
    }    
    //Set the bias node
    hidden[NUM_HIDDEN_NODES] = 1;
    
    
    //Now get the values for the output nodes
    for(int o = 0; o < NUM_OUTPUT_NODES; o++){
        double nodeValue = 0;
        //Get the sum of all weights contributing to it
        for(int h = 0; h < NUM_HIDDEN_NODES + 1; h++){
            nodeValue += hidden[h] * mouse->w_ho[h + (NUM_HIDDEN_NODES + 1) * o];
        }
        output[o] = nodeValue;
    }
    
    //Make sure only 1 output has a value if we're using winner take all
    if(winnerTakeAll){
        int biggestOutputIndex = 0;
        double biggestOutput = output[0];
        
        //Find the biggest value
        for(int i = 1; i < NUM_OUTPUT_NODES; i++){
            if(output[i] > biggestOutput){
                //Reduce the loser to 0
                output[biggestOutputIndex] = 0;
                //Update the winner
                biggestOutput = output[i];
                biggestOutputIndex = i;
            }
            else{
                //Reduce the loser to 0
                output[i] = 0;
            }
        }
    }
    
    return output;
}

//Creates a creature (no, really?) with random weights
Creature* generateCreature(){
    std::normal_distribution<double> distribution_IH(0.0, 1 / sqrt(NUM_INPUT_NODES + 1));
    std::normal_distribution<double> distribution_HO(0.0, 1 / sqrt(NUM_HIDDEN_NODES + 1));
    
    Creature* mouse = new Creature();
    
    for(int i = 0; i < NUM_WEIGHTS_IH; i++){
        mouse->w_ih[i] = rand() / distribution_IH(generator);
    }
    for(int i = 0; i < NUM_WEIGHTS_HO; i++){
        mouse->w_ho[i] = rand() / distribution_HO(generator);
    }
    
    mouse->energy = 5.0;
    
    return mouse;
}


//Changes the weights of the creature
void mutateCreature(Creature* mouse){
    std::normal_distribution<double> distribution(0.0, .20);
    
    //Go through all weights between input and hidden layers, and mutate them randomly
    for(int i = 0; i < NUM_WEIGHTS_IH; i++){
        if(rand() % MUTATION_CHANCE == 0){
            mouse->w_ih[i] += distribution(generator);
        }
    }
    
    //Go through all weights between hidden and output layers, and mutate them randomly
    for(int i = 0; i < NUM_WEIGHTS_HO; i++){
        if(rand() % MUTATION_CHANCE == 0){
            mouse->w_ho[i] += distribution(generator);
        }
    }
    
    
}

//Creates a new creature by crossing traits of two parents
Creature* crossCreatures(Creature* dad, Creature* mom){
    Creature* child = new Creature();
    //First, choose what mutation we're going to use
    int mutationType = rand() % 3;
    int crossPoint = 0, crossPoint1 = 0, crossPoint2 = 1;
    switch(mutationType){
        //Single point crossover
        case 0:
            //We'll cross the input/hidden weights and the hidden/output weights separately
            //IH first, figure out where to cross
            crossPoint = (rand() % (NUM_WEIGHTS_IH - 1)) + 1;
            //Cross
            for(int i = 0; i < crossPoint; i++){
                child->w_ih[i] = dad->w_ih[i];
            }
            for(int i = crossPoint; i < NUM_WEIGHTS_IH; i++){
                child->w_ih[i] = mom->w_ih[i];
            }
            //Now HO, figure out where to cross
            crossPoint = rand() % NUM_WEIGHTS_HO;
            //Cross
            for(int i = 0; i < crossPoint; i++){
                child->w_ho[i] = dad->w_ho[i];
            }
            for(int i = crossPoint; i < NUM_WEIGHTS_HO; i++){
                child->w_ho[i] = mom->w_ho[i];
            }
            break;
        //Double point crossover
        case 1:
            //We'll cross the input/hidden weights and the hidden/output weights separately
            //IH first, figure out where to cross
            crossPoint1 = (rand() % (NUM_WEIGHTS_IH - 2)) + 1;
            crossPoint2 = crossPoint1 + 1 + (rand() % (NUM_WEIGHTS_IH - crossPoint1 - 1));
            //Cross
            for(int i = 0; i < crossPoint1; i++){
                child->w_ih[i] = dad->w_ih[i];
            }
            for(int i = crossPoint1; i < crossPoint2; i++){
                child->w_ih[i] = mom->w_ih[i];
            }
            for(int i = crossPoint2; i < NUM_WEIGHTS_IH; i++){
                child->w_ih[i] = dad->w_ih[i];
            }
            //Now HO, figure out where to cross
            crossPoint1 = (rand() % (NUM_WEIGHTS_HO - 2)) + 1;
            crossPoint2 = crossPoint1 + 1 + (rand() % (NUM_WEIGHTS_HO - crossPoint1 - 1));
            //Cross
            for(int i = 0; i < crossPoint1; i++){
                child->w_ho[i] = dad->w_ho[i];
            }
            for(int i = crossPoint1; i < crossPoint2; i++){
                child->w_ho[i] = mom->w_ho[i];
            }
            for(int i = crossPoint2; i < NUM_WEIGHTS_HO; i++){
                child->w_ho[i] = dad->w_ho[i];
            }
            break;
        //Uniform crossover
        case 2:
            for(int i = 0; i < NUM_WEIGHTS_IH; i++){
                child->w_ih[i] = (rand() % 2) ? dad->w_ih[i] : mom->w_ih[i];
            }
            for(int i = 0; i < NUM_WEIGHTS_HO; i++){
                child->w_ho[i] = (rand() % 2) ? dad->w_ho[i] : mom->w_ho[i];
            }
            break;
    }
    
    return child;
}

//Returns the similarity between two creatures
//0 is two identical creatures
double compareCreatures(Creature* male, Creature* female){
    //For now, the best easiest measure of similarity seems to be the sum of differences for each weight
    double difference = 0;
    
    for(int i = 0; i < NUM_WEIGHTS_IH; i++){
        difference += std::abs(male->w_ih[i] - female->w_ih[i]);
    }
    for(int i = 0; i < NUM_WEIGHTS_HO; i++){
        difference += std::abs(male->w_ho[i] - female->w_ho[i]);
    }
    
    return difference;
}