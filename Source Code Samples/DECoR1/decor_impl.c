#include "imports.h"
#include "helpers.h"
#include "decor.h"

//initialise global rankings matrix with popSize random rankings of N items and get the min value of the cost function and its index
//performs linear search to find the min value in mainData.costVals and its index which shows which individual it belongs to
double* initPopulationAndGetBest(int popSize)
{
    //seed for rand()
    //srand(time(NULL));

    //allocate memory for the global popSize x N rankings matrix that will be initialised using the data in the file
    double** population = (double**)malloc(popSize * sizeof(double*));
    for (int i = 0; i < popSize; i++) {
        population[i] = (double*)malloc(mainData.n * sizeof(double));
    }

    int n = mainData.n;

    double ranking[n];

    double* costVals = (double*)malloc(popSize * sizeof(double));

    double* minData = (double*)malloc(2 * sizeof(double));
    double min_val;
    int min_index;
    double tempRand;

    double sm[mainData.n][mainData.n];
    int ranked[mainData.n];



    for (int i = 0; i < popSize; i++) {

        for (int j = 0; j < n; j++) {

            tempRand = (double)rand() / (double)RAND_MAX;   //[0,1]
            ranking[j] = tempRand;
            population[i][j] = tempRand;
            //printf("%f ", population[i][j]);
            //printf("%f ", ranking[j]);
        }
        //printf("\n");

        sortAndRank(ranking, mainData.n,ranked);
        /*for (int j = 0; j < n; j++)
        {
            printf("%d ", ranked[j]);
        }
        printf("\n");
        */

        //get score matrix for that ranking and apply the cost function
        rankingToScoreMatrix(ranked, n,sm);
        //double costVal = objectiveFunction(mainData.CI,sm);
        /*
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                printf("  %f  ", sm[i][j]);
            }

        }
        printf("\n-----------------------------------------------\n");
        */

        //apply cost function
        double costVal = costFunction(mainData.n,sm);

        //printf("costVal=%f\n",costVal);

        //store in array
        costVals[i] = costVal;
        //printf("costVal=%f\n",costVals[i]);

        //if at first individual initialise values for linear search
        if (i == 0)
        {
            min_val = costVals[0];
            min_index = 0;
            //printf("costVal=%f\n", costVals[i]);
        }

        //linear search comparison
        if (costVals[i] < min_val)
        {
            min_val = costVals[i];
            min_index = i;



        }

        //freeDoubleMatrixSpace(sm, mainData.n);
        //free(ranked);

    }
    //printf("------------------------------------------------------------------------------------------\n");

    decorInstanceData.costVals = costVals; //set the global struct's decorInstanceData.costVals to the costVals initialized here
    decorInstanceData.population = population; //set the global struct's decorInstanceData.population to the population initialized here

    //decorInstanceData.currentGlobalBest = min_val;
    //decorInstanceData.currentGlobalBestIdx = (int)min_index;

    minData[0] = min_val;
    minData[1] = (int)min_index;

    return minData;

}

//mutate operator selector function -> is used to select which mutation method/operator will be used in DECoR
// methodChoice = 1: rand_1_bin , 2: rand_2_bin, 3: best_1_bin, 4: best_2_bin
void mutationOperator(int methodChoice,double F, int popSize, int size, double mutant[size])
{
    if (methodChoice == 1) { mutate_Rand_1_bin(F,popSize,size,mutant); }
    else if (methodChoice == 2) { mutate_Rand_2_bin(F,popSize,size,mutant); }
    else if (methodChoice == 3) { mutate_Best_1_bin(F,popSize,size,mutant); }
    else if (methodChoice == 4) { mutate_Best_2_bin(F,popSize,size,mutant); }
    else {
        //default choice is mutate_Rand_1_bin
        mutate_Rand_1_bin(F,popSize,size,mutant);
    }

}


//reads 3 random rankings from the current population and turns them into rankings before performing a mutative operation
//follows DE/rand/1/bin  v=x_r1_G + F*(x_r2_G - x_r3_G)
void mutate_Rand_1_bin(double F,int popSize,int size,double mutant[size])
{

    int r1, r2, r3;


    // Generate three random numbers within the range [0 , (popSize-1)]
    while (1)
    {
        //srand(time(NULL));
        r1 = rand() % popSize ;

        //srand(time(NULL) + 1);
        r2 = rand() % popSize;

        //srand(time(NULL) + 2);
        r3 = rand() % popSize ;

        if (r1 != r2 && r2 != r3 && r1 != r3)
        {
            break; //exit loop, distinct random index values aquired
        }

    }

    //FOR DEBUGGING

    //printf("r1= %d\n",r1);
    //printf("r2=%d\n",r2);
    //printf("r3=%d\n",r3);
    //printf("\n");
    //double* mutant = (double*)malloc(mainData.n * sizeof(double));

    for (int i = 0; i < mainData.n; i++) {
        mutant[i] = decorInstanceData.population[r1][i] + F * (decorInstanceData.population[r2][i]- decorInstanceData.population[r3][i]);
    }


    //return mutant;

}


//reads 5 random rankings from the current population and turns them into rankings before performing a mutative operation
//follows DE/rand/2/bin  v=x_r1_G + F*(x_r2_G + x_r3_G-x_r4_G - x_r5_G)
void mutate_Rand_2_bin(double F,int popSize,int size, double mutant[size])
{

    int r1,r2,r3,r4,r5;


    // Generate three random numbers within the range [0 , (popSize-1)]
    while (1)
    {
        //srand(time(NULL));
        r1 = rand() % popSize;

        //srand(time(NULL) + 1);
        r2 = rand() % popSize;

        //srand(time(NULL) + 2);
        r3 = rand() % popSize;

        //srand(time(NULL) + 3);
        r4 = rand() % popSize;

        //srand(time(NULL) + 4);
        r5 = rand() % popSize;

        if (r1 != r2 && r1 != r3 && r1 != r4 && r1 != r5 &&
            r2 != r3 && r2 != r4 && r2 != r5 &&
            r3 != r4 && r3 != r5 &&
            r4 != r5)
        {
            break; //exit loop, distinct random index values aquired
        }

    }

    /*
    printf("r1= %d\n", r1);
    printf("r2=%d\n", r2);
    printf("r1= %d\n", r3);
    printf("r2=%d\n", r4);
    printf("r1= %d\n", r5);
    */

   //double* mutant = (double*)malloc(mainData.n * sizeof(double));

    for (int i = 0; i < mainData.n; i++) {

        mutant[i] = decorInstanceData.population[r1][i] + F * (decorInstanceData.population[r2][i] + decorInstanceData.population[r3][i] - decorInstanceData.population[r4][i] - decorInstanceData.population[r5][i]);

    }

    //return mutant;

}

//reads the current global best and 2 random rankings from the current population and turns them into rankings before performing a mutative operation
//follows DE/best/1/bin  v=x_best_G + F*(x_r2_G - x_r3)
void mutate_Best_1_bin(double F,int popSize,int size, double mutant[size])
{

    int r1, r2;

    // Generate three random numbers within the range [0 , (popSize-1)]
    while (1)
    {
        //srand(time(NULL));
        r1 = rand() % popSize;

        //srand(time(NULL) + 1);
        r2 = rand() % popSize;

        if (r1 != r2 )
        {
            break; //exit loop, distinct random index values aquired
        }

    }

    //FOR DEBUGGING
    /*
    printf("r1= %d\n",r1);
    printf("r2=%d\n",r2);
    */
    //double* mutant = (double*)malloc(mainData.n * sizeof(double));

    for (int i = 0; i < mainData.n; i++) {
        mutant[i] = decorInstanceData.population[decorInstanceData.currentGlobalBestIdx][i] + F * (decorInstanceData.population[r1][i] - decorInstanceData.population[r2][i]);

    }


    //return mutant;

}


//reads the current global best and 4 random rankings from the current population and turns them into rankings before performing a mutative operation
//follows DE/best/2/bin  v=x_best_G + F*(x_r1_G + x_r2_G-x_r3_G - x_r4_G)
void mutate_Best_2_bin(double F,int popSize,int size, double mutant[size])
{

    int r1, r2, r3, r4;

    // Generate three random numbers within the range [0 , (popSize-1)]
    while (1)
    {
        //srand(time(NULL));
        r1 = rand() % popSize;

        //srand(time(NULL) + 1);
        r2 = rand() % popSize;

        //srand(time(NULL) + 2);
        r3 = rand() % popSize;

        //srand(time(NULL) + 3);
        r4 = rand() % popSize;

        if (r1 != r2 && r1 != r3 && r1 != r4 &&
            r2 != r3 && r2 != r4 &&
            r3 != r4)
        {
            break; //exit loop, distinct random index values aquired
        }

    }

    //FOR DEBUGGING
    /*
    printf("r1= %d\n",r1);
    print f("r2=%d\n",r2);
    */
    //double* mutant = (double*)malloc(mainData.n * sizeof(double));

    for (int i = 0; i < mainData.n; i++) {
        mutant[i] = decorInstanceData.population[decorInstanceData.currentGlobalBestIdx][i] + F * (decorInstanceData.population[r1][i]+ decorInstanceData.population[r2][i] - decorInstanceData.population[r3][i] - decorInstanceData.population[r4][i]);

    }


    //return mutant;

}



//applies the crossover operator
void crossover(double* current_ranking,int size, double mutant[size], double CR, double crossovered[size])
{
    //double* crossovered = (double*)malloc(mainData.n * sizeof(double));

    for (int i = 0; i < mainData.n ; i++)
    {
        double r = (double) rand() / (RAND_MAX + 1.0);

        if (r > CR)
        {
            crossovered[i] = mutant[i];
        }
        else
        {
            crossovered[i] = current_ranking[i]*(1.0);
        }

    }

    //return crossovered;
}

void DECoR(double F, double CR, int budget,int verbose, int methodChoice,int popSize)
{
    //seed for rand()
    decorInstanceData.seed = time(NULL);
    srand(decorInstanceData.seed);

    //random generator warmup
    for (int i = 0; i < 100; i++) { rand(); }



    //set constant product for the cost function (weightSum()*(n*(n-1)\2))
    costFunctionDataSetup();

    //DIFFERENTIAL EVO INITIATE  --- --- --- --- --- --- --- --- --- --- --- ---

    //initialize population and
    //get best from starting population
    double* prevBestData = initPopulationAndGetBest(popSize);
    decorInstanceData.currentGlobalBest = prevBestData[0];
    decorInstanceData.currentGlobalBestIdx = (int)prevBestData[1];

    //double* currBestDataTemp;
    //double* currBestData = (double*)malloc(2 * sizeof(double));

    decorInstanceData.lastHit =0;

    if (verbose == 1)
    {

        printf(" init best value :%f \n", prevBestData[0]);
        printf(" init best index :%d \n", (int)prevBestData[1]);
        printf("--- --- ---\n");
    }

    //EVOLVE UNTIL STOP CONDITION IS MET  --- --- --- --- --- --- --- --- --- --- --- ---

    int bestSize = 2;
    double currBestData[bestSize];
    int gen = 1; //start from 1 not 0 because lastHit receives a negative value if the min is found in the first gen
    int no_improvement = 0;
    decorInstanceData.restartCounter = 0;
    double restartIntervals[5] = {0.05, 0.1, 0.2, 0.4, 0.8};


    while (gen<=budget)
    {
        //restart check************************************************************************************************
        if (no_improvement == (restartIntervals[decorInstanceData.restartCounter] * budget))
        {
            prevBestData = initPopulationAndGetBest(popSize);
            no_improvement = 0;
            decorInstanceData.restartCounter++;
        }
        //*************************************************************************************************************


        //evolve population and get best from this generation
        DECoR_Main(F, CR, methodChoice, popSize,bestSize, currBestData);

        //currBestDataTemp=DECoR_Main(F, CR, methodChoice,popSize);
        //free(currBestData);

        //currBestData = currBestDataTemp;

        if (currBestData[0] < decorInstanceData.currentGlobalBest)
        {
            decorInstanceData.currentGlobalBest = currBestData[0];
            decorInstanceData.currentGlobalBestIdx = (int)currBestData[1];
            decorInstanceData.lastHit = (gen-1)*popSize+ decorInstanceData.currentGlobalBestIdx; //num of functional operations executed until the current best is found
        }


        if (verbose==1)
        {
            printf("***\n");
            printf(" current  GEN best value :%f \n", currBestData[0]);
            printf(" current GEN best index :%d \n", (int)currBestData[1]);
            printf(" current gen :%d \n", gen);
        }
        /*
        printf("DEBUG COST PRINTS: \n");
        for (int j = 1; j < popSize; j++)
        {
            printf("costval = %f\n", decorInstanceData.costVals[j]);
        }
        printf("\n");
        */

        //no improvement check ************************************************
        if (prevBestData[0] == currBestData[0])
        {
            no_improvement++;
        }
        else
        {
            no_improvement = 0;
        }
        //*********************************************************************

        prevBestData[0] = currBestData[0];
        gen++;
    }
    printf("***\n");
    printf("last hit %d  \n", decorInstanceData.lastHit);
    printf("***\n");

    printf("***\n");
    printf("restarts %d \n", decorInstanceData.restartCounter);
    printf("***\n");

    if (verbose == 1)
    {
        int medianRanking[mainData.n];
        sortAndRank(decorInstanceData.population[decorInstanceData.currentGlobalBestIdx], mainData.n,medianRanking);

        printf("***\n");
        printf(" final  GLOBAL best value :%f \n", decorInstanceData.currentGlobalBest);
        printf(" final GLOBAL best index :%d \n", decorInstanceData.currentGlobalBestIdx);

        printf("The currently found median ranking is : \n");

        for (int i = 0; i < mainData.n; i++)
        {
            printf(" %d", medianRanking[i]);
        }
        printf("\n");
    }

    free(prevBestData);
    //free(currBestData);
    //free(medianRanking);

    if (verbose == 1)
    {
        printf("TOTAL GENS: %d \n", gen);
    }
}



void DECoR_Main(double F, double CR, int methodChoice,int popSize,int bestSize, double currBestData[bestSize])
{

    //double* minData = (double*)malloc(2 * sizeof(double));
    double min_val;
    int min_index;

    double evo[mainData.n];
    double evo2[mainData.n];
    int evo3[mainData.n];
    double evoScore[mainData.n][mainData.n];

    double* temp = (double*)malloc(mainData.n * sizeof(double));

    for (int i = 0; i < popSize; i++)
    {


        for (int j = 0; j < mainData.n; j++)
        {
            temp[j] = decorInstanceData.population[i][j];

            //printf(" %d", decorInstanceData.population[i][j]);

        }


        //mutate
        mutationOperator(F,methodChoice,popSize,mainData.n,evo);

        //crossover
        crossover(temp,mainData.n, evo, CR,evo2);

        //discretization
        sortAndRank(evo2, mainData.n,evo3);

        //selection (cost compare)
        rankingToScoreMatrix(evo3, mainData.n,evoScore);

        double evoCost = costFunction(mainData.n,evoScore);

        if ( evoCost <= decorInstanceData.costVals[i])
        {
            for (int j = 0; j < mainData.n; j++)
            {
                decorInstanceData.population[i][j]=evo2[j]; // update with evo2 which is after the crossover and before the individual is ranked for the cost function


            }
            // we have a cost value change for the current individual (i)
            decorInstanceData.costVals[i] = evoCost;
        }

        /*
        for (int j = 0; j < mainData.n; j++)
        {
            printf("%f ", decorInstanceData.population[i][j]);
        }
        printf("\n");
        */
        //if at first individual initialize values for linear search
        if (i == 0)
        {
            min_val = decorInstanceData.costVals[0];
            min_index = 0;
        }

        //linear search comparison
        if (decorInstanceData.costVals[i] < min_val)
        {
            min_val = decorInstanceData.costVals[i];
            min_index = i;
        }

        //free(temp);
        //free(evo);
        //free(evo2);
        //free(evo3);
        //freeDoubleMatrixSpace(evoScore, mainData.n);

    }
    free(temp);
    //printf("*********************************************************************************\n");
    //minData[0] = min_val;
    //minData[1] = min_index;

    currBestData[0] = min_val;
    currBestData[1] = min_index;

    /*
    printf("DEBUG COST PRINTS: \n");
    for (int j = 0; j < popSize; j++)
    {
        printf("costval = %f\n",decorInstanceData.costVals[j]);
    }
    printf("\n");
    */

    //return minData;

}
