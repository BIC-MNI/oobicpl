#include "mniVolume.h"

// blank constructor - just initialises the sizes for now
mniVolume::mniVolume() {
  this->sizes = new int[MAX_DIMENSIONS];
}

// Constructor from file
mniVolume::mniVolume(STRING filename, 
                     Real voxelMin = 0.0, 
                     Real voxelMax = 0.0,
                     int nDimensions = 3,
                     STRING dimensions[] = ZXYdimOrder,
                     nc_type dataType = NC_UNSPECIFIED, 
                     BOOLEAN volumeSigned = FALSE,
                     BOOLEAN createVolume = TRUE, 
                     minc_input_options *options = NULL ) {
  if ( input_volume(filename, nDimensions, dimensions, dataType, 
                    volumeSigned, voxelMin,
                    voxelMax, createVolume, &this->volume, options) != OK ) {
    throw loadException();
  }

  this->sizes = new int[MAX_DIMENSIONS];
  get_volume_sizes(this->volume, this->sizes);
  this->nDimensions = nDimensions;
  this->filename = filename;
  this->dimNames = dimensions;
  this->voxelMin = voxelMin;
  this->voxelMax = voxelMax;
  this->dataType = dataType;
  this->signedFlag = volumeSigned;

}

mniVolume::mniVolume(mniBaseVolume *copyVolume, 
		     BOOLEAN copyVolumeDefinitionOnly=FALSE,
		     nc_type dataType = NC_UNSPECIFIED,
		     BOOLEAN signedFlag = FALSE,
		     Real voxelMin = 0.0,
		     Real voxelMax = 0.0) {


  //initialise sizes
  this->sizes = new int[MAX_DIMENSIONS];
  
  // now copy relevant bits from other volume
  if (copyVolumeDefinitionOnly == TRUE) {
    this->volume = copy_volume_definition(copyVolume->getVolume(), dataType,
					  signedFlag, voxelMin, voxelMax);
    this->dataType = dataType;
    this->signedFlag = signedFlag;
    this->voxelMin = voxelMin;
    this->voxelMax = voxelMax;
  }
  else { // create an exact copy
    this->volume = copy_volume(copyVolume->getVolume());
    this->voxelMin = copyVolume->getVoxelMin();
    this->voxelMax = copyVolume->getVoxelMax();
    this->signedFlag = copyVolume->getSignedFlag();
    this->dataType = copyVolume->getDataType();
  }

    this->sizes = copyVolume->getSizes();
    this->dimNames = copyVolume->getDimNames();
}
  
  

mniVolume::~mniVolume() {
  delete_volume(this->volume);
  delete this->sizes;
}

inline Real mniVolume::getVoxel(int v1, int v2, int v3, int v4=0, int v5=0) {
  return get_volume_real_value(this->volume, v1, v2, v3, v4, v5);
}

inline Real mniVolume::getVoxel(int indices[3]) {
  return get_volume_real_value(this->volume, indices[0], indices[1],
			       indices[2], 0, 0);
}

Real mniVolume::getWorld(Real xWorld, Real yWorld, Real zWorld) {
  Real *voxelCoord;
  cout << "In get world" << endl;
  voxelCoord = this->convertWorldToVoxel(xWorld, yWorld, zWorld);
  cout << "Converted ... " << endl;
  return get_volume_real_value(this->volume, 
                               (int)rint(voxelCoord[0]), 
                               (int)rint(voxelCoord[1]),
                               (int)rint(voxelCoord[2]), 0, 0);
}
  

inline Real mniVolume::getInterpolatedVoxel(Real indices[],
					    int degreesContinuity=2,
					    BOOLEAN interpolatingDimensions[]=NULL,
					    int useLinearAtEdge=TRUE,
					    Real outsideValue=0,
					    Real **firstDerivative=NULL,
					    Real ***secondDerivative=NULL) {
  Real tmpReturnValue;
  evaluate_volume(this->volume,
		  indices,
		  interpolatingDimensions,
		  degreesContinuity,
		  useLinearAtEdge,
		  outsideValue,
		  &tmpReturnValue,
		  firstDerivative,
		  secondDerivative);
  return tmpReturnValue;
}

inline Real mniVolume::getInterpolatedVoxel(Real v1, Real v2, Real v3,
					    int degreesContinuity=2,
					    BOOLEAN interpolatingDimensions[]=NULL,
					    int useLinearAtEdge=TRUE,
					    Real outsideValue=0,
					    Real **firstDerivative=NULL,
					    Real ***secondDerivative=NULL) {
  Real tmpReturnValue;
  Real indices[3] = {v1, v2, v3};
  evaluate_volume(this->volume,
		  indices,
		  interpolatingDimensions,
		  degreesContinuity,
		  useLinearAtEdge,
		  outsideValue,
		  &tmpReturnValue,
		  firstDerivative,
		  secondDerivative);
  return tmpReturnValue;
}


inline void mniVolume::setVoxel(Real value, int v1, int v2, int v3,
                         int v4=0, int v5=0) {
  set_volume_real_value(this->volume, v1, v2, v3, v4, v5, value);
}

inline void mniVolume::setVoxel(Real value, int indices[3]) {
  set_volume_real_value(this->volume, indices[0], indices[1],
			indices[2], 0, 0, value);
}

void mniVolume::output(STRING file, int cropValue = 0) {
  if (output_volume(file, this->dataType, this->signedFlag,
                    this->voxelMin, this->voxelMax, this->volume,
                    "mnipl-- test", NULL) != OK) {
    throw writeException();
  }
}



/*
  mniVolume& mniVolume::operator+(mniVolume *a, mniVolume *b) {
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
