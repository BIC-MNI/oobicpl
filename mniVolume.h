#ifndef __MNIVOLUME__
#define __MNIVOLUME__

extern "C" {
#include "bicpl.h"
#include "volume_io.h"
}

#include <iostream.h>
#include "mniBaseVolume.h"

//! A class for working with minc volumes
/*!

  This class uses the standard volume_io functions for handling
  volumes.  It should be used for most cases ... at least until
  classes which bypass volume_io come along.

  \todo Create a constructor for creating a new volume without reading
  header information from a file
*/
class mniVolume : public mniBaseVolume {

public:
  //! Empty Constructor
  /*!  
  An empty constructor - this constructor really should not be used by
  any calling code, as all that it does is initialiset the sizes
  variable. Moreover, the code to create a volume from scratch is not
  yet present in this class
  */

  mniVolume();
  //! Constructor from file
  /*!
  A constructor which initialises the class by reading in the volume
  information from the filename provided. The other arguments all have
  default values that can usually be left untouched.

  \exception loadException Thrown should there be an error reading the
  file
  */

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
  /*!
  Copies the volume definition, including the volume data itself, from
  the volume provided.
  */
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
