// A series of test cases

#include <iostream.h>
#include "mniVolume.h"
#include "mniLabelVolume.h"

int main(int argc, char* argv[]) {
  mniVolume *volume;

  STRING avg305file = "/usr/local/mni/data/mni_autoreg/average_305.mnc";

  cout << "Testing volume instantiation ..." << endl;
  cout << "Assuming the existence of " << avg305file << endl;

  volume = new mniVolume(avg305file);

  Real testv[3] = {1,2,3};
  Real *something = volume->convertVoxelToWorld(testv);
  
  cout << "before conversion" << endl;
  Real *conversion = volume->convertWorldToVoxel(-23, -17, 11);
  cout << "after conversion: " << conversion[0] 
       << " " << conversion[1] << " " << conversion[2] << endl;

  cout << "test: " << volume->getVoxel(conversion[0], conversion[1],
                                       conversion[2]) << endl;
  cout << "Testing world value: " << volume->getWorld(-23, -17, 11)
       << endl;
}

  
