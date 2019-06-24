#include <netcdf>
#include <netcdf_par.h>
#include "file_io.hpp"
#include <iostream>
#include <libconfig.h++>
#include <stdio.h>
#include "mpi.h"
#include <cstring>

int main(int argc, char **argv) {
  MPI_Init(&argc,&argv);

  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  typedef double P;
  
  libconfig::Config cfg, cfg_geom;
  
  std::string file = "file.cfg";
  importLibConfig(cfg, file);
  
  const std::string var1 = "stuff.thing";
  int this_thing = getVariable<int>(cfg, var1);
  
  std::string ncfilename = getVariable<const char*>(cfg, "stuff.filename");
  std::string ncvarname = getVariable<const char*>(cfg, "stuff.variable_name");
  
  int err,status,ncid,groupid,cmode,val_id,r_id,retval;
  cmode = NC_NOWRITE;
  err = nc_open_par(ncfilename.c_str(), cmode, MPI_COMM_WORLD, MPI_INFO_NULL, &ncid);
  std::cout << "err " << err << std::endl;
  std::cout << "Error: " <<  nc_strerror(err) << std::endl;
  err=nc_inq_varid (ncid, ncvarname.c_str(), &val_id);
  std::cout << "Error: " <<  nc_strerror(err) << std::endl;
  int ndimsp;
  nc_inq_varndims (ncid, val_id, &ndimsp);
  std::cout << "val number of dims " << ndimsp << std::endl;
  int dimidsp[ndimsp];
  nc_inq_vardimid(ncid,val_id, &dimidsp[0]);
  std::cout << "val_dim_ids " << dimidsp[0] << " " << dimidsp[1]<< std::endl;

  size_t val_size=0;
  
  for(int i=0;i<ndimsp;i++)
  {
    size_t lenp;
    nc_inq_dimlen(ncid,dimidsp[i],&lenp);
    if(i==0)
    {
      val_size = lenp;
    }
    else
    {
      val_size = val_size*lenp;
    }
    std::cout << " i and dim size " << i << " " << lenp << std::endl;
    char dimname[128];
    nc_inq_dimname (ncid, dimidsp[i], &dimname[0]);
    std::cout << " name " << dimname << std::endl;
    int dim_var_id;
    nc_inq_varid (ncid, dimname, &dim_var_id);
    std::vector<P> coord(lenp);
    size_t startp=0;
    nc_get_vara_double (ncid, dim_var_id, &startp, &lenp, &coord[0]);
    for (auto it = coord.cbegin(); it != coord.cend(); it++)
    {
      std::cout << *it << ' ';
    }

  }
  std::vector<P> val(val_size);
  size_t startp=0;
  nc_get_var_double (ncid, val_id, &val[0]);
  std::cout << "val_id " << val_id  << std::endl;
  std::cout << "val_size " << startp << " " << val_size << std::endl;
  for(int i=0 ;i<val_size; i++)
  {
    std::cout << val[i] << " " ;
  }
  nc_close(ncid);
  MPI_Finalize();
  return 0;
}
