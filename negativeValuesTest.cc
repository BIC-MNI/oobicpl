/*
 * Tests a bug found in writing out negative numbers
 *
 * NOTE: does not yet use correct formula to work with make check, and
 * will therefore always fail.
 */

#include "mniVolume.h"

void main (int argc, char* argv[]) {
  if (argc < 1) {
    cout << "USAGE: programme filename.mnc" << endl;
    exit(1);
  }

  mniVolume *volume = new mniVolume(argv[1],
                                   -100, 100,
                                   3, ZXYdimOrder,
                                   NC_BYTE, TRUE, TRUE, NULL);
  cout << "Setting a few values to negative numbers" << endl;
  for (int i=0; i < volume->getSize(0); i++) {
    for (int j=0; j < volume->getSize(1); j++) {
      for (int n=0; n < volume->getSize(2); n++) {
        volume->setVoxel(-50, i,j, n);
      }
    }
  }
  volume->output("negative_numbers.mnc");

  cout << "And now do the same thing with straight volume_io calls" << endl;

  Volume cVolume;
  input_volume(argv[1], 3, ZXYdimOrder, NC_FLOAT, TRUE, 0, 0, TRUE,
               &cVolume, NULL);
  for (int i=0; i< 100; i++) {
    for (int j=0; j<50; j++) {
      set_volume_real_value(cVolume, i, j, 50, 0, 0, -50);
    }
  }

  Real value = get_volume_real_value(cVolume, 1,1,50,0,0);
  cout << "Real value: " << value << endl;

  value = convert_value_to_voxel(cVolume, 50);
  cout << "Real value: " << value << endl;

  value = convert_value_to_voxel(cVolume, -50);
  cout << "Real value: " << value << endl;

  value = get_volume_voxel_min(cVolume);
  cout << "Real value: " << value << endl;

  value = get_volume_real_min(cVolume);
  cout << "Real value: " << value << endl;

  output_modified_volume("volume_io_negative_volume.mnc", NC_FLOAT, TRUE, 0,0,
                cVolume, argv[1], "test", NULL);

}
