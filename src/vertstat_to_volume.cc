#include <iostream>
#include <arguments.h>

extern "C" {
#include <bicpl.h>
}
#include <mniVolume.h>
#include <mniVertstatsFile.h>

using namespace std;

Real rms_distance_from_point(
                             Point  *point,
                             Point  *end_point1,
                             Point  *end_point2 ) {
  Real  dist1, dist2;

  dist1 = sq_distance_between_points(point, end_point1);
  dist2 = sq_distance_between_points(point, end_point2);

  return(dist1 + dist2);
}

int main (int argc, char *argv[]) {
  Arguments cArg("vertstat_to_volume", "(c) jharlap@bic", "-");
  cArg.addOption("help", "display usage help");
  cArg.addOption("mask", "consider input volume as a mask, and only map verstats for voxels with value > 0.5");
  cArg.addArgument("input_volume_file", "volume to determine dimension sizes");
  cArg.addArgument("white_surface_mesh_file", "white matter surface mesh");
  cArg.addArgument("gray_surface_mesh_file", "gray matter surface mesh");
  cArg.addArgument("vertstats_file", "verstats file");
  cArg.addArgument("output_volume_file", "output volume");

  Arguments::Option cOptColumn("column", "define the column in the vertstats file containing the statistic of interest");
  cOptColumn.addArgument("column", "name of the column");
  cArg.addOption(cOptColumn);

  if(!cArg.parse(argc, argv))
    return 1;

  if(cArg.getOption((char *) "help")) {
    cArg.usage();
    return 0;
  }

  // open the verstat file
  mniVertstatsFile stats((char *)cArg[(char *) "vertstats_file"].c_str());
  cout << "Loading: " << cArg[(char *) "vertstats_file"] << endl;
  
  // read the user-defined column (or the first column if not defined)
  vertexColumn statsCol;
  if(cArg.getOption((char *) "column"))
    statsCol = stats.getDataColumn(cArg.getOption((char *) "column")[(char *) "column"].c_str());
  else
    statsCol = stats.getDataColumn(0);

  Real statMin = 10000000;
  Real statMax = -10000000;
  vertexColumn::iterator it;
  for(it = statsCol.begin(); it != statsCol.end() ; it++) {
    if(*it > statMax) statMax = *it;
    if(*it < statMin) statMin = *it;
  }
  if(statMin > 0) statMin = 0;

  // Arguments uses std::strings, we use strcpy to munge the const
  // char * to a char * so that it is palatable to mniVolume
  char inputVolumeFile[255] = "";
  strcpy(inputVolumeFile, cArg[(char *) "input_volume_file"].c_str());
  char outputVolumeFile[255] = "";
  strcpy(outputVolumeFile, cArg[(char *) "output_volume_file"].c_str());

  // make mni volume objects
  mniVolume *maskVolume = new mniVolume(inputVolumeFile);
  mniVolume *outputVolume = new mniVolume(maskVolume, TRUE, NC_SHORT, TRUE, -32760, 32760);
  outputVolume->setRealRange(statMin, statMax);

  // initialize variables for surfaces
  File_formats    format;
  int             num_objects;
  object_struct** object_list_white;
  object_struct** object_list_gray;
  polygons_struct *polygons_white;
  polygons_struct *polygons_gray;

  // read in the white surface
  if ( input_graphics_file( (char*) cArg[(char *) "white_surface_mesh_file"].c_str(), &format, &num_objects, &object_list_white )
       != OK ) {
    cerr << "ERROR reading file " << cArg[(char *) "white_surface_mesh_file"] << endl;
    return 0;
  }

  // can't deal with anything other than polygons for now
  if ((object_list_white[0])->object_type != POLYGONS ) {
    cerr << "ERROR: can only read obj files containing polygons." << endl;
    return 0;
  }
  
  // read in the gray surface
  if ( input_graphics_file( (char*) cArg[(char *) "gray_surface_mesh_file"].c_str(), &format, &num_objects, &object_list_gray )
       != OK ) {
    cerr << "ERROR reading file " << cArg[(char *) "gray_surface_mesh_file"] << endl;
    return 0;
  }

  // can't deal with anything other than polygons for now
  if ((object_list_gray[0])->object_type != POLYGONS ) {
    cerr << "ERROR: can only read obj files containing polygons." << endl;
    return 0;
  }
  
  // initialize points
  Point point;
  Point *points_white;
  Point *points_gray;

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
        if(cArg.getOption((char *) "mask")) {
          // if the value is less than 0.5, zero this voxel
          if(maskVolume->getVoxel(i, j, k) < 0.5) {
            outputVolume->setVoxel(0, i, j, k);
            continue;
          }
        }

        Real voxel_coord[3] = {i, j, k};
        Real *world_coord;
        world_coord = outputVolume->convertVoxelToWorld(voxel_coord);

        // set query point location
        Point_x(point) = world_coord[0];
        Point_y(point) = world_coord[1];
        Point_z(point) = world_coord[2];

        Real cur_dist;
        Real min_dist = 100000000.0f;
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
