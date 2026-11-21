import numpy as np
from itertools import product
import subprocess as sp
from pathlib import Path

min  = np.array([0.01, 0.1, 0.01, 0.1, 1],    dtype=np.float64)
max  = np.array([0.95, 4, 2, 3, 7],    dtype=np.float64)
levels = (3, 3, 3, 3, 3)
axes = [np.linspace(lo, hi, k) for lo, hi, k in zip(min, max, levels)]
points = np.array(list(product(*axes)))

print(points.shape)

path = Path("./images/terrain")
for i in range(points.shape[0]):

    padded = f"{i:04d}"

    filename = path / f"terrain.{padded}.exr"
    
    sp.call(["./bin/main", f'{points[i][0]}', 
                           f'{points[i][1]}', 
                           f'{points[i][2]}', 
                           f'{points[i][3]}',
                           f'{points[i][4]}', 
                           filename.as_posix()])

    command = "oiiotool"

    sp.call([command, filename.as_posix(),
                      '--text:x=700:y=100:font="Arial Bold":color=1,1,1:size=12',
                      f'Roughness:{points[i][0]}\nGain:{points[i][1]}\nFreq:{points[i][2]}\nLac:{points[i][3]}\nOctaves:{points[i][4]}',
                      '-o',
                      filename.as_posix()])