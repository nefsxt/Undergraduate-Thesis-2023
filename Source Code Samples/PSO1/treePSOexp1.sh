#!/bin/bash

#preset parameter values
C=(1 2 3)
N=(6 79 182)
M=(46 1393 7320)
chi=0.729
c1=2.05
c2=2.05

budgets=(25000 25000 50000 50000)
swarmSize_values=(200 200 100 100)
nbr_values=(1 1 1 1)
verbose=0
alphas=(0.25 0.5 0.25 0.5)

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
		echo "Running experiment $counter with parameters:"${C[f]}" "${N[f]}" "${M[f]}" "$chi" "$c1" "$c2" "${nbr_values[j]}" "${budgets[j]}" "$verbose" "${swarmSize_values[j]}" "${alphas[j]}" "
  
  					# Run PSo with the current parameter values
  					
					./runPSO "$counter" "${C[f]}" "${N[f]}" "${M[f]}" "$chi" "$c1" "$c2" "${nbr_values[j]}" "${budgets[j]}" "$verbose" "${swarmSize_values[j]}" "${alphas[j]}"
		done
	((counter++))	
	done

done
