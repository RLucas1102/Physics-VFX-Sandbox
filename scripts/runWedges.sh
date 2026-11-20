#!/bin/bash

# List of parameters that will be input to program
min=(0 1)
max=(5 2)
dx=()

# Calculate exponent
x=${#min[@]}
e=$(awk "BEGIN {print 1/$x}")

N=25 # Number of frames

# N = N'^x or N' = N^(1/x)
# A given interval gives us N'^x frames, so we 
# do the x root of the frame count to get our interval, 
# or the amount of times we increase per parameter
NN=$(awk "BEGIN {print $N**$e}")
NN=$(awk "BEGIN {if ($N % $NN) print $NN+1; else print $NN"}) # Ceiling if not evenly divisible
NN=${NN%.*} # Remove floating point precision

# Calculate dx for each parameter
for ((i = 0 ; i < $x ; i++)); do
    dx+=($(awk "BEGIN {print (${max[i]} - ${min[i]})/($NN - 1)}"))
done

# Continue loop unitl we have created N frames
while [ $N -gt 0 ]; do

    # For each parameter
    for ((i = 0 ; i < $x ; i++)); do

        echo "Parameter: $i"

        # Starting from i, increase until max is reached
        for ((j = $i ; j < $NN ; j++)); do
            
            curr=$(awk "BEGIN {print ${min[i]} + ${dx[i]} * $j}")

            str=""
            for ((k = 0; k < $x ; k++)); do
                if [ $k -ne $i ]; then
                    str="$str ${min[k]}"
                else
                    str="$str $curr"
                fi
            done

            echo $str

            N=$(awk "BEGIN {print $N - 1}") # Decrease frame count 

        done

    done
    
    NN=$(awk "BEGIN {print $NN - 1}") # Decrease interval for j

    # Increase each min by dx
    for ((i = 0 ; i < $x ; i++)); do
        min[i]=$(awk "BEGIN {print (${min[i]} + ${dx[i]})}")
    done

done