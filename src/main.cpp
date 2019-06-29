#include <netcdf>
#include <netcdf_par.h>
#include <libconfig.h++>

#include <iostream>
#include <stdio.h>
#include <cstring>

#include "Field.cpp"
#include "file_io.hpp"

#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/execution_policy.h>
#include <thrust/sequence.h>
#include <thrust/transform.h>
#include "mpi.h"

struct init
{
    double * inVec;
    //thrust::device_ptr inVec;
    init(double * _inVec):inVec(_inVec) {};
    void operator()(std::size_t indx)
    {
        inVec[indx]=2.0*indx;
    }
};

int main(int argc, char **argv) {
  MPI_Init(&argc,&argv);

  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  typedef double P;
  typedef Field2d_rz P2;
  
  libconfig::Config cfg, cfg_geom;
  
  std::string file = "file.cfg";
  importLibConfig(cfg, file);
  
  const std::string var1 = "stuff.thing";
  int this_thing = getVariable<int>(cfg, var1);
  
  std::string ncfilename = getVariable<const char*>(cfg, "stuff.filename");
  std::string ncvarname = getVariable<const char*>(cfg, "stuff.variable_name");
  
  std::string field1_string = "stuff.field1";
  P2 magnetic_field(cfg,"stuff.field1");
  //std::cout << "magnetic field stuff " <<  magnetic_field.n_dimensions << std::endl;
    for (auto it = magnetic_field.r.cbegin(); it != magnetic_field.r.cend(); it++)
    {
      std::cout << *it << ' ';
    }
    for (auto it = magnetic_field.z.cbegin(); it != magnetic_field.z.cend(); it++)
    {
      std::cout << *it << ' ';
    }
    for (auto it = magnetic_field.value.cbegin(); it != magnetic_field.value.cend(); it++)
    {
      std::cout << *it << ' ';
    }
  P bfield = magnetic_field.interpolate_field(0.2,0.0, 0.5);
  std::cout << " magnetic field at 0.2, 0.5 " << bfield << std::endl;
  int nP=1000;
  //std::vector<float> vec(nP,0.0);
  thrust::host_vector<P> vec(nP);
  thrust::device_vector<P> dVec = vec;
  thrust::counting_iterator<std::size_t> iterator0(0);
  //thrust::device_ptr devPtr = &dVec[0];
  double * devPtr = thrust::raw_pointer_cast(&(dVec[0]));
  init init0(devPtr);
  thrust::for_each(thrust::device,iterator0,iterator0+nP,init0);
  for(int i = 0; i < dVec.size(); i++)
        std::cout << "D[" << i << "] = " << dVec[i] << std::endl;
  MPI_Finalize();
  return 0;
}
