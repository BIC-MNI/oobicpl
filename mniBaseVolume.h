#ifndef __MNIBASEVOLUME__
#define __MNIBASEVOLUME__

extern "C" {
#include "bicpl.h"
#include "volume_io.h"
}

#include <iostream.h>

// static value used as default for volume loading
static STRING  ZXYdimOrder[] = {MIzspace, MIxspace, MIyspace};

//! An abstract baseclass for a minc volume

class mniBaseVolume {
protected:
  Volume       volume;
  int          *sizes; 
  int          nDimensions;
  STRING*      dimNames;
  STRING       filename;
  nc_type      dataType;
  Real         voxelMin;
  Real         voxelMax;
  BOOLEAN      signedFlag;

public:
  //! Load exception class
  class loadException { };
  //! Write exception class
  class writeException { };
  //! Set the filename
  void setFilename(STRING file) { filename = file; };
  //! Return pointer to volume_io volume
  Volume getVolume() { return this->volume; };
  //! Get pointer to volume sizes
  int* getSizes() { return this->sizes; };
  //! Get one size from sizes array
  int getSize(int index) { return this->sizes[index]; };
  //! Get dimensions names
  STRING *getDimNames() { return this->dimNames; };
  //! Return volume min
  Real getVoxelMin() { return this->voxelMin; };
  //! Retrun volume max
  Real getVoxelMax() { return this->voxelMax; };
  //! Return signed flag
  BOOLEAN getSignedFlag() { return this->signedFlag; };
  //! Return data type
  nc_type getDataType() { return this->dataType; };
  //! Output the volume
  virtual void output(STRING file, int cropValue = 0) = 0;
};

#endif
