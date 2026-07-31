#include <iostream>
#include "mniArgs.h"

extern "C" {
#include <bicpl.h>
}

#include <mniVertstatsFile.h>
#include <vector>
#include <algorithm>

using namespace std;

// pair up values and their indices
class vertexIndexPair {
public:
  float value;
  int index;
  vertexIndexPair(float v, int i ) { 
    value = v;
    index = i;
  };
};

// to be able to print
ostream& operator << (ostream& os, vertexIndexPair& vip) {
  os << vip.value;
  os << " ";
  os << vip.index;
  return os;
}  

// is greater then
bool operator > (vertexIndexPair vip1, vertexIndexPair vip2) {
  if (vip1.value > vip2.value) 
    return true;
  else
    return false;
}

// is less then - needed for sort
bool operator < (vertexIndexPair vip1, vertexIndexPair vip2) {
  if (vip1.value < vip2.value) 
    return true;
  else
    return false;
}

int main (int argc, char *argv[]) {

  // argument handling
  cxxopts::Options cArg("vertstats_find_peaks", "(c) jason@bic");
  cArg.add_options()
    ("help", "display usage help")
    // minimum distance option
    ("min_distance", "minimum distance between peaks", cxxopts::value<string>(), "<minimum distance to use>")
    // minimum value option
    ("min_value", "minimum value of peaks", cxxopts::value<string>(), "<value to use>")
    // output vertstats file option - takes two values: filename and value
    ("vertstats_output", "output vertstats file with peaks", cxxopts::value<vector<string> >(), "<filename to output to> <value to place at peaks>");
  cArg.add_options("Arguments")
    ("vertstats_file", "vertstats file to find peaks from", cxxopts::value<string>())
    ("obj_file", "geometry to determine distances", cxxopts::value<string>())
    ("output", "comma separated list of peaks go here", cxxopts::value<string>());
  cArg.positional_help("<vertstats_file> <obj_file> <output>");

  mniArgs::MultiValueMap multiValue;
  multiValue["vertstats_output"] = 2;

  cxxopts::ParseResult args = mniArgs::parse(cArg, argc, argv,
    {"vertstats_file", "obj_file", "output"}, multiValue);

  float min_distance = 30.0;
  if (args.count("min_distance")) {
    min_distance = atof(args["min_distance"].as<string>().c_str());
  }

  float min_value = 0.0;
  if (args.count("min_value")) {
    min_value = atof(args["min_value"].as<string>().c_str());
  }
  
  // load the vertstats file
  mniVertstatsFile stats( args["vertstats_file"].as<string>().c_str());
  vertexColumn statsCol = stats.getDataColumn(0);
  
  // initialize the variables for the surface
  VIO_File_formats format;
  int num_objects;
  object_struct** object_list;
  polygons_struct *polygons;
  VIO_Point *points;

  // read in the surface
  if ( input_graphics_file( (char*) args["obj_file"].as<string>().c_str(), 
                            &format, &num_objects, &object_list )
       != VIO_OK ) {
    cerr << "ERROR reading file " << args["obj_file"].as<string>() << endl;
    return 0;
  }

  // can't deal with anything other than polygons for now
  if ((object_list[0])->object_type != POLYGONS ) {
    cerr << "ERROR: can only read obj files containing polygons." << endl;
    return 0;
  }

  polygons = get_polygons_ptr(object_list[0]);
  points = polygons->points;

  // a vector to hold the peaks
  std::vector<vertexIndexPair> peaks;
  // vector to hold the vertex values and indices
  std::vector<vertexIndexPair> vipVector;

  //assign values and indices
  for (int i=0; i < statsCol.size(); i++) {
    vipVector.push_back(vertexIndexPair(statsCol[i], i));
  }

  // sort the vector of values
  sort(vipVector.begin(), vipVector.end());
  reverse(vipVector.begin(), vipVector.end());

  // now build the peaks

  /* peak finding algorithm:
   * add the highest vertex.
   * add the next vertex if not within min_distance of existing peaks
   * if vertex added, start from next highest vertex
   */

  int max_peaks = 20;
  std::vector<vertexIndexPair>::iterator vipIt, vipIt2, vipItPeaks, vipItPeaks2;

  // add the highest vertex to the peaks vector
  vipIt = vipVector.begin();
  peaks.push_back( *vipIt );
  vipIt++;

  while (vipIt != vipVector.end() && peaks.size() <= max_peaks){
    bool reset = false;
    while (! reset) {
      vipItPeaks = peaks.begin();
      vipItPeaks2 = peaks.end();
      bool outsideRange = false;
      if ((*vipIt).value >= min_value) {
        outsideRange = true;
        while ( vipItPeaks != vipItPeaks2 ) {
          if ( distance_between_points(&points[(*vipIt).index], 
                                       &points[(*vipItPeaks).index])
               < min_distance) {
            outsideRange = false;
            break;
          }
          else {
            vipItPeaks++;
          }
        }
      }
      if (outsideRange) {
        peaks.push_back(*vipIt);
        reset = true;
      }
      else {
        vipIt++;
        if (vipIt == vipVector.end()) 
          reset = true;
      }
    }
  }

  // create output vertstats file if the user so desires
  string outputFile;
  float peakValue = 1;
  bool createOutputFile = false;
  mniVertstatsFile outputVstats;
  if (args.count("vertstats_output")) {
    const vector<string> &vertstatsOutput = args["vertstats_output"].as<vector<string> >();
    createOutputFile = true;
    outputFile = vertstatsOutput[0];
    peakValue = atof(vertstatsOutput[1].c_str());
  }

  // print the tags to stdout
  ofstream output((char *)args["output"].as<string>().c_str());
  output << "value, vertex, x, y, z" << endl;

  vipItPeaks = peaks.begin();
  while (vipItPeaks != peaks.end()) {
    output << (*vipItPeaks).value << ", " 
         << (*vipItPeaks).index << ", "
         << Point_x(points[(*vipItPeaks).index]) << ", "
         << Point_y(points[(*vipItPeaks).index]) << ", "
         << Point_z(points[(*vipItPeaks).index]) << endl;
    statsCol[(*vipItPeaks).index] = peakValue;
    vipItPeaks++;
  }
  output.close();

  if (createOutputFile) {
    outputVstats.putDataColumn(statsCol, "peaks");
    outputVstats.addToHistory("vertstats_find_peaks");
    outputVstats.writeFile(outputFile);
  }
    
    

  return 0;
}

  
