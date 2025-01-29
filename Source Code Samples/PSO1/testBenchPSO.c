#include "imports.h"
#include "helpers.h"    
#include "pso.h"

int main(int argc, char* argv[])
{
	int expID = atoi(argv[1]);

	int C = atoi(argv[2]);
	int N = atoi(argv[3]);
	int M = atoi(argv[4]);

	double chi = atof(argv[5]); //0.729; //default values used for chi,c1,c2
	double c1 = atof(argv[6]);//2.05;
	double c2 = atof(argv[7]);//2.05;
	int nb_r = atoi(argv[8]);//3;
	int budget = atoi(argv[9]);//250;
	int verbose = atoi(argv[10]);//0; // if value = 1 pso() prints data for each generation 
	int swarmSize = atoi(argv[11]);//100;
	double a = atof(argv[12]);//0.5;


	//get weights filename according to current working directory
	char filenameW[1024];
	char filenameWpart2[500];
	if (getcwd(filenameW, sizeof(filenameW)) != NULL) {
		snprintf(filenameWpart2, sizeof(filenameWpart2), "/testFiles/Dataset_Weights_C_%d_N_%d_M_%d.txt", C, N, M);
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


	//-------------------------------------RUN EXPERIMENT FUNCTION------------------------------------------------------------------------
	

	//set up global mainData
	globalMainDataSetup(filenameW, filenameR);

	clock_t start = clock(); //---------------TIME START---------------
	pso(chi, c1, c2, nb_r, budget, verbose, swarmSize, a);
	clock_t end = clock();//------------------TIME END-----------------
	double runtime = (double)(end - start) / CLOCKS_PER_SEC;

	unsigned int seed = (unsigned int)psoInstanceData.seed;

	//printf("cost product = %f \n", mainData.costProduct);
	printf("runtime: %f\n", runtime);
	printf("best %f\n", psoInstanceData.currentGlobalBest);
	double results[7] = { expID,psoInstanceData.currentGlobalBest,(double)budget,(double)psoInstanceData.lastHit,(double)runtime,seed,(int)psoInstanceData.restartCounter };

	//--------------------------------------WRITE RESULTS TO OUTPUT FILES------------------------------------------------------------------------------

	//write out stats
	//cwd+ dirname + filename
	char filenameSTAT[1024];
	char dirCNM[100];
	char filenameSTATpart2[1024];
	struct stat dirStatus;
	if (getcwd(filenameSTAT, sizeof(filenameSTAT)) != NULL) {
		snprintf(dirCNM, sizeof(dirCNM), "/resultFiles/STATS_C_%d_N_%d_M_%d", C, N, M);
		strcat(filenameSTAT, dirCNM);
		int status = stat(filenameSTAT, &dirStatus);
		// Check if directory exists
		if (status == 0 && S_ISDIR(dirStatus.st_mode)) {
			//dir exists -> write
			snprintf(filenameSTATpart2, sizeof(filenameSTATpart2), "/STATS_C_%d_N_%d_M_%d_chi_%.2f_c1_%.2f_c2_%.2f_NB_%d_B_%d_V_%d_Sw_%d_a_%.2f.txt", C, N, M, chi,c1,c2, nb_r, budget, verbose, swarmSize, a);
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
			snprintf(filenameSTATpart2, sizeof(filenameSTATpart2), "/STATS_C_%d_N_%d_M_%d_chi_%.2f_c1_%.2f_c2_%.2f_NB_%d_B_%d_V_%d_Sw_%d_a_%.2f.txt", C, N, M, chi, c1, c2, nb_r, budget, verbose, swarmSize, a);
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
			snprintf(filenameMRpart2, sizeof(filenameMRpart2), "/MR_C_%d_N_%d_M_%d_chi_%.2f_c1_%.2f_c2_%.2f_NB_%d_B_%d_V_%d_Sw_%d_a_%.2f.txt", C, N, M, chi, c1, c2, nb_r, budget, verbose, swarmSize, a);
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
			snprintf(filenameMRpart2, sizeof(filenameMRpart2), "/MR_C_%d_N_%d_M_%d_chi_%.2f_c1_%.2f_c2_%.2f_NB_%d_B_%d_V_%d_Sw_%d_a_%.2f.txt", C, N, M, chi, c1, c2, nb_r, budget, verbose, swarmSize, a);
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
		individual[i] = psoInstanceData.swarm[psoInstanceData.currentGlobalBestIdx][i];

	}
	//turn individual into ranking
	int medianRanking[mainData.n];
	sortAndRank(individual, mainData.n,medianRanking);

	// Open the file in append mode
	FILE* fp = fopen(filenameMR, "a");

	if (fp == NULL) {
		printf("Failed to open the file.\n");
		return 0;
	}


	for (int i = 0; i < mainData.n; i++)
	{
		if (i == 0)
		{
			fprintf(fp, "%d, ", expID);
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



	
	//--------------------------FREE ALLOCATED MEMORY------------------------------------------------------------------------------------
	freeDoubleMatrixSpace(psoInstanceData.swarm, swarmSize);
	freeDoubleMatrixSpace(psoInstanceData.velocities, swarmSize);
	freeDoubleMatrixSpace(psoInstanceData.bestPositions, swarmSize);
	//free array of cost values
	free(psoInstanceData.objVals);

	//-------------FREE ALLOCATED SPACE FOR MAINDATA--------------------------------------------------------------------------------------

	//free array of customer weights

	free(mainData.customerWeights);

	//free combined input matrix CI

	freeDoubleMatrixSpace(mainData.CI, mainData.n);

	return 0;
}