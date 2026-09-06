/* computes the surface area of a region of interest on a 
polygonal surface, as defined by a vertstat file.

$Id: surface_area_roi.cc,v 1.3 2013-02-07 19:52:48 claude Exp $
*/

#include <mniVertstatsFile.h>

extern "C" {
#include <bicpl.h>
}

#include <iostream>
#include <list>
#include "mniArgs.h"
#include <algorithm>

using namespace std;

int main (int argc, char *argv[]) {
  vertexColumn  regions;
  list<int>     included_vertices;
  int           included_polygons = 0;
  int           roi;

  VIO_File_formats    format;
  int             num_objects;
  object_struct** object_list;
  polygons_struct *polygons;

  VIO_Point points[10];
  int n_points;
  int poly;

  VIO_Real area;
  area = 0.0;

  // parse command line arguments
  cxxopts::Options cArg( "surface_area_roi", "(c) jharlap@bic" );
  cArg.add_options()
    ("help", "display usage help")
    ("region", "define the region of interest (defaults to all regions > 0)", cxxopts::value<string>(), "<region of interest>")
    ("column", "define the column in the vertstats file containing the ROI segmentation", cxxopts::value<string>(), "<name of the column>")
    ("include", "inclusion rule - either one or all vertices of a polygon must be in the ROI to include the polygon in the surface area", cxxopts::value<string>(), "<'one' or 'all' (default is 'one')>");
  cArg.add_options("Arguments")
    ("surface_file", "surface object file", cxxopts::value<string>())
    ("vertstats_file", "vertstats file containing ROI segmentation", cxxopts::value<string>());
  cArg.positional_help("<surface_file> <vertstats_file>");

  cxxopts::ParseResult args = mniArgs::parse(cArg, argc, argv,
    {"surface_file", "vertstats_file"});

  if(args.count("region")) {
    cout << "Using ROI " << args["region"].as<string>() << endl;
  }

  if(args.count("column")) {
    cout << "Using column " << args["column"].as<string>() << endl;
  }

  // open the surface file
  cout << "Loading: " << args["surface_file"].as<string>() << endl;
  if ( input_graphics_file( (char*) args["surface_file"].as<string>().c_str(), &format, &num_objects, &object_list )
       != VIO_OK ) {
    cerr << "ERROR reading file " << args["surface_file"].as<string>() << endl;
    return 0;
  }

  // can't deal with anything other than polygons for now
  if ((object_list[0])->object_type != POLYGONS ) {
    cerr << "ERROR: can only read obj files containing polygons." << endl;
    return 0;
  }
  
  polygons = get_polygons_ptr(object_list[0]);
  
  // open the verstat file
  mniVertstatsFile stats(args["vertstats_file"].as<string>().c_str());
  cout << "Loading: " << args["vertstats_file"].as<string>() << endl;

  // read the user-defined column (or the first column if not defined)
  if(args.count("column"))
    regions = stats.getDataColumn(args["column"].as<string>().c_str());
  else
    regions = stats.getDataColumn(0);

  // get the region of interest
  if(args.count("region")) {
    roi = atoi(args["region"].as<string>().c_str());
  } else {
    roi = -1;
  }

  // find vertices that match the desired ROI
  vertexColumn::iterator regions_iterator;
  int vert_index;
  vert_index = 0;
  cout << "Starting loop through region list" << endl;
  for (regions_iterator = regions.begin() ; regions_iterator != regions.end() ; regions_iterator++) {
    // if this vertex is part of the ROI
    if ((roi == -1 && *regions_iterator > 0) || *regions_iterator == roi) {
      // add the vertex to the list of vertices to include in the surface area tally
      included_vertices.push_back(vert_index);
    }
    vert_index++;
  }
  cout << "Found " << included_vertices.size() << " vertices for ROI " << roi << endl;

  list<int>::iterator point_idx;
  int included_vertex_count = 0;
  int required_vertex_count = 1;

  if(args.count("include") && args["include"].as<string>() == "all")
    required_vertex_count = n_points;
    

  // loop through all the polygons
  cout << "Starting loop through polygons" << endl;
  for (poly = 0 ; poly < polygons->n_items ; ++poly) {
    // find the points for the polygon
    n_points = get_polygon_points(polygons, poly, points);
    if(n_points > 0) {
      // reset the included vertex counter and set the number of required vertices
      included_vertex_count = 0;
      if(args.count("include") && args["include"].as<string>() == "all")
        required_vertex_count = n_points;

      // loop over all the points in the polygon
      for (int i = 0; i < n_points; ++i) {
        // include the point in the included vertex count if it's in
        // the included vertex list
        point_idx = std::find(included_vertices.begin(), included_vertices.end(), POINT_INDEX(polygons->end_indices,poly,i));
        if(point_idx != included_vertices.end()) {
          included_vertex_count++;
        }

        // if we found enough vertices to include the poly
        if(included_vertex_count >= required_vertex_count) {
          // add the polygon's surface area to the tally
          area += get_polygon_surface_area (n_points, points);
          included_polygons++;
          
          // stop looping over points if we've included enough points
          break;
        }
      } // end looping over points
    }
  } // end looping through polygons

  cout << "Surface area: " << area << " using " << included_polygons << " polygons" << endl;
  
  return 0;
}
