HOW IT RUNS WITH THE SCRIPT:

the script used is : treeDECoRexp.sh 
if the file is edited on Windows, the following is necessary after every edit before running the script: dos2unix treeDECoRexp.sh
(i have already done this after the last edit, so the problem should not occur)
update permissions: chmod +x treeDECoRexp.sh


to RUN: 

make clean
make 
./treeDECoRexp.sh

********************************************************************************************************************************
DECoR has the following parameters:

./runDECoR expID C N M F CR budget verbose methodChoice popSize

expID: unique integer id for an experiment, it is determined by a simple counter within the bash script

C,N,M : define the input file

F : mutation scaling factor

CR: crossover ratio 

verbose : if 0 info for each generation is NOT PRINTED on the terminal (input 1 to print that info)

methodChoice: chooses the mutation function -> 1: rand_1_bin , 2: rand_2_bin, 3: best_1_bin, 4: best_2_bin , if another num is used as input, the default option is 1
popSize: the population size/num of individuals

NOTE: in the script the first loop defines the number of experiments that will be done for each parametrization(how many "appends" per output file), it is set to 1

********************************************************************************************************************************

OUTPUT FILES DESCRIPTION:

STATS: expID, cost function value, budget, last hit, time, seed
MR: expID, median ranking


**NOTE: In the files sent , there are result files with the results of one command that was run (a few times) directly from the terminal (that is also why the expID is one every time)

I used the following parametrization: ./runDECoR 1 1 6 46 0.75 0.85 200 0 1 550

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

GENERAL (FOR SINGLE "MANUAL" RUN AND DEBUG) 

In a linux terminal:

COMPILE AND RUN :

FOR COMPILATION : make
FOR CLEAN UP OF .o FILES : make clean
TO RUN: ./runDECoR 1 1 6 46 0.75 0.85 200 0 1 550

******************************************************************************************************************************************************************************
ALTERNATIVE MEM LEAK CHECK RUN COMMAND for makefile: valgrind --track-origins=yes --leak-check=full ./runDECoR 1 1 6 46 0.75 0.85 200 0 1 550 2> valgrind_output.txt
******************************************************************************************************************************************************************************
the folder make_folder includes the following two alternative make files:

makefile: standard makefile with -g which supports gdb and valgrind
makefile_debug: a makefile alterned to support the Clang tools: AddressSanitizer (ASan) and UndefinedBehaviorSanitizer (UBSan) 

NOTE*: for the makefile to work makefile_debug must be renamed to makefile if it is to be used in the main folder that holds all the code and other files
 
!!! DEBUG AND MEM CHECK RUNS ARE SIGNIFICANTLY SLOWER !!!
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
