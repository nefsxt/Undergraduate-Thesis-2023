#include "imports.h"
#include "helpers.h"    
#include "decor.h"

int main(int argc, char* argv[])
{
    int expID = atoi(argv[1]);
   
    int C = atoi(argv[2]);
    int N = atoi(argv[3]);
    int M = atoi(argv[4]);
    
    double F = atof(argv[5]); //0.55;
    double CR = atof(argv[6]); //0.85;
    int budget = atoi(argv[7]); //50;
    int verbose = atoi(argv[8]); //0; // 1 to print, 0 to not print
    int methodChoice = atoi(argv[9]); //1; //1: rand_1_bin , 2: rand_2_bin, 3: best_1_bin, 4: best_2_bin for mutation operator choice
    int popSize = atoi(argv[10]); //100;


    //get weights filename according to current working directory
    char filenameW[1024];
    char filenameWpart2[500];
    if (getcwd(filenameW, sizeof(filenameW)) != NULL) {
        snprintf(filenameWpart2, sizeof(filenameWpart2), "/testFiles/Dataset_Weights_C_%d_N_%d_M_%d.txt", C,N,M);
        strcat(filenameW, filenameWpart2);

        //printf("filenameW: %s\n", filenameW);
    }
    else {
        perror("getcwd() error");
    }

    //get rankings filename according to current working directory
    char filenameR[1024];
    char filenameRpart2[500];
    if (getcwd(filenameR, sizeof(filenameR)) != NULL) {
        snprintf(filenameRpart2, sizeof(filenameRpart2), "/testFiles/Dataset_Rankings_C_%d_N_%d_M_%d.txt", C, N, M);
        strcat(filenameR, filenameRpart2);
        
        //printf("filenameR: %s\n", filenameR);
    }
    else {
        perror("getcwd() error");
    }


    //-------------------------------------------------------------------------------------------------------------------------------
    //                 TEST BENCH 
    //-------------------------------------------------------------------------------------------------------------------------------

    globalMainDataSetup(filenameW,filenameR);

    clock_t start = clock(); //---------------TIME START---------------
    DECoR(F, CR, budget, verbose,methodChoice, popSize);
    clock_t end = clock();//------------------TIME END-----------------
    double runtime = (double)(end - start) / CLOCKS_PER_SEC;
    
    unsigned int seed = (unsigned int)decorInstanceData.seed;
    //printf("seed %u \n",seed);
    printf("runtime: %f\n", runtime);
    printf("best %f\n", decorInstanceData.currentGlobalBest);
    double results[7] = {expID,decorInstanceData.currentGlobalBest,(double)budget,(double)decorInstanceData.lastHit,(double)runtime,seed,(int)decorInstanceData.restartCounter};

    //--------------------------------------WRITE RESULTS TO OUTPUT FILES------------------------------------------------------------------------------

    //write out stats

    //cwd+ dirname + filename
    char filenameSTAT[1024];
    char dirCNM[100];
    char filenameSTATpart2[1024];
    struct stat dirStatus;
    if (getcwd(filenameSTAT, sizeof(filenameSTAT)) != NULL) {
        snprintf(dirCNM, sizeof(dirCNM), "/resultFiles/STATS_C_%d_N_%d_M_%d",C,N,M);
        strcat(filenameSTAT, dirCNM);
        int status = stat(filenameSTAT, &dirStatus);
        // Check if directory exists
        if (status == 0 && S_ISDIR(dirStatus.st_mode)) {
            //dir exists -> write
            snprintf(filenameSTATpart2, sizeof(filenameSTATpart2), "/STATS_C_%d_N_%d_M_%d_F_%.2f_CR_%.2f_B_%d_V_%d_Mth_%d_Pop_%d.txt", C, N, M, F, CR, budget, verbose, methodChoice, popSize);
            strcat(filenameSTAT, filenameSTATpart2);
        }
        else {
            //dir does not exist -> create and write
            int s1 = mkdir(filenameSTAT, 0700);  // 0700 sets read, write, and execute permissions for the owner
            if (s1 == 0) {
                printf("Directory created successfully!\n");
            }
            else {
                printf("Failed to create directory.\n");
                exit(EXIT_FAILURE);
            }
            snprintf(filenameSTATpart2, sizeof(filenameSTATpart2), "/STATS_C_%d_N_%d_M_%d_F_%.2f_CR_%.2f_B_%d_V_%d_Mth_%d_Pop_%d.txt", C, N, M, F, CR, budget, verbose, methodChoice, popSize);
            strcat(filenameSTAT, filenameSTATpart2);
        }


        //printf("filenameSTAT: %s\n", filenameSTAT);
    }
    else {
        perror("getcwd() error");
    }




    // Open the file in append mode
    FILE* file = fopen(filenameSTAT, "a");

    if (file == NULL) {
        printf("Failed to open the file.\n");
        printf("STAT\n");
        return 0;
    }

    // Append the values to the file separated by commas
    for (int i = 0; i < 7; i++) {
        if (i == 0) 
        { 
            fprintf(file, "%d", (int)results[i]); 
        }
        else 
        {
            fprintf(file, ",");
            if (i == 5) 
            {
                fprintf(file, "%d", (int)results[i]);
            }
            else 
            {
                fprintf(file, "%f", results[i]);
            }
            
        }
    }
    fprintf(file, "\n");

    // Close the file
    fclose(file);


    //write out ranking

    //cwd+ dirname + filename
    char filenameMR[1024];
    char dirCNM2[100];
    char filenameMRpart2[1024];
    struct stat dirStatus2;
    if (getcwd(filenameMR, sizeof(filenameMR)) != NULL) {
        snprintf(dirCNM2, sizeof(dirCNM2), "/resultFiles/MR_C_%d_N_%d_M_%d", C, N, M);
        strcat(filenameMR, dirCNM2);
        int status2 = stat(filenameMR, &dirStatus2);
        // Check if directory exists
        if (status2 == 0 && S_ISDIR(dirStatus2.st_mode)) {
            //dir exists -> write
            snprintf(filenameMRpart2, sizeof(filenameMRpart2), "/MR_C_%d_N_%d_M_%d_F_%.2f_CR_%.2f_B_%d_V_%d_Mth_%d_Pop_%d.txt", C, N, M, F, CR, budget, verbose, methodChoice, popSize);
            strcat(filenameMR, filenameMRpart2);
        }
        else {
            //dir does not exist -> create and write
            int s2 = mkdir(filenameMR, 0700);  // 0700 sets read, write, and execute permissions for the owner
            if (s2 == 0) {
                printf("Directory created successfully!\n");
            }
            else {
                printf("Failed to create directory.\n");
                exit(EXIT_FAILURE);
            }
            snprintf(filenameMRpart2, sizeof(filenameMRpart2), "/MR_C_%d_N_%d_M_%d_F_%.2f_CR_%.2f_B_%d_V_%d_Mth_%d_Pop_%d.txt", C, N, M, F, CR, budget, verbose, methodChoice, popSize);
            strcat(filenameMR, filenameMRpart2);
        }


        //printf("filenameMR: %s\n", filenameMR);
    }
    else {
        perror("getcwd() error");
    }


    //get the median ranking of the best found 
    double* individual = malloc(mainData.n * sizeof(double));
    
    // write the ranking to an output file, each number is separated by a space
    for (int i = 0; i < mainData.n; i++)
    {
        individual[i] = decorInstanceData.population[decorInstanceData.currentGlobalBestIdx][i];
        
    }
    //turn individual into ranking
    int medianRanking[mainData.n];
    sortAndRank(individual,mainData.n,medianRanking);

    // Open the file in append mode
    FILE* fp = fopen(filenameMR, "a");

    if (fp == NULL) {
        printf("Failed to open the file.\n");
        printf("MR\n");
        return 0;
    }


    for (int i = 0; i < mainData.n; i++)
    {
        if (i==0) 
        {
            fprintf(fp, "%d, ",expID);
        }
        else 
        {
            fprintf(fp, "%d ", medianRanking[i]);
        }
        
    }

    //separate each ranking on a new line
    fprintf(fp, "\n");

    // Close the file
    fclose(fp);

    free(individual);
    //free(medianRanking);
    //------------------------------------------FREE----------------------------------------------------------------------------------------------------
     
    //free rankingMatrix
    freeDoubleMatrixSpace(decorInstanceData.population, popSize);
    //free array of cost values
    free(decorInstanceData.costVals);
     
    //FREE ALLOCATED MEMORY FOR GLOBAL DATA ###########################################################################################

    //free array of customer weights

    free(mainData.customerWeights);

    //free combined input matrix CI

    freeDoubleMatrixSpace(mainData.CI, mainData.n);



    return 0;

}
