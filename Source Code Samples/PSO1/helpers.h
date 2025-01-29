#ifndef HELPERS_H
#define HELPERS_H

void rankingToScoreMatrix(int* ranking, int size, double scoreMatrix[size][size]); // returns the score matrix of a given ranking 

double kemenyDistance(int size,double** matrixA, double matrixB[size][size]); // calculates and returns the kemeny distance between two matrices of size NxN

double tau_x(int size,double** matrixA, double matrixB[size][size]); // calculates and returns the tau_x corr. coefficient between two matrices of size NxN (Emond and Mason formula) 

double objectiveFunction(double** c, double** y); // calculates and returns the value of the objective function argmax Ó cij*yij for i,j = 1 to N where Y E S^n

double weightSum(); //computes the sum of all customer weights stored in mainData.customerWeights

void costFunctionDataSetup();// sets the constant product needed for the cost function that represents Ó wk*(n*(n-1)\2) from cost(s) = Ó wk*(n*(n-1)\2)*(1-tau_x(cij,sij)) for k= 1 to m 

double costFunction(int size,double s[size][size]); // calculates and returns the value of the cost function cost(s) = Ó wk*(n*(n-1)\2)*(1-tau_x(cij,sij)) for k= 1 to m 

//helper function for sortAndRankn -> used as the comparator function in qsort
int compare(const void* a, const void* b);

//sorts and ranks an input array -> produces the ranking that array represents
//ties are broken by the sort algorithm
void sortAndRank(double* rankArray, int n, int ranking[n]);

void readRankingsAndCreateCI(char* filename); // reads an input txt file containing rankings and sets the global struct field mainData.CI to the calculated combined input matrix CI
//this function also sets the global struct's mainData.n and mainData.m to the respective values of N (items) and M (customers)

void readWeights(char* filename);// reads an input txt file containing the customer weights and sets the global struct field mainData.customerWeights to the array of weights read

void freeDoubleMatrixSpace(double** matrix, int size); // frees up the space allocated for a matrix of doubles

void freeIntMatrixSpace(int** matrix, int size); // frees up the space allocated for a matrix of ints

void globalMainDataSetup(char* filenameW, char* filenameR); //sets up the MainData global struct

#endif