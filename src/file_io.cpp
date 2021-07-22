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
double interpolate_field_0d(double x_point,double y_point, double z_point, Field *obj){
   return obj->value[0];
 }

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
Field importField(std::string ncfilename,std::string ncvarname) {
  std::cout << "in importField "  << std::endl;
  typedef double P;
  
  int err,status,ncid,groupid,cmode,val_id,r_id,retval;
  cmode = NC_NOWRITE;
  //err = nc_open_par(ncfilename.c_str(), cmode, MPI_COMM_WORLD, MPI_INFO_NULL, &ncid);
  std::cout << "in importField 2"  << std::endl;
  err = nc_open(ncfilename.c_str(), cmode, &ncid);
  std::cout << "in importField 3"  << std::endl;
  std::cout << "err " << err << std::endl;
  std::cout << "Error: " <<  nc_strerror(err) << std::endl;
  err=nc_inq_varid (ncid, ncvarname.c_str(), &val_id);
  std::cout << "Error: " <<  nc_strerror(err) << std::endl;
  int ndimsp;
  nc_inq_varndims (ncid, val_id, &ndimsp);
  //Field::n_dimensions=ndimsp;
  std::cout << "val number of dims " << ndimsp << std::endl;
  int dimidsp[ndimsp];
  nc_inq_vardimid(ncid,val_id, &dimidsp[0]);
  for(int i=0; i< ndimsp; i++)
  {
    std::cout << "val_dim_ids " << i << " " << dimidsp[i] <<  std::endl;
  }
  
  Field field{};

  // Contant and 1D fields
  if(ndimsp == 1)
  {
    char dimname[128];
    nc_inq_dimname (ncid, dimidsp[0], &dimname[0]);
    std::cout << " name " << dimname << std::endl;
      std::cout << " at if statement " << std::endl;
    
    if(strncmp(dimname, "const",5)== 0)
    {
      std::cout << " handling const " << std::endl;
      //Field0d_const field0d_const{};
      std::vector<P> val(1);
      std::cout << " set val vector " << std::endl;
      nc_get_var_double (ncid, val_id, &val[0]);
      std::cout << " read ncfile and put variable to val " << std::endl;
      field.value = val;
      std::cout << " copied val to field value " << std::endl;
      field.interpolate_value = interpolate_field_0d;
      std::cout << "interpolated val " << field.interpolate_value(0.0,0.0,0.0,&field) << std::endl;
      return field;
    }
  }

  //size_t val_size=0;
  //std::vector<P> coord1;
  //std::vector<P> coord2;
  //std::vector<P> coord3;
  //
  //for(int i=0;i<ndimsp;i++)
  //{
  //  size_t lenp;
  //  nc_inq_dimlen(ncid,dimidsp[i],&lenp);
  //  if(i==0)
  //  {
  //    val_size = lenp;
  //  }
  //  else
  //  {
  //    val_size = val_size*lenp;
  //  }
  //  std::cout << " i and dim size " << i << " " << lenp << std::endl;
  //  char dimname[128];
  //  nc_inq_dimname (ncid, dimidsp[i], &dimname[0]);
  //  std::cout << " name " << dimname << std::endl;
  //  int dim_var_id;
  //  nc_inq_varid (ncid, dimname, &dim_var_id);
  //  size_t startp=0;
  //  //for (auto it = coord.cbegin(); it != coord.cend(); it++)
  //  //{
  //  //  std::cout << *it << ' ';
  //  //}
  //  if(i==0)
  //  {
  //    coord1.resize(lenp);
  //    nc_get_vara_double (ncid, dim_var_id, &startp, &lenp, &coord1[0]);
  //  }
  //  else if(i==1)
  //  {
  //    coord2.resize(lenp);
  //    nc_get_vara_double (ncid, dim_var_id, &startp, &lenp, &coord2[0]);
  //  }

  //}
  //std::vector<P> val(val_size);
  //size_t startp=0;
  //nc_get_var_double (ncid, val_id, &val[0]);
  //std::cout << "val_id " << val_id  << std::endl;
  //std::cout << "val_size " << startp << " " << val_size << std::endl;
  ////for(int i=0 ;i<val_size; i++)
  ////{
  ////  std::cout << val[i] << " " ;
  ////}
  nc_close(ncid);
  

  //if(ndimsp == 2)
  //{
  //  char dimname1[128];
  //  char dimname2[128];
  //  nc_inq_dimname (ncid, dimidsp[0], &dimname1[0]);
  //  nc_inq_dimname (ncid, dimidsp[1], &dimname1[0]);
  //  if(strncmp(dimname1,"r",3) & strncmp(dimname2, "z",3))
  //  {
  //    field.x = coord1;
  //    field.nx = field.x.size();
  //    field.dx = field.x[1] - field.x[0];
  //    field.x_start = field.x.front();
  //    field.x_end = field.x.back();
  //    field.z = coord2;
  //    field.nz = field.z.size();
  //    field.dz = field.z[1] - field.z[0];
  //    field.z_start = field.z.front();
  //    field.z_end = field.z.back();
  //    field.value = val;
  //    return field;
  //  }
  //  else if(strncmp(dimname1, "x",3) & strncmp(dimname2, "z",3))
  //  {
  //    field.x = coord1;
  //    field.nx = field.x.size();
  //    field.dx = field.x[1] - field.x[0];
  //    field.x_start = field.x.front();
  //    field.x_end = field.x.back();
  //    field.z = coord2;
  //    field.nz = field.z.size();
  //    field.dz = field.z[1] - field.z[0];
  //    field.z_start = field.z.front();
  //    field.z_end = field.z.back();
  //    field.value = val;
  //    return field;
  //  }
  //}
  return field;
}
//template Field importField(std::string file,std::string s);
//template Field0d_const importField(std::string file,std::string s);
//template Field2d_rz importField(std::string file,std::string s);
//template Field2d_xz importField(std::string file,std::string s);
