#include "bicLabelVolume.h"

bicLabelVolume::bicLabelVolume(STRING filename, 
			       Real voxelMin = 0.0, 
			       Real voxelMax = 0.0,
			       int nDimensions = 3,
			       STRING dimensions[] = ZXYdimOrder,
			       nc_type dataType = NC_UNSPECIFIED, 
			       BOOLEAN volumeSigned = FALSE,
			       BOOLEAN createVolume = TRUE, 
			       minc_input_options *options = NULL) 
  : bicVolume(filename, voxelMin, voxelMax, nDimensions,
	      dimensions, dataType, volumeSigned, createVolume, options) {
  cout << "got this far ... " << endl;
  create_label_volume_from_file(filename, this->volume, &this->volume);
                               
}
                               
bicLabelVolume::bicLabelVolume(bicVolume *copyVolume, 
			       nc_type dataType = NC_LONG)
  : bicVolume() {
  this->volume = create_label_volume(copyVolume->getVolume(), dataType);
  this->dimNames = copyVolume->getDimNames();
  this->sizes = copyVolume->getSizes();
  this->dataType = dataType;
}

bicLabelVolume::bicLabelVolume(STRING filename,
			       int newVolume,
			       int nDimensions = 3,
			       STRING dimensions[] = ZXYdimOrder,
			       nc_type dataType = NC_UNSPECIFIED,
			       minc_input_options *options = NULL)
           : bicVolume() {

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

inline Real bicLabelVolume::getVoxel(int v1, int v2, int v3, 
                                     int v4=0, int v5=0) {
  return (Real) get_volume_label_data_5d(this->volume, v1, v2, v3, v4, v5);
}

inline Real bicLabelVolume::getVoxel(int indices[3]) {
  return (Real) get_volume_label_data(this->volume, indices);
}

inline void bicLabelVolume::setVoxel(Real value, int v1, int v2, int v3,
                                     int v4=0, int v5=0) {
  set_volume_label_data_5d(this->volume, v1, v2, v3, v4, v5, (int)value);
}

inline void bicLabelVolume::setVoxel(Real value, int indices[3]) {
  set_volume_label_data(this->volume, indices, (int)value);
}

void bicLabelVolume::output(STRING file) {
  // should replace the constant with an argument option
    cout << "FILENAME: " << this->filename << endl;
    save_label_volume(file, this->filename, this->volume, 255);
}

  
  
