#include "bicLabelVolume.h"

bicLabelVolume::bicLabelVolume(STRING filename, 
			       Real voxelMin = 0.0, 
			       Real voxelMax = 0.0,
			       int nDimensions = 3,
			       STRING dimensions[] = ZXYdimOrder,
			       nc_type dataType = NC_UNSPECIFIED, 
			       BOOLEAN volumeSigned = FALSE,
			       BOOLEAN createVolume = TRUE, 
			       minc_input_options *options = NULL) {

  // initialise the sizes variable
  this->sizes = new int[MAX_DIMENSIONS];

  // load the volume headers first
  if ( input_volume_header_only(filename, nDimensions, dimensions,
				&this->volume, options) != OK ) {
    throw loadException();
  }
  // now create it as a label volume
  if ( create_label_volume_from_file(filename, this->volume, &this->volume)
       != OK ) {
    throw loadException();
  }
    
                               
}
                               
bicLabelVolume::bicLabelVolume(bicVolume *copyVolume, 
			       nc_type dataType = NC_SHORT) {

  // initialise sizes
  this->sizes = new int[MAX_DIMENSIONS];

  // now copy all the relevant bits from the other volume
  this->volume = create_label_volume(copyVolume->getVolume(), dataType);
  this->dimNames = copyVolume->getDimNames();
  this->sizes = copyVolume->getSizes();
  this->dataType = dataType;
}

bicLabelVolume::bicLabelVolume(bicLabelVolume *copyVolume,
			       nc_type dataType = NC_SHORT) {

  // initialise sizes
  this->sizes = new int[MAX_DIMENSIONS];

  // now copy all the relevant bits from the other volume
  this->volume = create_label_volume(copyVolume->getVolume(), dataType);
  this->dimNames = copyVolume->getDimNames();
  this->sizes = copyVolume->getSizes();
  this->dataType = dataType;
}

bicLabelVolume::bicLabelVolume() {
  // initialise sizes
  this->sizes = new int[MAX_DIMENSIONS];
}

bicLabelVolume::bicLabelVolume(STRING filename,
			       int newVolume,
			       int nDimensions = 3,
			       STRING dimensions[] = ZXYdimOrder,
			       nc_type dataType = NC_UNSPECIFIED,
			       minc_input_options *options = NULL){

  // initialise sizes
  this->sizes = new int[MAX_DIMENSIONS];

  if (input_volume_header_only(filename, nDimensions, dimensions,
                               &this->volume, options) != OK) {
    throw loadException();
  }
  this->volume = create_label_volume(this->volume, dataType);

  get_volume_sizes(this->volume, this->sizes);
  this->nDimensions = nDimensions;
  this->dimNames = dimensions;
  this->filename = filename;
}

bicLabelVolume::~bicLabelVolume() {
  delete_volume(this->volume);
  delete this->sizes;
}  

inline int bicLabelVolume::getVoxel(int v1, int v2, int v3, 
                                     int v4=0, int v5=0) {
  return get_volume_label_data_5d(this->volume, v1, v2, v3, v4, v5);
}

inline int bicLabelVolume::getVoxel(int indices[3]) {
  return get_volume_label_data(this->volume, indices);
}

inline void bicLabelVolume::setVoxel(int value, int v1, int v2, int v3,
                                     int v4=0, int v5=0) {
  set_volume_label_data_5d(this->volume, v1, v2, v3, v4, v5, value);
}

inline void bicLabelVolume::setVoxel(int value, int indices[3]) {
  set_volume_label_data(this->volume, indices, value);
}

void bicLabelVolume::output(STRING file) {
  // should replace the constant with an argument option
    cout << "FILENAME: " << this->filename << endl;
    save_label_volume(file, this->filename, this->volume, 255);
}

  
  
