#include <igl/copyleft/marching_cubes.h>
#include <igl/signed_distance.h>
#include <igl/read_triangle_mesh.h>
#include <Eigen/Core>
#include <iostream>
#include <igl/writePLY.h>
#include <igl/decimate.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
  /*
    argv[1] in: bin
    argv[2] in: txt
    argv[3] in: level
    argv[4] in: decimation ratio
    argv[5] out: ply
  */
  using namespace Eigen;
  using namespace std;
  using namespace igl;
  MatrixXi F;
  MatrixXd V;
  FILE *f;
  int xdim,ydim,zdim;
  int sz;
  char datatype[512];
  float xvox, yvox, zvox;
  float level;
  float decimation;
  short *vol;
 
  cout<<"Marching cubes"<<endl;
  cout<<"in.bin: "<<argv[1]<<endl;
  cout<<"in.txt: "<<argv[2]<<endl;
  cout<<"in level: "<<argv[3]<<endl;
  cout<<"in decimation: "<<argv[4]<<endl;
  cout<<"out.ply: "<<argv[5]<<endl;

  level = atof(argv[3]);
  decimation = atof(argv[4]);

  f = fopen(argv[2],"r");
  fscanf(f, "dim: %i %i %i ", &xdim, &ydim, &zdim);
  fscanf(f, "dataType: %s ", datatype);
  fscanf(f, "voxelSize: %f %f %f ", &xvox, &yvox, &zvox);
  fclose(f);

  sz = xdim*ydim*zdim;
  vol = (short*)calloc(sz,sizeof(short));
  f = fopen(argv[1],"r");
  fread(vol,sz,sizeof(short),f);
  fclose(f);

  // create grid
  cout<<"Creating grid..."<<endl;
  MatrixXd GV(sz,3);
  VectorXd S(sz,1);

  int i, j, k, ijk;
  for(k = 0; k < zdim; k++)
  {
    for(j = 0; j < ydim; j++)
    {
      for(i = 0; i < xdim; i++)
      {
        ijk = i + xdim*j + xdim*ydim*k;
        GV.row(ijk) = RowVector3d((double)i,(double)j,(double)k);
        S(ijk) = (double)(vol[ijk]>level)?1:0;
      }
    }
  }

  free(vol);

  // marching cubes
  cout<<"Marching cubes..."<<endl;
  MatrixXd V1, V2;
  MatrixXi F1, F2;
  VectorXi J;
  igl::copyleft::marching_cubes(S,GV,xdim,ydim,zdim,V1,F1);
  cout<<"ntris: "<<F1.rows()<<endl;

  // decimate
  if(decimation>1)
  {
    cout<<"Decimating..."<<endl;
    igl::decimate(V1,F1,F1.rows()/decimation,V2,F2,J);
    cout<<"ntris: "<<F2.rows()<<endl;
  }
  else
  {
    cout<<"No decimation used."<<endl;
    V2=V1;
    F2=F1;
  }

  // scale voxel size
  for(i = 0; i < V2.rows(); i++)
  {
    V2(i, 0) *= xvox;
    V2(i, 1) *= yvox;
    V2(i, 2) *= zvox;
  }

  // save result
  igl::writePLY(argv[5], V2, F2, igl::FileEncoding::Ascii);

  cout<<"Done."<<endl;

}
