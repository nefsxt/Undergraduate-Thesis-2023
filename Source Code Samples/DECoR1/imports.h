#ifndef IMPORTS_H
#define IMPORTS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>
#include <signal.h>
#include <math.h>

//this struct is meant to be a shared resource amongst various implementations -> it is only meant to be read after setup! 
//it is initalised once using globalMainDataSetup(char* filenameW, char* filenameR);
struct MainData
{
    double** CI;                 //holds the combined input CI
    double** rankingMatrix;     //holds the MxN matrix with the rankings each customer has given
    double* customerWeights;   // holds the customer weights
    double costProduct;       // the constant product needed for the cost function that represents Ó wk*(n*(n-1)\2) from cost(s) = Ó wk*(n*(n-1)\2)*(1-tau_x(cij,sij)) for k= 1 to m 
    int n;                   // number of ranked items 
    int m;                  //number of customers
};

//this struct corresponds to the data that is unique to a single instance of DECoR 
struct DecorInstanceData
{
    double** population;      //ranking matrix popSize x N
    double* costVals;           // holds the cost function values for each individual
    double currentGlobalBest;  // holds the global best found until the current generation
    int currentGlobalBestIdx; //holds the index of the currentGlobalBest
    int lastHit;             // holds the number of functional operations spent until a the best global best is found
    int restartCounter;     // holds the number of restarts 
    time_t seed;
};

//this struct corresponds to the data that is unique to a single instance of PSO
struct PsoInstanceData 
{ 
    double** swarm;                // swarmSizexN matrix for the swarm -> swarmSize particles where each particle is a vector with N components 
    double** velocities;          //swarmSizexN matrix for the velocity vectors of each particle
    double** bestPositions;      //swarmSizexN matrix for the best position vectors of each particle
    double* objVals;            // holds the objective function values for each particle
    double currentGlobalBest;  //holds the global best found until the current generation
    int currentGlobalBestIdx; //holds the index of the currentGlobalBest
    int lastHit;             // holds the number of functional operations spent until a the best global best is found
    int restartCounter;     // holds the number of restarts 
    time_t seed;

};


extern struct MainData mainData; //global Declaration of the struct that holds the data extracted from the input files for rankings and customer weights
extern struct DecorInstanceData  decorInstanceData; //global declaration of a struct that will hold the data for one DECoR instance

//extern struct PsoInstanceData  psoInstanceData; //global declaration of a struct that will hold the data for one PSO instance

#endif
