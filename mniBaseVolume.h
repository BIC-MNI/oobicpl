#ifndef __MNIBASEVOLUME__
#define __MNIBASEVOLUME__

extern "C" {
#include "bicpl.h"
#include "volume_io.h"
}

#include <iostream.h>

/*! \mainpage The Object Oriented BIC Programming Library

OOBICPL is, at the moment, a series of C++ classes to wrap the
volume_handling functionality of volume_io and bicpl. The main goal is
therefore programming convenience and elegance, achieved mainly
through the use of data encapsulation, function overloading, and
default arguments to functions.

\section Volume classes overview

The OOBICPL contains three classes for working with volumes. The first
is an abstract base class, mniBaseVolume. One can never work directly
with this class, only derive other classes from it. It contains all
functionality for dealing with associated data, such as volume sizes,
dimensions, etc., but not for handling the voxels themselves.

Two classes inherit from mniBaseVolume: the first is mniVolume, which
wraps the volume handling code to be found in volume_io. This class
should be used for just about every situation except for dealing with
label volumes. For that purpose the other derived class,
mniLabelVolume, ought to be used, as it wraps the label volume
functionality which can be found in the bicpl.

\section Further work

As it stands right now, this is clearly just a beginning. The next
goal will be to write a template class for dealing with volumes in
their native data-type. Such a class would either use volume_io to
extract all of the imaging information and place it into an array, or
it would work with the minc format directly. The second option is
clearly preferable, but also involves more work!

Furthermore, code to handle tag file and obj files ought to also be
included in this type of a library. And lastly, the featureset has to
be fleshed out more, as many things are still missing.


*/


// static value used as default for volume loading
static STRING  ZXYdimOrder[] = {MIzspace, MIxspace, MIyspace};
static STRING  ZYXdimOrder[] = {MIzspace, MIyspace, MIxspace};

static STRING  XYZdimOrder[] = {MIxspace, MIyspace, MIzspace};
static STRING  XZYdimOrder[] = {MIxspace, MIzspace, MIyspace};

static STRING  YXZdimOrder[] = {MIyspace, MIxspace, MIzspace};
static STRING  YZXdimOrder[] = {MIyspace, MIzspace, MIxspace};

//! An abstract baseclass for a minc volume
/*!

This is an abstract class, meaning that no members of this type can
ever be created. One of the derived classes, mniVolume or
mniLabelVolume has to be used instead.

\todo Incorporate the voxel to world transformation info

*/

class mniBaseVolume {
protected:
  //! Holds the volume_io volume
  Volume       volume;
  //! Holds the sizes - has to be instantiated before use
  int          *sizes; 
  //! Holds the number of dimensions
  int          nDimensions;
  //! Holds the dimension order
  STRING*      dimNames;
  //! Holds the original filename - not yet used
  STRING       filename;
  //! Holds the minc data type
  nc_type      dataType;
  //! Holds the minimum voxel value
  Real         voxelMin;
  //! Holds the maximum voxel value
  Real         voxelMax;
  //! Whether the data type is signed or not
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
  //! Set the volume real range
  void setRealRange(Real lower, Real upper) { set_volume_real_range(
                                          this->volume, lower, upper); }
  //! Return signed flag
  BOOLEAN getSignedFlag() { return this->signedFlag; };
  //! Return data type
  nc_type getDataType() { return this->dataType; };

  // voxel to world coordinate stuff:

  //!converts a voxel to world space
  /*!
    Returns a voxel to from voxel space to world space
    \param voxel[] An array holding the voxel to be converted
    \return An array holding the world coordinates in X Y Z order
    \note You have to free the memory of the returned array yourself
  */
  Real* convertVoxelToWorld(Real voxel[]);
  //! Convert a world coordinate into a voxel
  /*!
    \return An array holding the voxel coordinates
  */
  Real* convertWorldToVoxel(Real xWorld, Real yWorld, Real zWorld);
  

  //! Output the volume
  virtual void output(STRING file, int cropValue = 0) = 0;
};

#endif
