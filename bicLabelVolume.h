#ifndef __BICLABELVOLUME__
#define __BICLABELVOLUME__

extern "C" {
#include "bicpl.h"
#include "volume_io.h"
}

#include "bicVolume.h"
#include <iostream.h>

class bicLabelVolume : public bicVolume {
public:
  //! Empty Constructor
  bicLabelVolume() { };
  //! Constructor from file, creating initialised volume
  bicLabelVolume(STRING filename, 
		 Real voxelMin = 0.0, 
		 Real voxelMax = 0.0,
		 int nDimensions = 3,
		 STRING dimensions[] = ZXYdimOrder,
		 nc_type dataType = NC_UNSPECIFIED, 
		 BOOLEAN volumeSigned = FALSE,
		 BOOLEAN createVolume = TRUE, 
		 minc_input_options *options = NULL
		 );
  //! Constructor from file, creating uninitialised volume
  /*!
    Creates a label volume with the same parameters as the filename
    passed in.
    \param newVolume Itself of no use, only here to differentiate it from
    the other constructors for function overloading.
  */
  bicLabelVolume(STRING filename,
		 int newVolume,
		 int nDimensions = 3,
		 STRING dimensions[] = ZXYdimOrder,
		 nc_type dataType = NC_UNSPECIFIED,
		 minc_input_options *options = NULL);
  //! Copy constructor, creates uninitialised label volume
  bicLabelVolume(bicVolume *copyVolume, nc_type dataType = NC_LONG);

  virtual void setAllVoxels(Real value) { 
    set_all_volume_label_data(this->volume, (int)value); };
  virtual Real getVoxel(int v1, int v2, int v3, int v4=0, int v5=0);
  virtual Real getVoxel(int indices[3]);
  virtual void setVoxel(Real value, int v1, int v2, int v3, 
                        int v4=0, int v5=0);
  virtual void setVoxel(Real value, int indices[3]);
  virtual void output(STRING file);
};

#endif // __BICLABELVOLUME__

