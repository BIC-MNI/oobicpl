#ifndef __BICVOLUME__
#define __BICVOLUME__

extern "C" {
#include "bicpl.h"
#include "volume_io.h"
}

#include <iostream.h>

class bicVolume {
protected:
  Volume       volume;
  int          sizes[MAX_DIMENSIONS];
  int          nDimensions;
  char*        dimNames[3];
  STRING       filename;
  nc_type      dataType;
  Real         voxelMin;
  Real         voxelMax;
  BOOLEAN      signedFlag;
public:
  //! File loading exception
  class loadException { };
  class writeException { };

  //! Empty Constructor
  bicVolume() { };
  //! Constructor from file
  bicVolume(STRING filename, Real voxelMin = -1, Real voxelMax = -1,
            int nDimensions = 3,
            char* dim1 = MIzspace, char* dim2 = MIxspace, 
            char* dim3 = MIyspace,
            nc_type dataType = NC_UNSPECIFIED, 
            BOOLEAN volumeSigned = FALSE,
            BOOLEAN createVolume = TRUE, 
            minc_input_options *options = NULL
            );
  //! Set the filename
  void setFilename(STRING file) { filename = file; };
  //! Return pointer to volume_io volume
  Volume *getVolume() { return &this->volume; };
  //! Get pointer to volume sizes
  int *getSizes() { return this->sizes; };
  //! Get voxel value
  virtual Real getVoxel(int v1, int v2, int v3, int v4=0, int v5=0);
  //! Set voxel value
  virtual void setVoxel(Real value, int v1, int v2, int v3,
			int v4=0, int v5=0);
  virtual void output() { };
  virtual void output(STRING file);
};

#endif // __BICVOLUME__
