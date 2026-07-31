#include <iostream>
#include "mniArgs.h"

extern "C" {
#include <bicpl.h>
}
#include <mniVolume.h>
#include <mniVertstatsFile.h>

using namespace std;

VIO_Real rms_distance_from_point(
                             VIO_Point  *point,
                             VIO_Point  *end_point1,
                             VIO_Point  *end_point2 ) {
  VIO_Real  dist1, dist2;

  dist1 = sq_distance_between_points(point, end_point1);
  dist2 = sq_distance_between_points(point, end_point2);

  return(dist1 + dist2);
}

int main (int argc, char *argv[]) {
  cxxopts::Options cArg("vertstat_to_volume", "(c) jharlap@bic");
  cArg.add_options()
    ("help", "display usage help")
    ("mask", "consider input volume as a mask, and only map verstats for voxels with value > 0.5")
    ("column", "define the column in the vertstats file containing the statistic of interest", cxxopts::value<string>(), "<name of the column>");
  cArg.add_options("Arguments")
    ("input_volume_file", "volume to determine dimension sizes", cxxopts::value<string>())
    ("white_surface_mesh_file", "white matter surface mesh", cxxopts::value<string>())
    ("gray_surface_mesh_file", "gray matter surface mesh", cxxopts::value<string>())
    ("vertstats_file", "verstats file", cxxopts::value<string>())
    ("output_volume_file", "output volume", cxxopts::value<string>());
  cArg.positional_help("<input_volume_file> <white_surface_mesh_file> <gray_surface_mesh_file> <vertstats_file> <output_volume_file>");

  cxxopts::ParseResult args = mniArgs::parse(cArg, argc, argv,
    {"input_volume_file", "white_surface_mesh_file", "gray_surface_mesh_file",
     "vertstats_file", "output_volume_file"});

  // open the verstat file
  mniVertstatsFile stats((char *)args["vertstats_file"].as<string>().c_str());
  cout << "Loading: " << args["vertstats_file"].as<string>() << endl;
  
  // read the user-defined column (or the first column if not defined)
  vertexColumn statsCol;
  if(args.count("column"))
    statsCol = stats.getDataColumn(args["column"].as<string>().c_str());
  else
    statsCol = stats.getDataColumn(0);

  VIO_Real statMin = 10000000;
  VIO_Real statMax = -10000000;
  vertexColumn::iterator it;
  for(it = statsCol.begin(); it != statsCol.end() ; it++) {
    if(*it > statMax) statMax = *it;
    if(*it < statMin) statMin = *it;
  }
  if(statMin > 0) statMin = 0;

  // cxxopts gives us std::strings, we use strcpy to munge the const
  // char * to a char * so that it is palatable to mniVolume
  char inputVolumeFile[255] = "";
  strcpy(inputVolumeFile, args["input_volume_file"].as<string>().c_str());
  char outputVolumeFile[255] = "";
  strcpy(outputVolumeFile, args["output_volume_file"].as<string>().c_str());

  // make mni volume objects
  mniVolume *maskVolume = new mniVolume(inputVolumeFile);
  mniVolume *outputVolume = new mniVolume(maskVolume, TRUE, NC_SHORT, TRUE, -32760, 32760);
  outputVolume->setRealRange(statMin, statMax);

  // initialize variables for surfaces
  VIO_File_formats    format;
  int             num_objects;
  object_struct** object_list_white;
  object_struct** object_list_gray;
  polygons_struct *polygons_white;
  polygons_struct *polygons_gray;

  // read in the white surface
  if ( input_graphics_file( (char*) args["white_surface_mesh_file"].as<string>().c_str(), &format, &num_objects, &object_list_white )
       != VIO_OK ) {
    cerr << "ERROR reading file " << args["white_surface_mesh_file"].as<string>() << endl;
    return 0;
  }

  // can't deal with anything other than polygons for now
  if ((object_list_white[0])->object_type != POLYGONS ) {
    cerr << "ERROR: can only read obj files containing polygons." << endl;
    return 0;
  }
  
  // read in the gray surface
  if ( input_graphics_file( (char*) args["gray_surface_mesh_file"].as<string>().c_str(), &format, &num_objects, &object_list_gray )
       != VIO_OK ) {
    cerr << "ERROR reading file " << args["gray_surface_mesh_file"].as<string>() << endl;
    return 0;
  }

  // can't deal with anything other than polygons for now
  if ((object_list_gray[0])->object_type != POLYGONS ) {
    cerr << "ERROR: can only read obj files containing polygons." << endl;
    return 0;
  }
  
  // initialize points
  VIO_Point point;
  VIO_Point *points_white;
  VIO_Point *points_gray;

  polygons_white = get_polygons_ptr(object_list_white[0]);
  polygons_gray = get_polygons_ptr(object_list_gray[0]);

  points_white = polygons_white->points;
  points_gray = polygons_gray->points;

  int *volumeSizes;
  volumeSizes = outputVolume->getSizes();
  
  cout << "Starting loop" << endl;

  int progress, last_progress;
  for(int i=0; i<volumeSizes[0]; ++i) {
    for(int j=0; j<volumeSizes[1]; ++j) {
      for(int k=0; k<volumeSizes[2]; ++k) {
        // if we're using the input volume as a mask
        if(args.count("mask")) {
          // if the value is less than 0.5, zero this voxel
          if(maskVolume->getVoxel(i, j, k) < 0.5) {
            outputVolume->setVoxel(0, i, j, k);
            continue;
          }
        }

        VIO_Real voxel_coord[3] = {static_cast<VIO_Real>(i), static_cast<VIO_Real>(j), static_cast<VIO_Real>(k)};
        VIO_Real *world_coord;
        world_coord = outputVolume->convertVoxelToWorld(voxel_coord);

        // set query point location
        Point_x(point) = world_coord[0];
        Point_y(point) = world_coord[1];
        Point_z(point) = world_coord[2];

        VIO_Real cur_dist;
        VIO_Real min_dist = 100000000.0f;
        int min_point_index;
        for(int point_index=0; point_index<polygons_white->n_points; ++point_index) {
          cur_dist = rms_distance_from_point(&point, &points_white[point_index], &points_gray[point_index]);
          if(cur_dist < min_dist) {
            min_dist = cur_dist;
            min_point_index = point_index;
          }
        }
        
        outputVolume->setVoxel(statsCol[min_point_index], i,j,k);
        //cerr << "setting " << i << ", " << j << ", " << k << "(" << Point_x(point) << ", " << Point_y(point) << ", " << Point_z(point) << ") to value " << statsCol[min_point_index] << endl;
      }
    }
    progress = (int)((float)i/(float)volumeSizes[0]*100.0f);
    if(progress != last_progress) {
      cerr << progress << "%" << endl;
      last_progress = progress;
    }
  }

  cerr << endl;
  outputVolume->output(outputVolumeFile);
  cout << "Done" << endl;
  return 0;
}
