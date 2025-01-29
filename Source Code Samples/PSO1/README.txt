FIXES:
- chi out of parenthesis

- neighborhood funciton: get current, repetitive evals need to be done once in each loop or once in general

- matrices in loops that can be malloced and destroyed outside the loop (speed benefit) 

***Fixed creation not deletion, repeated deletion can't seem to be avoided. Did this for 1D wherever possible to avoid as much overhead as possible. -> maybe use static arrays since their size is known for rankingToScoreMatrix

-velocities: fix bounds, fix updates

-bound positions




HOW IT RUNS WITH THE SCRIPT:

the script used is : treePSOexp.sh 
if the file is edited on Windows, the following is necessary after every edit before running the script: dos2unix treePSOexp.sh 
(i have already done this after the last edit, so the problem should not occur)
update permissions: chmod +x treePSOexp.sh


to RUN: 

make clean
make 
./treePSOexp.sh

********************************************************************************************************************************
PSO has the following parameters:

./runPSO expID C N M chi c1 c2 nb_r budget verbose swarmSize a

expID: unique integer id for an experiment, it is determined by a simple counter within the bash script

C,N,M : define the input file

chi = 0.729, c1=c2=2.05 are the default pso parameters

nb_r : the neighborhood radius [ nb_r=0 : gbest , nb_r>0 : lbest]

verbose : if 0 info for each generation is NOT PRINTED on the terminal (input 1 to print that info)

swarmSize: the swarm size/num of particles

a: alpha value for velocity bound, the values used are 0.25 and 0.5

NOTE: in the script the first loop defines the number of experiments that will be done for each parametrization(how many "appends" per output file), it is set to 1

********************************************************************************************************************************

OUTPUT FILES DESCRIPTION:

STATS: expID, cost function value, budget, last hit, time, seed
MR: expID, median ranking

**NOTE: In the files sent , there are result files with the results of one command that was run a few times directly from the terminal (that is also why the expID is one every time)

I used the following parametrization: ./runPSO 1 1 6 46 0.729 2.05 2.05 3 2000 0 100 0.5



----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

GENERAL (FOR SINGLE "MANUAL" RUN AND DEBUG)


In a linux terminal:

COMPILE AND RUN :

FOR COMPILATION : make
FOR CLEAN UP OF .o FILES : make clean
TO RUN:  (example paremeters): ./runPSO 1 1 6 46 0.729 2.05 2.05 3 2000 0 100 0.5

******************************************************************************************************************************************************************************
ALTERNATIVE MEM LEAK CHECK RUN COMMAND for makefile: valgrind --track-origins=yes --leak-check=full ./executeAll 2> valgrind_output.txt
valgrind --track-origins=yes --leak-check=full ./runPSO 1 1 6 46 0.729 2.05 2.05 3 2000 0 100 0.5 2> valgrind_output.txt
******************************************************************************************************************************************************************************
the folder make_folder includes the following two alternative make files:

makefile: standard makefile with -g which supports gdb and valgrind
makefile_debug: a makefile alterned to support the Clang tools: AddressSanitizer (ASan) and UndefinedBehaviorSanitizer (UBSan) 

NOTE*: for the makefile to work makefile_debug must be renamed to makefile if it is to be used in the main folder that holds all the code and other files

!!! DEBUG AND MEM CHECK RUNS ARE SIGNIFICANTLY SLOWER !!!
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

