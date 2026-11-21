import numpy as np
from math import ceil
import subprocess as sp
from pathlib import Path

min  = np.array([1, 0.1, 1.1, 0.1],    dtype=np.float64)
max  = np.array([7, 4, 3, 3],    dtype=np.float64)
dx   = np.zeros(min.shape, dtype=np.float64)

N = 505

x = min.shape[0]

NN = ceil(N/x)

dx = (max - min) / (NN - 1)

n = 0
path = Path("./images")
for i in range(min.shape[0]):

    curr = min.copy()

    for j in range(i, NN):

        padded = f"{n:04d}"

        filename = path / f"pyroclastic.{padded}.exr"

        curr[i] = min[i] + dx[i] * j

        sp.call(["./bin/main", f'{curr[0]}', f'{curr[1]}', f'{curr[2]}', f'{curr[3]}', filename.as_posix()])

        n += 1

sp.call(["./bin/main", f'{max[0]}', f'{max[1]}', f'{max[2]}', f'{max[3]}'])

