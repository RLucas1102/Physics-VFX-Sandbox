import numpy as np
from math import floor, ceil

def toBinary(x, n):
    
    binary = np.zeros(n, dtype=np.int64)
    
    for k in reversed(range(n)):
        binary[k] = x%2
        x = floor(x / 2);

    return binary

N = 81

min = np.array([0, 1, 2, 3])
max = np.array([2, 4, 6, 8])

n = min.shape[0]

NN = ceil(N**(1/n))

dx = (max - min) / (NN - 1)

x = (2**n) - 1

count = 0 
while (min < max).all():

    for xx in range(x):

        minTemp = min.copy()

        binary = toBinary(xx, n)

        expr = (binary == 1)
        if (expr).any():
            while(np.not_equal(np.round(minTemp[expr], decimals=3), max[expr]).any()):
                minTemp = np.where(expr, minTemp+dx, minTemp)
                print(minTemp)
                count += 1
        else:
            print(minTemp)
            count += 1

    min = min + dx

print(max)
print(count + 1)
