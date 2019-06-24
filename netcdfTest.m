nR = 100;
nZ = 200;
r = linspace(0,1,nR);
z = linspace(-1,1,nZ);
val = zeros(nR,nZ);
for i=1:nR
    for j=1:nZ
        val(i,j) = 2*i + j;
    end
end

h = pcolor(r,z,val')
h.EdgeColor = 'none';
colorbar
% % set(gca, 'YDir', 'normal')
%  set(gca, 'XScale', 'log')
title('2d val plot')
xlabel('r') % x-axis label
ylabel('z') % y-axis label
set(gca,'fontsize',16)

ncid = netcdf.create(['./example_file.nc'],'NC_WRITE')

dimR = netcdf.defDim(ncid,'nR',nR);
dimZ = netcdf.defDim(ncid,'nZ',nZ);

gridRnc = netcdf.defVar(ncid,'gridR','double',dimR);
gridZnc = netcdf.defVar(ncid,'gridZ','double',dimZ);
valnc = netcdf.defVar(ncid,'val','double',[dimR dimZ]);
netcdf.endDef(ncid);

netcdf.putVar(ncid,gridRnc,r);
netcdf.putVar(ncid,gridZnc,z);
netcdf.putVar(ncid,valnc,val);
netcdf.close(ncid);