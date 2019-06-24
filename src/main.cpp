//#include <netcdf.h>
#include "netcdf4"
//#include "ncFile.h"
//#include "ncDim.h"
//#include "ncVar.h"
//#include "netcdf_par.h"
#include "file_io.hpp"
#include <iostream>
#include <libconfig.h++>
#include <stdio.h>
#include "mpi.h"
int main(int argc, char **argv) {
  std::cout << "main1" << std::endl;
  MPI_Init(&argc,&argv);

  std::cout << "main2" << std::endl;
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  std::cout << "main3" << std::endl;
     // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  std::cout << "main4" << std::endl;
  std::cout << "main5" << std::endl;
  typedef double P;
  libconfig::Config cfg, cfg_geom;
  P a = 1.0000000;
  std::string file = "file.cfg";
  importLibConfig(cfg, file);
  std::cout << "a " << a << std::endl;
  const std::string var1 = "stuff.thing";
  int this_thing = getVariable<int>(cfg, var1);
  std::string this_string = getVariable<const char*>(cfg, "stuff.string");

  std::string ncfilename = "netcdf_file_py.nc";
  int status,ncid,groupid;
  //netCDF::NcFile ncp(ncfilename.c_str(), netCDF::NcFile::read);
  netCDF::NcFile ncp(MPI_COMM_WORLD,MPI_INFO_NULL,ncfilename.c_str(), netCDF::NcFile::read);
  netCDF::NcVar nc_val(ncp.getVar("val"));
  std::vector<P> val;
  int val_size=0;
  int ndim_val = nc_val.getDimCount();
  std::vector<netCDF::NcDim> dims(ndim_val);
  dims = nc_val.getDims();
  //std::string val_string = "val";
  //std::set<netCDF::NcDim> dims = ncp.getDims (val_string);
  //std::set<netCDF::NcDim>::iterator dimsIt = dims.begin();
  for(int i=0;i<ndim_val;i++)
  {
    if(i==0)
    {
      val_size = dims[i].getSize();
    }
    else
    {
      val_size = val_size*dims[i].getSize();
    }
    std::cout << " i and dim size " << i << " " << dims[i].getSize() << std::endl;
    std::cout << " name " << dims[i].getName() << std::endl;
    std::vector<P> coord(dims[i].getSize());
    netCDF::NcVar tmp_val(ncp.getVar(dims[i].getName()));
    tmp_val.getVar(&coord[0]);
    for (auto it = coord.cbegin(); it != coord.cend(); it++)
    {
      std::cout << *it << ' ';
    }

  }
  val.resize(val_size);
  nc_val.getVar(&val[0]);
    for (auto it = val.cbegin(); it != val.cend(); it++)
    {
      std::cout << *it << ' ';
    }
  netCDF::NcDim ncDim1;
  netCDF::NcVar ncVar1;
  std::string rstring = "r";
  ncp.getCoordVar(rstring,ncDim1,ncVar1);
  //status = nc_open("netcdf_file_py.nc", NC_NOWRITE, &ncid);
  //if (status != NC_NOERR) handle_error(status);
  netCDF::NcDim ps_nP = ncp.getDim("r");
  int nPfile = ps_nP.getSize();
  std::cout << "r size " << nPfile << std::endl;
  std::vector<float> xpfile(nPfile);
  std::cout << "resized " << std::endl;
  netCDF::NcVar ncp_x(ncp.getVar("r"));
  std::cout << "got ncvar " << std::endl;
  ncp_x.getVar(&xpfile[0]);
  std::cout << "put var to vec "  << std::endl;
  int val_id;
  status = nc_inq_varid (0, "val", &val_id);
  std::cout << "val id "  <<val_id<< std::endl;
  //if (status != NC_NOERR) handle_error(status);
  ncp.close();
  std::cout << "closed file " << std::endl;
  MPI_Finalize();
  return 0;
}
