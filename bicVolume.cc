#include "bicVolume.h"

// Constructor from file
bicVolume::bicVolume(STRING filename, Real voxelMin = -1, Real voxelMax = -1,
                     int nDimensions = 3,
                     char* dim1 = MIzspace, char* dim2 = MIxspace, 
                     char* dim3 = MIyspace,
                     nc_type dataType = NC_UNSPECIFIED, 
                     BOOLEAN volumeSigned = FALSE,
                     BOOLEAN createVolume = TRUE, 
                     minc_input_options *options = NULL
                     ) {


  char* dims[] = {dim1, dim2, dim3};
  // code to figure out reasonable voxelMin and voxelMax should go here
  if (voxelMin == -1)
    voxelMin = 0;
  if (voxelMax == -1)
    voxelMax = 10;

  if ( input_volume(filename, nDimensions, dims, dataType, 
                    volumeSigned, voxelMin,
                    voxelMax, createVolume, &this->volume, options) != OK ) {
    throw loadException();
  }

  get_volume_sizes(this->volume, this->sizes);
  this->nDimensions = nDimensions;
  this->filename = filename;
  this->dimNames = dims;
  this->voxelMin = voxelMin;
  this->voxelMax = voxelMax;
  this->dataType = dataType;
  this->signedFlag = volumeSigned;
}

inline Real bicVolume::getVoxel(int v1, int v2, int v3, int v4=0, int v5=0) {
  return get_volume_real_value(this->volume, v1, v2, v3, v4, v5);
}

inline void bicVolume::setVoxel(Real value, int v1, int v2, int v3,
                         int v4=0, int v5=0) {
  set_volume_real_value(this->volume, v1, v2, v3, v4, v5, value);
}

void bicVolume::output(STRING file) {
  if (output_volume(file, this->dataType, this->signedFlag,
                    this->voxelMin, this->voxelMax, this->volume,
                    "bicpl-- test", NULL) != OK) {
    throw writeException();
  }
}
