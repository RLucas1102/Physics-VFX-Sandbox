import numpy as np
from itertools import product
import subprocess as sp
from pathlib import Path

min  = np.array([1, 0.1, 1.1, 0.1],    dtype=np.float64)
max  = np.array([7, 4, 3, 3],    dtype=np.float64)
levels = (4, 5, 5, 5)
axes = [np.linspace(lo, hi, k) for lo, hi, k in zip(min, max, levels)]
points = np.array(list(product(*axes)))

path = Path("./images")
for i in range(points.shape[0]):

    padded = f"{i:04d}"

    filename = path / f"pyroclastic.{padded}.exr"
    
    sp.call(["./bin/main", f'{points[i][0]}', 
                           f'{points[i][1]}', 
                           f'{points[i][2]}', 
                           f'{points[i][3]}', 
                           filename.as_posix()])

    command = "oiiotool"

    sp.call([command, filename.as_posix(),
                      '--text:x=700:y=100:font="Arial Bold":color=1,1,1:size=12',
                      f'Octaves:{points[i][0]}\nFrequency:{points[i][1]}\nLacunarity:{points[i][2]}\nGamma:{points[i][3]}',
                      '-o',
                      filename.as_posix()])