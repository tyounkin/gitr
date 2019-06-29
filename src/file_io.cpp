#include "file_io.hpp"
#include <iostream>
#include "Field.hpp"
#include <netcdf>
#include <netcdf_par.h>
#include <libconfig.h++>

#include <iostream>
#include <stdio.h>
#include <cstring>

#include "mpi.h"

int importLibConfig(libconfig::Config &cfg, const std::string filepath) {
    try {
      cfg.readFile(filepath.c_str());
    } catch (const libconfig::FileIOException &fioex) {
      std::cerr << "Error while opening " << filepath << std::endl;
      std::cerr << "It is possible this file does not exist" << std::endl;
      exit (EXIT_FAILURE);
    } catch (const libconfig::ParseException &pex) {
      std::cerr << "Parse error of " << filepath << " at " << pex.getFile() << ":" << pex.getLine()
                << " - " << pex.getError() << std::endl;
      exit (EXIT_FAILURE);
    }
    std::cout << "Finished libconfig import  " << filepath.c_str() << std::endl;
    return (EXIT_SUCCESS);
}

template <typename T>
T getVariable(libconfig::Config &cfg, const std::string s) {
  T tmp;
  if (cfg.lookupValue(s, tmp)) {
    std::cout << s << " = " << tmp << std::endl;
  } else {
    std::cout << "ERROR: Failed importing " << s << std::endl;
    exit(EXIT_FAILURE);
  }
  return tmp;
}

template int getVariable(libconfig::Config &cfg, const std::string s);
template float getVariable(libconfig::Config &cfg, const std::string s);
template double getVariable(libconfig::Config &cfg, const std::string s);
template const char* getVariable(libconfig::Config &cfg, const std::string s);

//template <typename F>
//F importField(libconfig::Config &cfg,std::string s) {
//  typedef double P;
//  std::string ncfilename = getVariable<const char*>(cfg, s+".filename");
//  std::string ncvarname = getVariable<const char*>(cfg, s+".variable_name");
//  
//  int err,status,ncid,groupid,cmode,val_id,r_id,retval;
//  cmode = NC_NOWRITE;
//  err = nc_open_par(ncfilename.c_str(), cmode, MPI_COMM_WORLD, MPI_INFO_NULL, &ncid);
//  std::cout << "err " << err << std::endl;
//  std::cout << "Error: " <<  nc_strerror(err) << std::endl;
//  err=nc_inq_varid (ncid, ncvarname.c_str(), &val_id);
//  std::cout << "Error: " <<  nc_strerror(err) << std::endl;
//  int ndimsp;
//  nc_inq_varndims (ncid, val_id, &ndimsp);
//  //Field::n_dimensions=ndimsp;
//  std::cout << "val number of dims " << ndimsp << std::endl;
//  int dimidsp[ndimsp];
//  nc_inq_vardimid(ncid,val_id, &dimidsp[0]);
//  std::cout << "val_dim_ids " << dimidsp[0] << " " << dimidsp[1]<< std::endl;
//
//  size_t val_size=0;
//  std::vector<P> coord1;
//  std::vector<P> coord2;
//  std::vector<P> coord3;
//  
//  for(int i=0;i<ndimsp;i++)
//  {
//    size_t lenp;
//    nc_inq_dimlen(ncid,dimidsp[i],&lenp);
//    if(i==0)
//    {
//      val_size = lenp;
//    }
//    else
//    {
//      val_size = val_size*lenp;
//    }
//    std::cout << " i and dim size " << i << " " << lenp << std::endl;
//    char dimname[128];
//    nc_inq_dimname (ncid, dimidsp[i], &dimname[0]);
//    std::cout << " name " << dimname << std::endl;
//    int dim_var_id;
//    nc_inq_varid (ncid, dimname, &dim_var_id);
//    size_t startp=0;
//    //for (auto it = coord.cbegin(); it != coord.cend(); it++)
//    //{
//    //  std::cout << *it << ' ';
//    //}
//    if(i==0)
//    {
//      coord1.resize(lenp);
//      nc_get_vara_double (ncid, dim_var_id, &startp, &lenp, &coord1[0]);
//    }
//    else if(i==1)
//    {
//      coord2.resize(lenp);
//      nc_get_vara_double (ncid, dim_var_id, &startp, &lenp, &coord2[0]);
//    }
//
//  }
//  std::vector<P> val(val_size);
//  size_t startp=0;
//  nc_get_var_double (ncid, val_id, &val[0]);
//  std::cout << "val_id " << val_id  << std::endl;
//  std::cout << "val_size " << startp << " " << val_size << std::endl;
//  //for(int i=0 ;i<val_size; i++)
//  //{
//  //  std::cout << val[i] << " " ;
//  //}
//  nc_close(ncid);
//  
//  F field;
//
//  if(ndimsp == 2)
//  {
//    char dimname1[128];
//    char dimname2[128];
//    nc_inq_dimname (ncid, dimidsp[0], &dimname1[0]);
//    nc_inq_dimname (ncid, dimidsp[1], &dimname1[0]);
//    if(dimname1 == "r" & dimname2 == "z")
//    {
//      field.r = coord1;
//      field.nr = field.r.size();
//      field.dr = field.r[1] - field.r[0];
//      field.r_start = field.r.front();
//      field.r_end = field.r.back();
//      field.z = coord2;
//      field.nz = field.z.size();
//      field.dz = field.z[1] - field.z[0];
//      field.z_start = field.z.front();
//      field.z_end = field.z.back();
//      field.value = val;
//      return field;
//    }
//    else if(dimname1 == "x" & dimname2 == "z")
//    {
//      field.x = coord1;
//      field.nx = field.x.size();
//      field.dx = field.x[1] - field.x[0];
//      field.x_start = field.x.front();
//      field.x_end = field.x.back();
//      field.z = coord2;
//      field.nz = field.z.size();
//      field.dz = field.z[1] - field.z[0];
//      field.z_start = field.z.front();
//      field.z_end = field.z.back();
//      field.value = val;
//      return field;
//    }
//  }
//
//}
//template Field2d_rz importField(libconfig::Config &cfg,std::string s);
//template Field2d_xz importField(libconfig::Config &cfg,std::string s);
