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
  //! Constructor from file, creating uninitialised volume
  bicLabelVolume(STRING filename, int newVolume,
            int nDimensions = 3,
            char* dim1 = MIzspace, 
            char* dim2 = MIxspace,
            char* dim3 = MIyspace, 
            minc_input_options *options = NULL);
  //! Constructor from file
  bicLabelVolume(STRING filename);
  virtual Real getVoxel(int v1, int v2, int v3, int v4=0, int v5=0);
  virtual void setVoxel(Real value, int v1, int v2, int v3, 
                        int v4=0, int v5=0);
  virtual void output(STRING file);
};

#endif // __BICLABELVOLUME__

