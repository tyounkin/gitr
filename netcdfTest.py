import netCDF4
import numpy as np
def netcdfFile():
    nR = 100;
    nZ = 200;

    r = np.linspace(0,1,nR)
    z = np.linspace(-1,1,nZ)
    r_meshgrid, z_meshgrid = np.meshgrid(r,z)
    val = 2*r_meshgrid + z_meshgrid
    print('val size ', val.shape)
    rootgrp = netCDF4.Dataset("netcdf_file_py.nc", "w", format="NETCDF4")
    nr_nc = rootgrp.createDimension("r", len(r))
    nz_nc = rootgrp.createDimension("z", len(z))
    ny_nc = rootgrp.createDimension("y", 123)
    val_nc = rootgrp.createVariable("val","f8",("z","r"))
    r_nc = rootgrp.createVariable("r","f8",("r"))
    z_nc = rootgrp.createVariable("z","f8",("z"))
    val_nc[:] = val
    r_nc[:] = r
    z_nc[:] = z
    rootgrp.close()
if __name__ == "__main__": 
    netcdfFile()
