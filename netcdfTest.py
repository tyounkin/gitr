import netCDF4
import numpy as np
import scipy
import scipy.interpolate

def netcdfFile():
    nR = 100;
    nZ = 200;

    r = np.linspace(0,1,nR)
    z = np.linspace(-1,1,nZ)
    r_meshgrid, z_meshgrid = np.meshgrid(r,z) #, indexing='xy')
    r_meshgrid = np.transpose(r_meshgrid)
    z_meshgrid = np.transpose(z_meshgrid)
    val = 2*r_meshgrid + z_meshgrid
    print('val size ', val.shape)
    rootgrp = netCDF4.Dataset("netcdf_file_py.nc", "w", format="NETCDF4")
    nr_nc = rootgrp.createDimension("r", len(r))
    nz_nc = rootgrp.createDimension("z", len(z))
    ny_nc = rootgrp.createDimension("y", 123)
    val_nc = rootgrp.createVariable("val","f8",("r","z"))
    r_nc = rootgrp.createVariable("r","f8",("r"))
    z_nc = rootgrp.createVariable("z","f8",("z"))
    val_nc[:] = val
    r_nc[:] = r
    z_nc[:] = z
    rootgrp.close()
    points_tuple = (r,z)
    xi_tuple = (0.2,0.5)
    interp_val = scipy.interpolate.interpn(points_tuple,val,xi_tuple)
    print("interp_val ", interp_val)
if __name__ == "__main__": 
    netcdfFile()
