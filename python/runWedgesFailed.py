import numpy as np
import os
from math import ceil

min  = np.array([0, 1, 2],    dtype=np.float64)
max  = np.array([5, 2, 3],    dtype=np.float64)
curr = np.zeros(min.shape, dtype=np.float64)
dx   = np.zeros(min.shape, dtype=np.float64)

# Calculate exponent
x = min.shape[0]
e = 1/x

N = 9 # Number of frames

# N = N'^x or N' = N^(1/x)
# A given interval gives us N'^x frames, so we 
# do the x root of the frame count to get our interval, 
# or the amount of times we increase per parameter
NN = ceil(N**e)

dx = (max - min) / (NN - 1)

count = 0
iter = 0
while (curr < max).all():
        
    curr = min + dx * iter
    curr[iter] = min[iter]
    
    # For each parameter
    for i in range(min.shape[0]):

        while (curr[i] <= max[i]):

            print(curr)

            curr[i] = curr[i] + dx[i]

            count = count + 1

        curr[i] = min[i] + dx[i] * iter

    iter += 1

print(count)
