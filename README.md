# Marching cubes

Roberto Toro, May 2019

Just a marching cubes. Based on libigl.

## Usage

```
marching_cubes input.bin input.txt input_level decimation_ratio output.ply
```

Where

* `input.bin`: binary volume, encoded in shorts (`int16`)
* `input.txt`: text file describing the data. Format is: <pre>
dim: xdim ydim zdim
dataType: short
voxelSize: xsize ysize zsize
</pre> Where `xdim`, `ydim` and `zdim` are integer number of voxels in the x, y and z dimensions; `short` is always `short` but one day may be something else; and `xsize`, `ysize` and `zsize` are floting point voxel dimensions (for example, `0.1`).
* `input_level`: the value in `input.bin` at which the mesh is created.
* `decimation_ratio`: the amount of decimation used after creating the raw mesh. For example, 1 if no decimation is required, or 10 to decrease triangles by 10 times.
* `output.ply`: the output mesh, in `.ply` format.

## Compilation

First, get a copy of libigl and put it at the same level of the `marching_cubes` directory. Then use the usual `cmake` procedure:

```
cd marching_cubes
mkdir build
cd build
cmake ..
make
```