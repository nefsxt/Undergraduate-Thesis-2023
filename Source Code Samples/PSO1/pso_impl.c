#include "imports.h"
#include "helpers.h"
#include "pso.h"


//initialise swarm with random values and find the global best (min)
//performs linear search to find the min value in mainData.objVals and its index which shows which individual it belongs to
double* initSwarmAndGetBest(int swarmSize)
{
    //seed for rand()
    //srand(time(NULL));

    //allocate memory for the swarm made up of swarmSize particles (vectors with N components)
    double** swarm = (double**)malloc(swarmSize * sizeof(double*));
    for (int i = 0; i < swarmSize; i++) {
        swarm[i] = (double*)malloc(mainData.n * sizeof(double));
    }

    //allocate memory for the global MxN rankings matrix that will be initialised using the data in the file
    double** bestPositions = (double**)malloc(swarmSize * sizeof(double*));
    for (int i = 0; i < swarmSize; i++) {
        bestPositions[i] = (double*)malloc(mainData.n * sizeof(double));
    }

    int n = mainData.n;

    double particle[n];

    double* objVals = (double*)malloc(swarmSize * sizeof(double));

    double* minData = (double*)malloc(2 * sizeof(double));
    double min_val;
    int min_index;
    double tempRand;

    //int* ranked = NULL;
    //double** sm = NULL;

    double sm[mainData.n][mainData.n];
    int ranked[mainData.n];


    for (int i = 0; i < swarmSize; i++) {

        for (int j = 0; j < n; j++) {
            // particles are initialised to be in range [0,1]
            tempRand = (double)rand() / RAND_MAX;
            particle[j] = tempRand;
            swarm[i][j] = tempRand;
            bestPositions[i][j] = tempRand;
            //printf("%f ", swarm[i][j]);
            //printf("%f ", particle[j]);
        }
        //printf("\n");

        sortAndRank(particle, mainData.n,ranked);
        /*for (int j = 0; j < n; j++)
        {
            printf("%d ", ranked[j]);
        }
        printf("\n");
        */

        //get score matrix for that ranking and apply the objective function
        rankingToScoreMatrix(ranked, n,sm);

        /*for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                printf("  %f  ", sm[i][j]);
            }

        }
        printf("\n-----------------------------------------------\n");
        */

        //apply objective function
        double objVal = costFunction(mainData.n,sm);

        //printf("costVal=%f\n",objVal);

        //store in array
        objVals[i] = objVal;
        //printf("costVal=%f\n",costVals[i]);

        //if at first individual initialise values for linear search
        if (i == 0)
        {
            min_val = objVals[0];
            min_index = 0;
            //printf("objVal 0 =%f\n", objVals[i]);
        }

        //linear search comparison
        if (objVals[i] < min_val)
        {
            min_val = objVals[i];
            min_index = i;


        }
        //printf("objVal=%f\n", objVals[i]);


        //free(ranked);
        //freeDoubleMatrixSpace(sm, mainData.n);
    }

    //implementation global data setup
    psoInstanceData.objVals = objVals;
    psoInstanceData.swarm = swarm;
    psoInstanceData.bestPositions = bestPositions;

    //psoInstanceData.currentGlobalBest = min_val;
    //psoInstanceData.currentGlobalBestIdx = (int)min_index;

    minData[0] = min_val;
    minData[1] = (int)min_index;

    return minData;

}


//initialise velocities
void initVelocities(int swarmSize,double a)
{
    //seed for rand()
    //srand(time(NULL));

    //allocate memory for the global swarmSizexN velocities matrix
    double** velocities = (double**)malloc(swarmSize * sizeof(double*));
    for (int i = 0; i < swarmSize; i++) {
        velocities[i] = (double*)malloc(mainData.n * sizeof(double));
    }

    int n = mainData.n;
    double tempRand;

    double vmax = a * (1 - 0); // vmax = a so 0.25 OR 0.5 depending on input
    for (int i = 0; i < swarmSize; i++) {

        for (int j = 0; j < n; j++) {

            // velocities are initialised to be in range (0,1) -------------------------------->(MIGHT NEED TO BE CHANGED)
            //this should affect how PSO explores the search space

            //tempRand = (double)rand() / RAND_MAX;
            tempRand = (double)(-vmax + rand()*2*vmax); //velocity initialised at -a or anywhere in (-a , a] -> basically in [-a,a]
            velocities[i][j] = tempRand;
            //printf("%f ", velocities[i][j]);
            //printf("%f ", particle[j]);
        }
        //printf("\n");

    }

    psoInstanceData.velocities = velocities;

}


//finds neighbors within neighborhodd radius nb_r range and compares their obj func vals to find the best using lin.search and returns the corresponding particle index
// if nb_r==0 : gbest -> returns the global best
// if nb_r > 0: lbest -> returns neighborhood best
// current is the index of the currently processed node
int getNBbestIndex(int nb_r,int current,int swarmSize)
{
    double bestVal;
    double currVal;
    int bestIdx;
    int currIdx;

    if (nb_r > 0)
    {
        //start with the first neighbor to the current individual (initialiser)
        bestVal = psoInstanceData.objVals[(current) % swarmSize];
        bestIdx = (current) % swarmSize;

        //search through neighbors on the right
        for (int i = 1; i <= nb_r; i++)
        {
            currVal = psoInstanceData.objVals[(current + i) % swarmSize];
            currIdx= (current + i) % swarmSize;

            if (currVal < bestVal)
            {
                bestVal = currVal;
                bestIdx = currIdx;
            }
         }

        //search through neighbors on the left
         for (int i = 1; i <= nb_r; i++)
         {
             currVal = psoInstanceData.objVals[(current - i + swarmSize) % swarmSize];
             currIdx = (current - i + swarmSize) % swarmSize;

            if (currVal < bestVal)
            {
                bestVal = psoInstanceData.objVals[(current - i + swarmSize) % swarmSize];
                bestIdx = currIdx;
            }
         }

         return bestIdx;


    }
    else
    {
        return psoInstanceData.currentGlobalBestIdx;
    }

}

//PSO core loop -> performs pso operations on 1 generation
void pso_main(double chi, double c1, double c2, int nb_r, int swarmSize, double a, int bestSize, double currBestData[bestSize])
{
    //srand(time(NULL));

    double r1, r2;
    double new_vij, new_xij;
    double min_val, min_index;

    double upperVelBound = a;
    double lowerVelBound = (-1) * (a);

    //int* ranked = NULL;
    //double** sm = NULL;
    double sm[mainData.n][mainData.n];
    int ranked[mainData.n];
    //for each individual particle vector
    for(int i=0;i< swarmSize;i++)
    {
        //find neighbor index(lbest) or global best index (gbest)
        int gi = getNBbestIndex(nb_r, i,swarmSize);
        //printf("gi %d \n",gi);
        //get new x and v
        for (int j = 0; j < mainData.n; j++)
        {

            r1 = (double) rand() / RAND_MAX;
            //printf("r1 = %f \n",r1);

            r2 = (double) rand() / RAND_MAX;
            //printf("r2 = %f \n", r2);

            new_vij = chi *(psoInstanceData.velocities[i][j] + (c1 * r1 * (psoInstanceData.bestPositions[i][j] - psoInstanceData.swarm[i][j])) + (c2 * r2 * (psoInstanceData.bestPositions[gi][j] - psoInstanceData.swarm[i][j])));

            //velocity bound check and reset if necessary
            if (new_vij <= lowerVelBound) { new_vij = lowerVelBound; }
            else if (new_vij >= upperVelBound) {new_vij = upperVelBound; }

            new_xij = psoInstanceData.swarm[i][j] + new_vij;

            // position bound check and reset if necessary
            if (new_xij < 0) { new_xij = 0; }
            else if (new_xij > 1) { new_xij = 1; }


            psoInstanceData.velocities[i][j] = new_vij;
            psoInstanceData.swarm[i][j] = new_xij;


        }
        //printf("\n");

        //evaluate
        sortAndRank(psoInstanceData.swarm[i], mainData.n,ranked);
        rankingToScoreMatrix(ranked, mainData.n,sm);

        double f_new_xij = costFunction(mainData.n,sm);

        //choose best particle for bestPositions
        if (f_new_xij < psoInstanceData.objVals[i])
        {
            //replace particle in best positions

            for (int j = 0; j < mainData.n; j++)
            {
                psoInstanceData.bestPositions[i][j] = psoInstanceData.swarm[i][j];
            }

           psoInstanceData.objVals[i] = f_new_xij;
        }

        //psoInstanceData.objVals[i] = f_new_xij;

        //free(ranked);
        //freeDoubleMatrixSpace(sm, mainData.n);

        //linear search for min is done within this loop in an attempt to make the implementation faster
        // the comparison occurs after all replacements and function evaluations have occured

        //if at first individual initialize values for linear search
        if (i == 0)
        {
            min_val = psoInstanceData.objVals[0];
            min_index = 0;
        }

        //linear search comparison
        if (psoInstanceData.objVals[i] < min_val)
        {
            min_val = psoInstanceData.objVals[i];
            min_index = i;
        }
    }

    currBestData[0] = min_val;
    currBestData[1] = min_index;

     //double* minData = (double*)malloc(2 * sizeof(double));
    //minData[0] = min_val;
    //minData[1] = min_index;

    //return minData;

}

//PSO implementation
//input: pso params chi,c1,c2 nb_r and budget (number of gens it is allowed to evolve)
// nbp_r : neighborhood radius -> if nb_r == N: gbest else if 0 < nb_r < N : lbest
//verbose : 1 to print, 0 (or other) to not print
void pso(double chi, double c1, double c2, int nb_r, int budget,int verbose,int swarmSize,double a)
{
    //seed for rand()
    psoInstanceData.seed = time(NULL);
    srand(psoInstanceData.seed);

    //random generator warmup
    for (int i = 0; i < 100; i++) { rand(); }

    //set constant product for the cost function (weightSum()*(n*(n-1)\2))
    costFunctionDataSetup();


    //init swarm,velicities and best positions and get best value and particle index
    double* minData = initSwarmAndGetBest(swarmSize); //swarm and best positions initialised here + get min value and the corresponding particle index
    psoInstanceData.currentGlobalBest = minData[0];
    psoInstanceData.currentGlobalBestIdx = (int)minData[1];
    double prevBestDataVal = minData[0];

    initVelocities(swarmSize,a); //velocites initalised here

    psoInstanceData.lastHit = 0;

    if (verbose == 1)
    {
        printf("init min global val: %f \n", minData[0]);
        printf("init min global paritcle idx : %d \n", (int)minData[1]);
    }
    //run until generation limit is reached
    //double* currBestData = NULL;
    int bestSize = 2;
    double currBestData[bestSize];
    int gen=1; //start from 1 not 0 because lastHit receives a negative value if the min is found in the first gen
    int no_improvement = 0;
    psoInstanceData.restartCounter = 0;
    double restartIntervals[5] = { 0.05, 0.1, 0.2, 0.4, 0.8 };

    while(gen <= budget)
    {

        //restart check************************************************************************************************
        if (no_improvement == (restartIntervals[psoInstanceData.restartCounter] * budget))
        {
            minData = initSwarmAndGetBest(swarmSize);
            prevBestDataVal = minData[0];
            initVelocities(swarmSize, a);
            no_improvement = 0;
            psoInstanceData.restartCounter++;
        }
        //*************************************************************************************************************


        //run pso_main
        pso_main(chi,c1,c2,nb_r, swarmSize,a,bestSize,currBestData);


        if (currBestData[0] < psoInstanceData.currentGlobalBest)
        {
            psoInstanceData.currentGlobalBest = currBestData[0];
            psoInstanceData.currentGlobalBestIdx = (int)currBestData[1];
            psoInstanceData.lastHit = (gen-1)*swarmSize+ psoInstanceData.currentGlobalBestIdx;
            //printf("last hit: %d\n",psoInstanceData.lastHit);
        }




        if (verbose == 1)
        {
            printf("***\n");
            printf("current min gen val: %f \n", currBestData[0]);
            printf("current min gen paritcle idx : %d \n", (int)currBestData[1]);
            printf("gen = %d \n",gen);
        }

        //no improvement check ************************************************
        if (prevBestDataVal == currBestData[0])
        {
            no_improvement++;
        }
        else
        {
            no_improvement = 0;
        }
        //*********************************************************************



        prevBestDataVal = currBestData[0];
        gen++;
        //free(currBestData);
    }
    if (verbose == 1)
    {
        int medianRanking[mainData.n];
        sortAndRank(psoInstanceData.swarm[psoInstanceData.currentGlobalBestIdx], mainData.n,medianRanking);
        printf("***\n");
        printf("min global val: %f \n", psoInstanceData.currentGlobalBest);
        printf("min global paritcle idx : %d \n", psoInstanceData.currentGlobalBestIdx);

        printf("The currently found median ranking is : \n");

        for (int i = 0; i < mainData.n; i++)
        {
            printf(" %d", medianRanking[i]);
        }
        printf("\n");

        //free(medianRanking);
    }
    free(minData);
}
