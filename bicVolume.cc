#include "bicVolume.h"

// blank constructor - just initialises the sizes for now
bicVolume::bicVolume() {
  this->sizes = new int[MAX_DIMENSIONS];
}

// Constructor from file
bicVolume::bicVolume(STRING filename, 
                     Real voxelMin = 0.0, 
                     Real voxelMax = 0.0,
                     int nDimensions = 3,
                     STRING dimensions[] = ZXYdimOrder,
                     nc_type dataType = NC_UNSPECIFIED, 
                     BOOLEAN volumeSigned = FALSE,
                     BOOLEAN createVolume = TRUE, 
                     minc_input_options *options = NULL ) {


  cout << "In bicVolume constructor" << endl;
  cout << "Constructor details: " << filename << " " 
       << dimensions[0] << " " << endl;
  if ( input_volume(filename, nDimensions, dimensions, dataType, 
                    volumeSigned, voxelMin,
                    voxelMax, createVolume, &this->volume, options) != OK ) {
    throw loadException();
  }
  cout << "just a test" << endl;
  this->sizes = new int[MAX_DIMENSIONS];
  get_volume_sizes(this->volume, this->sizes);
  this->nDimensions = nDimensions;
  this->filename = filename;
  this->dimNames = dimensions;
  this->voxelMin = voxelMin;
  this->voxelMax = voxelMax;
  this->dataType = dataType;
  this->signedFlag = volumeSigned;

  cout << "end of constructor" << endl;
}

bicVolume::~bicVolume() {
  delete_volume(this->volume);
  delete this->sizes;
}

inline Real bicVolume::getVoxel(int v1, int v2, int v3, int v4=0, int v5=0) {
  return get_volume_real_value(this->volume, v1, v2, v3, v4, v5);
}

inline Real bicVolume::getVoxel(int indices[3]) {
  return get_volume_real_value(this->volume, indices[0], indices[1],
			       indices[2], 0, 0);
}

inline void bicVolume::setVoxel(Real value, int v1, int v2, int v3,
                         int v4=0, int v5=0) {
  set_volume_real_value(this->volume, v1, v2, v3, v4, v5, value);
}

inline void bicVolume::setVoxel(Real value, int indices[3]) {
  set_volume_real_value(this->volume, indices[0], indices[1],
			indices[2], 0, 0, value);
}

void bicVolume::output(STRING file) {
  if (output_volume(file, this->dataType, this->signedFlag,
                    this->voxelMin, this->voxelMax, this->volume,
                    "bicpl-- test", NULL) != OK) {
    throw writeException();
  }
}

/*
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
*/
