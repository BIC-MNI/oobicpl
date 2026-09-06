// takes an obj file and returns the vertices in volume (rather than
// world coordinates). Not particularly useful, but written to satisfy
// the folks in Minnesota.

// BIC library includes
extern "C" {
#include <bicpl.h>
#include <volume_io.h>
}
#include <mniVolume.h>

// STL includes
#include <iostream>
#include <string>

// argument parsing
#include "mniArgs.h"

using namespace std;

int main (int argc, char *argv[]) {
  // geometry definitions
  VIO_File_formats        format;
  int                 num_objects;
  object_struct**     object_list;
  polygons_struct*    polygons;
  object_struct*      object;
  VIO_Point*              points;
  int                 n_points;
  
  // filename definitions
  string              input_object;
  string              input_volume;
  string              output_object;
  
  // parse command line arguments
  cxxopts::Options cArg( "surface_volume_coordinates", "");
  cArg.add_options()
    ("help", "display usage help");
  cArg.add_options("Arguments")
    ("input_object", "the input surface", cxxopts::value<string>())
    ("input_volume", "the volume to use for coordinate details", cxxopts::value<string>())
    ("output_object", "the surface with volume coordinates", cxxopts::value<string>());
  cArg.positional_help("<input_object> <input_volume> <output_object>");

  // parse arguments - prints usage and exits on -help or a bad command line
  cxxopts::ParseResult args = mniArgs::parse(cArg, argc, argv,
    {"input_object", "input_volume", "output_object"});

  // open the input object
  if (input_graphics_file( (char*) args["input_object"].as<string>().c_str(),
			   &format, &num_objects, &object_list) != VIO_OK) {
    cerr << "ERROR reading file " << args["input_object"].as<string>() << endl;
    return 1;
  }

  // complain if it is anything other than a polygon set
  if ( (object_list[0])->object_type != POLYGONS ) {
    cerr << "ERROR: can only read obj files containing polygons." << endl;
    return 1;
  }

  // open the volume which will be used to compute the world to voxel
  // coordinates.
  mniVolume *volume = new mniVolume( (char *) args["input_volume"].as<string>().c_str(),
				     0.0, 0.0, 3, XYZdimOrder);

  n_points = get_object_points(object_list[0], &points);
  VIO_Real x,y,z;
  for( int i=0; i < n_points; i++ ) {
    convert_3D_world_to_voxel(volume->getVolume(), 
			      (VIO_Real) Point_x(points[i]),
			      (VIO_Real) Point_y(points[i]),
			      (VIO_Real) Point_z(points[i]),
			      &x, &y, &z);
    fill_Point( points[i], x, y, z );
  }
  
  compute_polygon_normals( get_polygons_ptr(object_list[0]) );

  // output the revised obj file
  (void) output_graphics_file( (char *)args["output_object"].as<string>().c_str(), format,
			       num_objects, object_list );
  
  return 0;
}


