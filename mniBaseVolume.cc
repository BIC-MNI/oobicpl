#include "mniBaseVolume.h"

Real* mniBaseVolume::convertVoxelToWorld(Real voxel[]) {
  Real xWorld, yWorld, zWorld;
  convert_voxel_to_world(this->volume, voxel, &xWorld, &yWorld, &zWorld);
  
  Real returnValue[3] = {xWorld, yWorld, zWorld};
  return returnValue;
}

Real* mniBaseVolume::convertWorldToVoxel(Real xWorld,
                                                Real yWorld,
                                                Real zWorld) {
  Real *voxel;
  convert_world_to_voxel(this->volume, xWorld, yWorld, zWorld, voxel);
  return voxel;
}
