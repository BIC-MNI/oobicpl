#ifndef __MNIVOLUME__
#define __MNIVOLUME__

extern "C" {
#include "bicpl.h"
#include "volume_io.h"
}

#include <iostream.h>
#include "mniVolumeExceptions.h"
// static value used as default for volume loading
static STRING  ZXYdimOrder[] = {MIzspace, MIxspace, MIyspace};


class mniVolume {
protected:
  Volume       volume;
  int          *sizes; // should be sizes[MAX_DIMENSIONS] ?
  int          nDimensions;
  STRING*      dimNames;
  STRING       filename;
  nc_type      dataType;
  Real         voxelMin;
  Real         voxelMax;
  BOOLEAN      signedFlag;

public:
  //! Empty Constructor
  mniVolume();
  //! Constructor from file
  mniVolume(STRING filename, 
	    Real voxelMin = 0.0, 
	    Real voxelMax = 0.0,
            int nDimensions = 3,
	    STRING dimensions[] = ZXYdimOrder,
            nc_type dataType = NC_UNSPECIFIED, 
            BOOLEAN volumeSigned = FALSE,
            BOOLEAN createVolume = TRUE, 
            minc_input_options *options = NULL
            );
  //! Destructor to free memory
  virtual ~mniVolume();
  //! Set the filename
  void setFilename(STRING file) { filename = file; };
  //! Return pointer to volume_io volume
  Volume getVolume() { return this->volume; };
  //! Get pointer to volume sizes
  int* getSizes() { return this->sizes; };
  //! Get dimensions names
  STRING *getDimNames() { return this->dimNames; };
  //! Get voxel value
  virtual Real getVoxel(int v1, int v2, int v3, int v4=0, int v5=0);
  //! Overloaded getVoxel, taking three dimensional array
  virtual Real getVoxel(int indices[3]);
  //! Set voxel value
  virtual void setVoxel(Real value, int v1, int v2, int v3,
			int v4=0, int v5=0);
  //! Overloaded setVoxel, taking three dimensional array
  virtual void setVoxel(Real value, int indices[3]);
  virtual void output() { };
  virtual void output(STRING file);

  //! + operator
  //  mniVolume& operator+(mniVolume *a, mniVolume *b);
};

#endif // __MNIVOLUME__
