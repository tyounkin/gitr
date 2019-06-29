#pragma once
#include <vector>

class Field2d_rz
{
  typedef double P;
  private:
    //int n_dimensions;
    //std::vector<P> x;
    //std::vector<P> y;
    //std::vector<P> z;
    //std::vector<P> value;
  public:
    int n_dimensions;
    int nr;
    int nz;
    P dr;
    P dz;
    P r_start;
    P r_end;
    P z_start;
    P z_end;
    std::vector<P> r;
    std::vector<P> z;
    std::vector<P> value;
    Field2d_rz();
    Field2d_rz(libconfig::Config &cfg,std::string);
    P interpolate_field(P x_point,P y_point, P z_point);
};

class Field2d_xz
{
  typedef double P;
  private:
    //int n_dimensions;
    //std::vector<P> x;
    //std::vector<P> y;
    //std::vector<P> z;
    //std::vector<P> value;
  public:
    int n_dimensions;
    int nx;
    int nz;
    P dx;
    P dz;
    P x_start;
    P x_end;
    P z_start;
    P z_end;
    std::vector<P> x;
    std::vector<P> y;
    std::vector<P> z;
    std::vector<P> value;
    Field2d_xz();
    Field2d_xz(libconfig::Config &cfg,std::string);
    P interpolate_field(P x_point,P y_point, P z_point);
};

//class Field
//{
//  typedef double P;
//  private:
//    //int n_dimensions;
//    //std::vector<P> x;
//    //std::vector<P> y;
//    //std::vector<P> z;
//    //std::vector<P> value;
//  public:
//    int n_dimensions;
//    int nx;
//    int nz;
//    P dx;
//    P dz;
//    P x_start;
//    P x_end;
//    P z_start;
//    P z_end;
//    std::vector<P> x;
//    std::vector<P> y;
//    std::vector<P> z;
//    std::vector<P> value;
//    Field();
//    Field(libconfig::Config &cfg,std::string);
//    P interpolate_field(P x, P z);
//};
