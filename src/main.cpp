#include <netcdf>
#include <netcdf_par.h>
#include <libconfig.h++>

#include <iostream>
#include <stdio.h>
#include <cstring>

#include "Field.hpp"
#include "file_io.hpp"
#include "managed.hpp"

#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/execution_policy.h>
#include <thrust/sequence.h>
#include <thrust/transform.h>
#include "mpi.h"

#ifdef USE_DOUBLES
typedef double P;
#else
typedef float P;
#endif

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
struct this_functor
{
  Field* field;
  this_functor(Field* _field) : field(_field) {}

    void operator()(size_t index) const {
      std::cout << "interpolated val " << field->interpolate_value(0.0,0.0,0.0,field) << std::endl;
      //printf ("inside functor");
    }
};
struct that_functor
{
  int * a;
  that_functor(int * _a) : a(_a) {}

    void operator()(size_t index) const {
      printf ("inside functor");
    }
};

int main(int argc, char **argv) {
  std::cout << "hi" << std::endl;
  //MPI_Init(&argc,&argv);

  //std::cout << "hi2" << std::endl;
  //int world_size;
  //MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  //std::cout << "hi3" << std::endl;

  //int world_rank;
  //MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  //std::cout << "hi4" << std::endl;
  
  Field te = importField("plasma_profiles.nc","te");
  std::cout << "hi5" << std::endl;
  //typedef Field2d_rz P2;
  //
  //libconfig::Config cfg, cfg_geom;
  //
  //std::string file = "file.cfg";
  //importLibConfig(cfg, file);
  //
  //const std::string var1 = "stuff.thing";
  //int this_thing = getVariable<int>(cfg, var1);
  //
  //std::string ncfilename = getVariable<const char*>(cfg, "stuff.filename");
  //std::string ncvarname = getVariable<const char*>(cfg, "stuff.variable_name");
  //
  //std::string field1_string = "stuff.field1";
  //P2 magnetic_field(cfg,"stuff.field1");
  ////std::cout << "magnetic field stuff " <<  magnetic_field.n_dimensions << std::endl;
  //  for (auto it = magnetic_field.r.cbegin(); it != magnetic_field.r.cend(); it++)
  //  {
  //    std::cout << *it << ' ';
  //  }
  //  for (auto it = magnetic_field.z.cbegin(); it != magnetic_field.z.cend(); it++)
  //  {
  //    std::cout << *it << ' ';
  //  }
  //  for (auto it = magnetic_field.value.cbegin(); it != magnetic_field.value.cend(); it++)
  //  {
  //    std::cout << *it << ' ';
  //  }
  //P bfield = magnetic_field.interpolate_field(0.2,0.0, 0.5);
  //std::cout << " magnetic field at 0.2, 0.5 " << bfield << std::endl;
  //int nP=1000;
  ////std::vector<float> vec(nP,0.0);
  //thrust::host_vector<P> vec(nP);
  //thrust::device_vector<P> dVec = vec;
  //thrust::counting_iterator<std::size_t> iterator0(0);
  ////thrust::device_ptr devPtr = &dVec[0];
  //double * devPtr = thrust::raw_pointer_cast(&(dVec[0]));
  //init init0(devPtr);
  //thrust::for_each(thrust::device,iterator0,iterator0+nP,init0);
  //for(int i = 0; i < dVec.size(); i++)
  //      std::cout << "D[" << i << "] = " << dVec[i] << std::endl;
  thrust::counting_iterator<std::size_t> point_first(0);
  thrust::counting_iterator<std::size_t> point_last(10);
  this_functor thf(&te);
  thrust::for_each(thrust::device,point_first,point_last,thf);
  int tha = 4;
  that_functor tf(&tha);
  thrust::for_each(thrust::device,point_first,point_last,tf);
  //MPI_Finalize();
  return 0;
}
