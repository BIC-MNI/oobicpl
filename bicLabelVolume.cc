#include "bicLabelVolume.h"

bicLabelVolume::bicLabelVolume(STRING filename) :
  bicVolume(filename) {
  create_label_volume_from_file(filename, this->volume, &this->volume);
  
                               
}
                               
  

bicLabelVolume::bicLabelVolume(STRING filename, int newVolume,
                               int nDimensions = 3,
                               char* dim1 = MIzspace, 
                               char* dim2 = MIxspace,
                               char* dim3 = MIyspace, 
                               minc_input_options *options = NULL)
           : bicVolume() {

  char* dims[] = {dim1, dim2, dim3};
  if (input_volume_header_only(filename, nDimensions, dims,
                               &this->volume, options) != OK) {
    throw loadException();
  }
  this->volume = create_label_volume(this->volume, NC_BYTE);

  get_volume_sizes(this->volume, this->sizes);
  this->nDimensions = nDimensions;
  this->dimNames = dims;
  this->filename = filename;
}

inline Real bicLabelVolume::getVoxel(int v1, int v2, int v3, 
                                     int v4=0, int v5=0) {
  return (Real) get_volume_label_data_5d(this->volume, v1, v2, v3, v4, v5);
}

inline void bicLabelVolume::setVoxel(Real value, int v1, int v2, int v3,
                                     int v4=0, int v5=0) {
  set_volume_label_data_5d(this->volume, v1, v2, v3, v4, v5, (int)value);
}

void bicLabelVolume::output(STRING file) {
  // should replace the constant with an argument option
  cout << "FILENAME: " << this->filename << endl;
  save_label_volume(file, this->filename, this->volume, 255);
}

  
  
