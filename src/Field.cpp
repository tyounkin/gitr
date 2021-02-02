#include "Field.hpp"
#include "file_io.hpp"
#include <cmath>
#include <netcdf_par.h>
#include "mpi.h"
typedef double P;
Field2d_rz::Field2d_rz() {}
Field2d_xz::Field2d_xz() {}

Field2d_rz::Field2d_rz(libconfig::Config &cfg,std::string s) {
  typedef double P;
  std::string ncfilename = getVariable<const char*>(cfg, s+".filename");
  std::string ncvarname = getVariable<const char*>(cfg, s+".variable_name");
  
  int err,status,ncid,groupid,cmode,val_id,r_id,retval;
  cmode = NC_NOWRITE;
  err = nc_open_par(ncfilename.c_str(), cmode, MPI_COMM_WORLD, MPI_INFO_NULL, &ncid);
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
  std::cout << "val_dim_ids " << dimidsp[0] << " " << dimidsp[1]<< std::endl;

  size_t val_size=0;
  std::vector<P> coord1;
  std::vector<P> coord2;
  std::vector<P> coord3;
  
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
    size_t startp=0;
    //for (auto it = coord.cbegin(); it != coord.cend(); it++)
    //{
    //  std::cout << *it << ' ';
    //}
    if(i==0)
    {
      coord1.resize(lenp);
      std::cout << "coord1 size " << coord1.size() << std::endl;
      nc_get_vara_double (ncid, dim_var_id, &startp, &lenp, &coord1[0]);
    for (auto it = coord1.cbegin(); it != coord1.cend(); it++)
    {
      std::cout << *it << ' ';
    }
    }
    else if(i==1)
    {
      coord2.resize(lenp);
      nc_get_vara_double (ncid, dim_var_id, &startp, &lenp, &coord2[0]);
    }

  }
  std::vector<P> val(val_size);
  size_t startp=0;
  nc_get_var_double (ncid, val_id, &val[0]);
  std::cout << "val_id " << val_id  << std::endl;
  std::cout << "val_size " << startp << " " << val_size << std::endl;
  //for(int i=0 ;i<val_size; i++)
  //{
  //  std::cout << val[i] << " " ;
  //}
  nc_close(ncid);
  
  //Field2d_rz field;
    for (auto it = coord1.cbegin(); it != coord1.cend(); it++)
    {
      std::cout << *it << ' ';
    }

  if(ndimsp == 2)
  {
    char dimname1[128];
    char dimname2[128];
    nc_inq_dimname (ncid, dimidsp[0], &dimname1[0]);
    nc_inq_dimname (ncid, dimidsp[1], &dimname1[0]);
      Field2d_rz::r=coord1;
    for (auto it = coord1.cbegin(); it != coord1.cend(); it++)
    {
      std::cout << *it << ' ';
    }
      Field2d_rz::nr = Field2d_rz::r.size();
      Field2d_rz::dr = Field2d_rz::r[1] - Field2d_rz::r[0];
      Field2d_rz::r_start = Field2d_rz::r.front();
      Field2d_rz::r_end = Field2d_rz::r.back();
      Field2d_rz::z = coord2;
      Field2d_rz::nz = Field2d_rz::z.size();
      Field2d_rz::dz = Field2d_rz::z[1] - Field2d_rz::z[0];
      Field2d_rz::z_start = Field2d_rz::z.front();
      Field2d_rz::z_end = Field2d_rz::z.back();
      Field2d_rz::value = val;
      //return Field2d_rz;
    //else if(dimname1 == "x" & dimname2 == "z")
    //{
    //  field.x = coord1;
    //  field.nx = field.x.size();
    //  field.dx = field.x[1] - field.x[0];
    //  field.x_start = field.x.front();
    //  field.x_end = field.x.back();
    //  field.z = coord2;
    //  field.nz = field.z.size();
    //  field.dz = field.z[1] - field.z[0];
    //  field.z_start = field.z.front();
    //  field.z_end = field.z.back();
    //  field.value = val;
    //  return field;
    //}
  }

}
Field2d_xz::Field2d_xz(libconfig::Config &cfg,std::string s) {
  typedef double P;
  std::string ncfilename = getVariable<const char*>(cfg, s+".filename");
  std::string ncvarname = getVariable<const char*>(cfg, s+".variable_name");
  
  int err,status,ncid,groupid,cmode,val_id,r_id,retval;
  cmode = NC_NOWRITE;
  err = nc_open_par(ncfilename.c_str(), cmode, MPI_COMM_WORLD, MPI_INFO_NULL, &ncid);
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
  std::cout << "val_dim_ids " << dimidsp[0] << " " << dimidsp[1]<< std::endl;

  size_t val_size=0;
  std::vector<P> coord1;
  std::vector<P> coord2;
  std::vector<P> coord3;
  
  for(int i=0;i<ndimsp;i++)
  {
    size_t lenp;
    nc_inq_dimlen(ncid,dimidsp[i],&lenp);
    if(i==0)
    {
      std::cout << "i==0" << std::endl;
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
    size_t startp=0;
    if(i==0)
    {
      coord1.resize(lenp);
      nc_get_vara_double (ncid, dim_var_id, &startp, &lenp, &coord1[0]);
      std::cout << " i=0 coord1 " << coord1.size() << std::endl;
    for (auto it = coord1.cbegin(); it != coord1.cend(); it++)
    {
      std::cout << *it << ' ';
    }
    }
    else if(i==1)
    {
      coord2.resize(lenp);
      nc_get_vara_double (ncid, dim_var_id, &startp, &lenp, &coord2[0]);
    for (auto it = coord2.cbegin(); it != coord2.cend(); it++)
    {
      std::cout << *it << ' ';
    }
    }

  }
  std::vector<P> val(val_size);
  size_t startp=0;
  nc_get_var_double (ncid, val_id, &val[0]);
    for (auto it = val.cbegin(); it != val.cend(); it++)
    {
      std::cout << *it << ' ';
    }
  std::cout << "val_id " << val_id  << std::endl;
  std::cout << "val_size " << startp << " " << val_size << std::endl;
  //for(int i=0 ;i<val_size; i++)
  //{
  //  std::cout << val[i] << " " ;
  //}
  nc_close(ncid);
  
  Field2d_xz field;

    //if(dimname1 == "r" & dimname2 == "z")
    //{
    //  field.r = coord1;
    //  field.nr = field.r.size();
    //  field.dr = field.r[1] - field.r[0];
    //  field.r_start = field.r.front();
    //  field.r_end = field.r.back();
    //  field.z = coord2;
    //  field.nz = field.z.size();
    //  field.dz = field.z[1] - field.z[0];
    //  field.z_start = field.z.front();
    //  field.z_end = field.z.back();
    //  field.value = val;
    //  return field;
    //}
      Field2d_xz::x = coord1;
      Field2d_xz::nx = Field2d_xz::x.size();
      Field2d_xz::dx = Field2d_xz::x[1] - Field2d_xz::x[0];
      Field2d_xz::x_start = Field2d_xz::x.front();
      Field2d_xz::x_end = Field2d_xz::x.back();
      Field2d_xz::z = coord2;
      Field2d_xz::nz = Field2d_xz::z.size();
      Field2d_xz::dz = Field2d_xz::z[1] - Field2d_xz::z[0];
      Field2d_xz::z_start = Field2d_xz::z.front();
      Field2d_xz::z_end = Field2d_xz::z.back();
      Field2d_xz::value = val;
      //return field;

}
//Field::Field(libconfig::Config &cfg,std::string s) {
//  std::string ncfilename = getVariable<const char*>(cfg, (s+".filename").c_str());
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
//  Field::n_dimensions=ndimsp;
//  std::cout << "val number of dims " << ndimsp << std::endl;
//  int dimidsp[ndimsp];
//  nc_inq_vardimid(ncid,val_id, &dimidsp[0]);
//  std::cout << "val_dim_ids " << dimidsp[0] << " " << dimidsp[1]<< std::endl;
//
//  size_t val_size=0;
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
//    std::vector<P> coord(lenp);
//    size_t startp=0;
//    nc_get_vara_double (ncid, dim_var_id, &startp, &lenp, &coord[0]);
//    //for (auto it = coord.cbegin(); it != coord.cend(); it++)
//    //{
//    //  std::cout << *it << ' ';
//    //}
//    if(ndimsp == 2)
//    {
//      if(i==0)
//      {
//        Field::x=coord;
//      }
//      else if(i==1)
//      {
//        Field::z=coord;
//      }
//    }
//
//  }
//  Field::nx = x.size();
//  Field::dx = x[1] - x[0];
//  Field::x_start = x.front();
//  Field::x_end = x.back();
//  Field::nz = z.size();
//  Field::dz = z[1] - z[0];
//  Field::z_start = z.front();
//  Field::z_end = z.back();
//  std::vector<P> val(val_size);
//  size_t startp=0;
//  nc_get_var_double (ncid, val_id, &val[0]);
//  Field::value = val;
//  std::cout << "val_id " << val_id  << std::endl;
//  std::cout << "val_size " << startp << " " << val_size << std::endl;
//  //for(int i=0 ;i<val_size; i++)
//  //{
//  //  std::cout << val[i] << " " ;
//  //}
//  nc_close(ncid);
//
//}
P Field2d_xz::interpolate_field(P x_point,P y_point, P z_point){
  int x_index = std::floor((x_point - Field2d_xz::x_start + 0.5*Field2d_xz::dx)/Field2d_xz::dx);
  int z_index = std::floor((z_point - Field2d_xz::z_start + 0.5*Field2d_xz::dz)/Field2d_xz::dz);
  int value_index = x_index*Field2d_xz::nz + z_index;
  std::cout << " x_ind z_ind val_ind " << x_index << " " << z_index << " " << value_index << std::endl;
  P interpolated_value= Field2d_xz::value[value_index];
  return interpolated_value;
}

P Field2d_rz::interpolate_field(P x_point,P y_point, P z_point){
  //P r_point = sqrt(x_point*x_point + y_point*y_point);
  P r_point = sqrt(x_point*x_point);// + y_point*y_point);
  int r_index = std::floor((r_point - Field2d_rz::r_start + 0.5*Field2d_rz::dr)/Field2d_rz::dr);
  int z_index = std::floor((z_point - Field2d_rz::z_start + 0.5*Field2d_rz::dz)/Field2d_rz::dz);
  int value_index = r_index*Field2d_rz::nz + z_index;
  std::cout << " x_ind z_ind val_ind " << r_index << " " << z_index << " " << value_index << std::endl;
  P interpolated_value= Field2d_rz::value[value_index];
  return interpolated_value;
}
