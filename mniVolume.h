#ifndef __MNIVOLUME__
#define __MNIVOLUME__

extern "C" {
#include "bicpl.h"
#include "volume_io.h"
}

#include <iostream.h>
#include "mniBaseVolume.h"

class mniVolume : public mniBaseVolume {

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
  //! Copy constructor from mniVolume
  mniVolume(mniBaseVolume *copyVolume, nc_type dataType = NC_UNSPECIFIED);

  //! Destructor to free memory
  virtual ~mniVolume();

  //! Get voxel value
  Real getVoxel(int v1, int v2, int v3, int v4=0, int v5=0);
  //! Overloaded getVoxel, taking three dimensional array
  Real getVoxel(int indices[3]);
  //! Set voxel value
  void setVoxel(Real value, int v1, int v2, int v3,
                        int v4=0, int v5=0);
  //! Overloaded setVoxel, taking three dimensional array
  void setVoxel(Real value, int indices[3]);
  //  virtual void output() { };
  virtual void output(STRING file, int cropValue = 0);


};

#endif // __MNIVOLUME__
