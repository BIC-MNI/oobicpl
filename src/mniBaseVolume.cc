#include "mniBaseVolume.h"

Real* mniBaseVolume::convertVoxelToWorld(Real voxel[]) {
  Real *returnValue = new Real[3];
  convert_voxel_to_world(this->volume, 
                         voxel, 
                         &returnValue[0],
                         &returnValue[1],
                         &returnValue[2]);
  
  return returnValue;
}

Real* mniBaseVolume::convertWorldToVoxel(Real xWorld,
                                         Real yWorld,
                                         Real zWorld) 
{
  Real *voxel = new Real[3];
  convert_world_to_voxel(this->volume, xWorld, yWorld, zWorld, voxel);
  return voxel;
}

