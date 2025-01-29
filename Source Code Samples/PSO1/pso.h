#ifndef PSO_H
#define PSO_H

//initialise swarm with random values and find the global best (min)
//performs linear search to find the min value in mainData.objVals and its index which shows which individual it belongs to
double* initSwarmAndGetBest(int swarmSize);

//initialise velocities 
void initVelocities(int swarmSize,double a);

//finds neighbors within neighborhodd radius nb_r range and compares their obj func vals to find the best using lin.search and returns the corresponding particle index
// if nb_r==0 : gbest -> returns the global best 
// if nb_r > 0: lbest -> returns neighborhood best
// current is the index of the currently processed node
int getNBbestIndex(int nb_r, int current,int swarmSize);

//PSO core loop -> performs pso operations on 1 generation
void pso_main(double chi, double c1, double c2, int nb_r,int swarmSize,double a ,int bestSize, double currBestData[bestSize]);

//PSO implementation
//input: pso params chi,c1,c2 nb_r and budget (number of gens it is allowed to evolve) 
// nbp_r : neighborhood radius -> if nb_r == N: gbest else if 0 < nb_r < N : lbest 
//verbose : 1 to print, 0 (or other) to not print 
void pso(double chi, double c1, double c2, int nb_r, int budget,int verbose, int swarmSize, double a);

#endif