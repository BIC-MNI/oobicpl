#include "bicVolume.h"

// Constructor from file
bicVolume::bicVolume(STRING filename, 
		     Real voxelMin = 0.0, 
		     Real voxelMax = 0.0,
		     int nDimensions = 3,
		     STRING dimensions[] = ZXYdimOrder,
		     nc_type dataType = NC_UNSPECIFIED, 
		     BOOLEAN volumeSigned = FALSE,
		     BOOLEAN createVolume = TRUE, 
		     minc_input_options *options = NULL
                     ) {

  if ( input_volume(filename, nDimensions, dimensions, dataType, 
                    volumeSigned, voxelMin,
                    voxelMax, createVolume, &this->volume, options) != OK ) {
    throw loadException();
  }

  get_volume_sizes(this->volume, this->sizes);
  this->nDimensions = nDimensions;
  this->filename = filename;
  this->dimNames = dimensions;
  this->voxelMin = voxelMin;
  this->voxelMax = voxelMax;
  this->dataType = dataType;
  this->signedFlag = volumeSigned;
}

bicVolume::~bicVolume() {
  delete_volume(this->volume);
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

bicVolume& bicVolume::operator+(bicVolume *a, bicVolume *b) {
  // check to make sure that sizes are the same
  int *a_sizes = a->getSizes();
  int *b_sizes = b->getSizes();
  if (a_sizes[0] != b_sizes[0] ||
      a_sizes[1] != b_sizes[1] || 
      a_sizes[2] != b_sizes[2])
    throw differentSizesException();

  for (int v1 = 0; v1 < a_sizes[0]; v1++) {
    for (int v2 = 0; v2 < a_sizes[1]; v2++) {
      for (int v3 = 0; v3 < a_sizes[2]; v3++) {
	
