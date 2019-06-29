#include <netcdf>
#include <netcdf_par.h>
#include <libconfig.h++>

#include <iostream>
#include <stdio.h>
#include <cstring>

#include "Field.cpp"
#include "file_io.hpp"

#include "mpi.h"

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
  MPI_Finalize();
  return 0;
}
