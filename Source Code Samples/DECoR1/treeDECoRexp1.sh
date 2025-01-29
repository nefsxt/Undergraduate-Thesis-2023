#!/bin/bash

#preset parameter values
C=(1 2 3)
N=(6 79 182)
M=(46 1393 7320)

budgets=(50000 50000 50000 50000)
F_values=(0.25 0.5 0.75 0.75)
CR_values=(0.5 0.5 0.5 0.8)
popSize_values=(100 100 100 100)
verbose=0
method_choice=1


#iterate over files
for((f=0;f<2;f++))
do
counter=1
# Run 25 experiments
for ((i=0; i<25; i++))
do	
	#iterate over 4 param combos
	for((j=0;j<4;j++))
	do
		echo "Running experiment $counter with parameters: "${C[f]}" "${N[f]}" "${M[f]}" "${F_values[j]}" "${CR_values[j]}" "${budgets[j]}" "$verbose" "$method_choice" "${popSize_values[j]}" "
  
  					# Run DECoR with the current parameter values
  					
					./runDECoR "$counter" "${C[f]}" "${N[f]}" "${M[f]}" "${F_values[j]}" "${CR_values[j]}" "${budgets[j]}" "$verbose" "$method_choice" "${popSize_values[j]}"

		done
	((counter++))	
	done

done
