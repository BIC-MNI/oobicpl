#include "mniLabelVolume.h"

mniLabelVolume::mniLabelVolume(STRING filename, 
			       Real voxelMin, 
			       Real voxelMax,
			       int nDimensions,
			       STRING dimensions[],
			       nc_type dataType,
			       BOOLEAN volumeSigned,
			       BOOLEAN createVolume,
			       minc_input_options *options) {

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

  get_volume_sizes(this->volume, this->sizes);
                               
}
                               
mniLabelVolume::mniLabelVolume(mniBaseVolume *copyVolume, 
			       nc_type dataType ) {

  // initialise sizes
  this->sizes = new int[MAX_DIMENSIONS];

  // now copy all the relevant bits from the other volume
  this->volume = create_label_volume(copyVolume->getVolume(), dataType);
  this->dimNames = copyVolume->getDimNames();
  *this->sizes = *copyVolume->getSizes(); // copy by value
  this->dataType = dataType;
}

/*
mniLabelVolume::mniLabelVolume(mniLabelVolume *copyVolume,
			       nc_type dataType = NC_SHORT) {

  // initialise sizes
  this->sizes = new int[MAX_DIMENSIONS];

  // now copy all the relevant bits from the other volume
  this->volume = create_label_volume(copyVolume->getVolume(), dataType);
  this->dimNames = copyVolume->getDimNames();
  *this->sizes = *copyVolume->getSizes(); // copy by value
  this->dataType = dataType;
}
*/
mniLabelVolume::mniLabelVolume() {
  // initialise sizes
  this->sizes = new int[MAX_DIMENSIONS];
}

mniLabelVolume::mniLabelVolume(STRING filename,
			       int newVolume,
			       int nDimensions,
			       STRING dimensions[],
			       nc_type dataType,
			       minc_input_options *options){

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

mniLabelVolume::~mniLabelVolume() {
  delete_volume(this->volume);
  delete this->sizes;
}  

inline int mniLabelVolume::getVoxel(int v1, int v2, int v3, 
                                     int v4, int v5) {
  return get_volume_label_data_5d(this->volume, v1, v2, v3, v4, v5);
}

inline int mniLabelVolume::getVoxel(int indices[3]) {
  return get_volume_label_data(this->volume, indices);
}

inline void mniLabelVolume::setVoxel(int value, int v1, int v2, int v3,
                                     int v4, int v5) {
  set_volume_label_data_5d(this->volume, v1, v2, v3, v4, v5, value);
}

inline void mniLabelVolume::setVoxel(int value, int indices[3]) {
  set_volume_label_data(this->volume, indices, value);
}

void mniLabelVolume::output(STRING file, int cropValue) {
  // should replace the constant with an argument option
    cout << "FILENAME: " << this->filename << endl;
    save_label_volume(file, this->filename, this->volume, cropValue);
}

  
  
