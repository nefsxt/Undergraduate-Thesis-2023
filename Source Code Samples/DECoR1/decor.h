#ifndef DECOR_H
#define DECOR_H



//initialise global rankings matrix with popSize random rankings of N items and get the min value of the cost function and its index
//performs linear search to find the min value in mainData.costVals and its index which shows which individual it belongs to
double* initPopulationAndGetBest(int popSize);

//mutate operator selector function -> is used to select which mutation method/operator will be used in DECoR
// methodChoice = 1: rand_1_bin , 2: rand_2_bin, 3: best_1_bin, 4: best_2_bin
void mutationOperator(int methodChoice, double F,int popSize,int size, double mutant[size]);

//reads 3 random rankings from the current population and turns them into rankings before performing a mutative operation
//follows DE/rand/1/bin  v=x_r1_G + F*(x_r2_G - x_r3_G)
void mutate_Rand_1_bin(double F, int popSize, int size, double mutant[size]);

//reads 5 random rankings from the current population and turns them into rankings before performing a mutative operation
//follows DE/rand/2/bin  v=x_r1_G + F*(x_r2_G + x_r3_G-x_r4_G - x_r5_G)
void mutate_Rand_2_bin(double F,int popSize, int size, double mutant[size]);

//reads the current global best and 2 random rankings from the current population and turns them into rankings before performing a mutative operation
//follows DE/best/1/bin  v=x_best_G + F*(x_r2_G - x_r3)
void mutate_Best_1_bin(double F,int popSize, int size, double mutant[size]);

//reads the current global best and 4 random rankings from the current population and turns them into rankings before performing a mutative operation
//follows DE/best/2/bin  v=x_best_G + F*(x_r1_G + x_r2_G-x_r3_G - x_r4_G)
void mutate_Best_2_bin(double F,int popSize, int size, double mutant[size]);

//applies the crossover operator and returnes the crossed-over individual
void crossover(double* current_ranking, int size, double mutant[size], double CR,double crossovered[size]);

//DECoR implementation
void DECoR(double F, double CR, int budget,int verbose, int methodChoice,int popSize);
 
// DECoR core loop 
//returns the min value and index of the generation just processed
void DECoR_Main(double F, double CR, int methodChoice,int popSize,int bestSize, double currBestData[bestSize]);

#endif