#ifndef __MNILABELVOLUME__
#define __MNILABELVOLUME__

extern "C" {
#include "bicpl.h"
#include "volume_io.h"
}

#include <iostream.h>
#include "mniVolumeExceptions.h"
#include "mniVolume.h"

class mniLabelVolume {
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
  mniLabelVolume();
  //! Constructor from file, creating initialised volume
  mniLabelVolume(STRING filename, 
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
  mniLabelVolume(STRING filename,
		 int newVolume,
		 int nDimensions = 3,
		 STRING dimensions[] = ZXYdimOrder,
		 nc_type dataType = NC_UNSPECIFIED,
		 minc_input_options *options = NULL);
  //! Copy constructor from mniVolume
  mniLabelVolume(mniVolume *copyVolume, nc_type dataType = NC_SHORT);
  //! Copy constructor from mniLabelVolume
  mniLabelVolume(mniLabelVolume *copyVolume, nc_type dataType = NC_SHORT);

  virtual ~mniLabelVolume();

  //! Set the filename
  void setFilename(STRING file) { filename = file; };
  //! Return pointer to volume_io volume
  Volume getVolume() { return this->volume; };
  //! Get pointer to volume sizes
  int* getSizes() { return this->sizes; };
  //! Get dimensions names
  STRING *getDimNames() { return this->dimNames; };


  void setAllVoxels(int value) { 
    set_all_volume_label_data(this->volume, value); };
  int getVoxel(int v1, int v2, int v3, int v4=0, int v5=0);
  int getVoxel(int indices[3]);
  void setVoxel(int value, int v1, int v2, int v3, 
                        int v4=0, int v5=0);
  void setVoxel(int value, int indices[3]);
  void output(STRING file);
};

#endif // __MNILABELVOLUME__

