#ifndef __MNIBASEVOLUME__
#define __MNIBASEVOLUME__

extern "C" {
#include "bicpl.h"
#include "volume_io.h"
}

#include <iostream.h>



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
  //! Gets interpolated value at indices
  /*!
    \bug Use with caution - the returned Real argument ought to be an array,
    since in some situations the underlying volume_io function is supposed
    to return more than one value. But I don't quite (yet) understand when
    and how this is supposed to happen.
  */
  Real getInterpolatedVoxel(Real indices[],
			    int degreesContinuity=2,
			    BOOLEAN interpolatingDimensions[]=NULL,
			    int useLinearAtEdge=TRUE,
			    Real outsideValue=0,
			    Real **firstDerivative=NULL,
			    Real ***secondDerivative=NULL) {
    Real tmpReturnValue;
    evaluate_volume(this->volume,
		    indices,
		    interpolatingDimensions,
		    degreesContinuity,
		    useLinearAtEdge,
		    outsideValue,
		    &tmpReturnValue,
		    firstDerivative,
		    secondDerivative);
    return tmpReturnValue;
  };
  //! Overloaded version of getInterpolatedVoxel
  Real getInterpolatedVoxel(Real v1, Real v2, Real v3,
			    int degreesContinuity=2,
			    BOOLEAN interpolatingDimensions[]=NULL,
			    int useLinearAtEdge=TRUE,
			    Real outsideValue=0,
			    Real **firstDerivative=NULL,
			    Real ***secondDerivative=NULL) {
    Real tmpReturnValue;
    Real indices[3] = {v1, v2, v3};
    evaluate_volume(this->volume,
		    indices,
		    interpolatingDimensions,
		    degreesContinuity,
		    useLinearAtEdge,
		    outsideValue,
		    &tmpReturnValue,
		    firstDerivative,
		    secondDerivative);
    return tmpReturnValue;
  };
  

  //! Output the volume
  virtual void output(STRING file, int cropValue = 0) = 0;
};

#endif
