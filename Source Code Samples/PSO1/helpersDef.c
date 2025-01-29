#include "imports.h"
#include "helpers.h"



//input: an array of doubles (a ranking) 
//output: an nxn array of doubles- > the CI matrix which is the weighted sum of all score matrices that are results of the input dataset's rankings
void rankingToScoreMatrix(int* ranking, int size, double scoreMatrix[size][size]) {
    
    //allocate memory
    //double** scoreMatrix = (double**)malloc(size * sizeof(double*));
    //for (int i = 0; i < size; i++) {
    //    scoreMatrix[i] = (double*)malloc(size * sizeof(double));
    //}

    // the indexes of the ij pairs are those 'below' the diagonal
    //Kendall's original formulation score:
    // if item at pos i rated ahead of item at pos j (this means ranking[i]<ranking[j]) then score = 1
    // if the element is on the diagonal or there is a tie then score = 0 
    // if item at pos i rated below item at pos j then score = -1

    //set diagonal elements to zero
    for (int i = 0; i < size; i++) {
        scoreMatrix[i][i] = 0*(1.0);
    }

    //score according to the rest of the conditions
    for (int i = 1; i < size; i++) {
        for (int j = 0; j < i; j++) {
            if(ranking[i]<ranking[j])
            {
                scoreMatrix[i][j] = 1 * (1.0);
                scoreMatrix[j][i] = -1 * (1.0);
            }
            else if (ranking[i] == ranking[j])
            {
                scoreMatrix[i][j] = 0 * (1.0);
                scoreMatrix[j][i] = 0 * (1.0);
            }
            else 
            {
                scoreMatrix[i][j] = -1 * (1.0);
                scoreMatrix[j][i] = 1 * (1.0);
            }
            
            //for index debugging    
            //printf("------------\n");
            //printf("%d \n", matrix[i][j]);
            //printf("i = %d \n", i);
            //printf("j = %d \n", j);

        }
    }
   
    //return scoreMatrix;
}

// calculates and returns the kemeny distance between two square matrices A and B with the same dimension
double kemenyDistance(int size,double** matrixA, double matrixB[size][size])
{
    double sum = 0; 
        for (int i = 1; i < size; i++) {
            for (int j = 0; j < i; j++) {
                sum += fabs(matrixA[i][j] - ((double)matrixB[i][j]));

            }
        }
    return sum;
}

// calculates and returns the tau x rank correlation coefficient (Emond and Mason formula) 
double tau_x(int size,double** matrixA, double matrixB[size][size])
{
    double kemeny_distance = kemenyDistance(size,matrixA, matrixB);
    double taux = 1 - ((2 * kemeny_distance) / (size*(size - 1)));
    return taux;
}


// calculates and returns the value of the objective function argmax Ó cij*yij for i,j = 1 to N where Y E S^n
double objectiveFunction(double** c, double** y)
{
    double value = 0;

    for (int i = 0; i < mainData.n; i++)
    {
        for (int j = 0; j < mainData.n; j++)
        {
            value += c[i][j] * y[i][j];
        }
    }
    return value;
}

//computes the sum of all customer weights stored in mainData.customerWeights
double weightSum() 
{
    double sum = 0;
    for (int i = 0; i < mainData.m; i++)
    {
        sum += mainData.customerWeights[i];
    }
    return sum;
}
// sets the constant product needed for the cost function that represents Ó wk*(n*(n-1)\2) from cost(s) = Ó wk*(n*(n-1)\2)*(1-tau_x(cij,sij)) for k= 1 to m 
void costFunctionDataSetup() 
{
    
    double n = mainData.n * (1.0);
    //printf("costProduct = %f\n", weightSum() * ((n * (n - 1)) / 2));
    mainData.costProduct = weightSum()* ((n * (n - 1)) / 2);
 
}
// calculates and returns the value of the cost function cost(s) = Ó wk*(n*(n-1)\2)*(1-tau_x(cij,sij)) for k= 1 to m 
//cij the combined input matrix , sij a score matrix 
//input: an NxN  score matrix s that corresponds to sij, the rest are global data
//output: the value of the calculated cost
double costFunction(int size, double s[size][size])
{
    double cost= mainData.costProduct * (1 - tau_x(mainData.n,mainData.CI, s));
    return cost;
}



//helper function for sortAndRankn -> used as the comparator function in qsort
int compare(const void* a, const void* b) {
    //cast and derefrenence the input pointers
    // double**: pointer to (pointer to double) -> to cast and deref. and get a pointer to double: *(double**)
    double* ptr_a = *(double**)a;
    double* ptr_b = *(double**)b;


    //compare the values that the pointers point to
    if (*ptr_a < *ptr_b) {
        return -1;
    }
    else if (*ptr_a > *ptr_b) {
        return 1;
    }
    else {
        return 0;
    }

}

//sorts and ranks an input array -> produces the ranking that array represents
//ties are broken by the sort algorithm
void sortAndRank(double* rankArray, int n, int ranking[n])
{
    //printf("sort start\n");
    //init each index[i] pos to point to &arr[i] (the memory address of arr[i])
    // instead of swapping the elemens of arr, it will be the pointers in index that will be swapped around
    //in the end: index will contain pointers that point to the elements of arr, but in the sorted order.

    //https://stackoverflow.com/questions/66400631/qsort-cast-the-comparator-function-itself-or-the-parameters-in-the-body-of-comp
    //https://stackoverflow.com/questions/36714030/c-sort-float-array-while-keeping-track-of-indices


    double* index[n];


    for (int i = 0; i < n; i++) {
        index[i] = &rankArray[i];
    }

    qsort(index, n, sizeof(*index), compare); // a quicksort implementation is being used : O(NlogN) time complexity


    //int* ranking = (int*)malloc(n * sizeof(int));
    int idx;
    for (int i = 0; i < n; i++) {
        idx = (int)(index[i] - &rankArray[0]); // the index of the element index[i] in the original rankArray
        //index[i] - &rankArray[0] : the number of items between the two mem addr/offset from the beginning of rankArray
        //printf("%d ", idx);
        ranking[idx] = i + 1;
        //ex: rankArray={ 4.1,2.5,6.7,8.23,1.99 } the result for each index[i] - &rankArray[0] will be: { 4,1,0,2,3 }
        //the ranking will be given as follows: ranking[4]=0+1=1, ranking[1]=1+1=2, ranking[0]=2+1=3, ranking[2]=3+1, ranking[3]=4+1=5
    }
    //total complexity: O(NlogN)+O(N)=O(NlogN)
    //printf("sort end\n");
    //return ranking;
}


//DATA READING***************************************************************************************************************************************************************


//reads a file containing rankings of N items by M judges 
//sets the global struct's mainData.CI to a matrix containing the combined input matrix
void readRankingsAndCreateCI(char* filename)
{
    printf("Reading Rankings...\n");
    // get the N,M values from filename
    char* n_ptr = strstr(filename, "N_"); // find the position of "N_"
    int n = 0;
    if (n_ptr != NULL) {
        if (sscanf(n_ptr + 2, "%d", &n) == 1) { // read the number after "N_"
            printf(" N =  %d\n", n);
            mainData.n = n;
        }
        else { printf("Filename not recognised"); }
    }
    else { printf("Filename not recognised"); }

    char* m_ptr = strstr(filename, "M_"); // find the position of "M_"
    int m = 0;
    if (m_ptr != NULL) {
        if (sscanf(m_ptr + 2, "%d", &m) == 1) { // read the number after "M_"
            printf(" M =  %d\n", m);
            mainData.m = m;
        }
        else { printf("Filename not recognised"); }
    }
    else { printf("Filename not recognised"); }


    //open file and read line by line
    //each line is turned into an array containing a ranking, which is used to produce the score matrix for that ranking 

    FILE* fp = fopen(filename, "r"); // Open file for reading

    int k = 0; // index counter for individuals

    //https://stackoverflow.com/questions/3501338/c-read-file-line-by-line

    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    if (fp == NULL) {
        printf("Could not open file\n");
    }

    //allocate memory for CI
    double** CI = (double**)malloc(mainData.n * sizeof(double*));
    for (int i = 0; i < mainData.n; i++) {
        CI[i] = (double*)malloc(mainData.n * sizeof(double));
    }
    // initialize the CI matrix with zeros
    for (int i = 0; i < mainData.n; i++) {
        for (int j = 0; j < mainData.n; j++) {
            CI[i][j] = 0;
        }
    }

    double scoreMatrix[mainData.n][mainData.n];

    while ((read = getline(&line, &len, fp)) != -1) {
        //printf("%s", line); //for debugging

        // Tokenize the line
        char* token;
        int ranking[n]; // n items are expected
        int i = 0;
        token = strtok(line, "    ");
        while (token != NULL && i < n)
        {
            ranking[i++] = atoi(token); //turn into int
            token = strtok(NULL, " "); //go to next
        }

        // print list**************************************************
        /*
        for (int k = 0; k < i; k++) {
            printf("%d ", ranking[k]);
        }
        printf("\n");
        */
        //*************************************************************
        //printf("---------------------------------------------------\n");//for debugging
        
        //multiply score matrix with its respective weight and add it to CI

        int size = sizeof(ranking) / sizeof(ranking[0]);
        rankingToScoreMatrix(ranking, size,scoreMatrix);//create the scoreMatrix for the current ranking

        for (int i = 0; i < mainData.n; i++) {
            for (int j = 0; j < mainData.n; j++) {
                CI[i][j] = CI[i][j] + scoreMatrix[i][j] * mainData.customerWeights[k];
            }
        }
        
        //print score matrix for the current ranking*********************
        /*
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                printf("%d      ", scoreMatrix[i][j]);

            }
            printf("\n");
        }
        */
        //***************************************************************


        k++; // go to next score matrix index
        //printf("---------------------------------------------------\n"); //for debugging
 
        //freeDoubleMatrixSpace(scoreMatrix,mainData.n);


    }
    free(line); // free the memory allocated by getline

    fclose(fp); // Close file

    mainData.CI = CI; //set the global struct value of CI to CI
    //freeDoubleMatrixSpace(scoreMatrix,mainData.n);
    //freeDoubleMatrixSpace(CI,mainData.n);
    printf("COMPLETE!\n");
}


//reads a file containing weights of importance of each customer 
//sets the global struct's mainData.customerWeights to an array containing the weights
void readWeights(char* filename) 
{
    printf("Reading Weights...\n");
    char* m_ptr = strstr(filename, "M_"); // find the position of "M_"
    int m = 0;
    if (m_ptr != NULL) {
        if (sscanf(m_ptr + 2, "%d", &m) == 1) { // read the number after "M_"
            printf("M =  %d\n", m);
            mainData.m = m;
        }
        else { printf("Filename not recognised"); }
    }
    else { printf("Filename not recognised"); }
     

    //open file and read line by line
    //there should only be one line (containing the weights)

    FILE* fp = fopen(filename, "r"); // Open file for reading

    double* customerWeights = malloc(m * sizeof(double)); //array of customer weights
    
    //https://stackoverflow.com/questions/3501338/c-read-file-line-by-line

    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    if (fp == NULL) {
        printf("Could not open file\n");
    }


    while ((read = getline(&line, &len, fp)) != -1) {
        //printf("%s", line); //for debugging

        // Tokenize the line
        char* token;
        int i = 0;
        token = strtok(line, "    ");
        while (token != NULL)
        {
            customerWeights[i++] = atof(token); //turn into double
            token = strtok(NULL, " "); //go to next
        }

        // print list**************************************************
        /*
        for (int k = 0; k < m; k++) {
            printf("%d ", customerWeights[k]);
        }
        printf("\n");
        */
        //*************************************************************
       
        mainData.customerWeights = customerWeights;
        

    }
    free(line); // free the memory allocated by getline

    fclose(fp); // Close file
    //free(customerWeights);
    
    printf("COMPLETE!\n");

}

// frees up the space allocated for a matrix of doubles
void freeDoubleMatrixSpace(double** matrix, int size) 
{
    for (int i = 0; i < size; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// frees up the space allocated for a matrix of ints
void freeIntMatrixSpace(int** matrix, int size)
{
    for (int i = 0; i < size; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void globalMainDataSetup(char* filenameW,char* filenameR) 
{
    //read weights
    readWeights(filenameW);
    // read rankings and create combined input matrix CI
    readRankingsAndCreateCI(filenameR);
}
