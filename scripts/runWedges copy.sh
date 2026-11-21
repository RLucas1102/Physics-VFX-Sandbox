#!/bin/bash

# Parameter values 
min_octaves=1
min_freq=0.01
min_lac=0.01
min_gamma=0.1

max_octaves=7
max_freq=4
max_lac=1
max_gamma=3

# List of parameters that will be input to program
min=($min_octaves $min_freq)
max=($max_octaves $max_freq)
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
echo $NN

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
        curr=${min[i]}
        while (( $(echo "$curr <= ${max[i]}" | bc -l) )); do

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
            
            curr=$(awk "BEGIN {print $curr + ${dx[i]}}")

        done

        echo "Frame count: $N"
        echo "Interval: $NN"

    done

    # Increase each min by dx
    for ((i = 0 ; i < $x ; i++)); do
        min[i]=$(awk "BEGIN {print (${min[i]} + ${dx[i]})}")
    done

done