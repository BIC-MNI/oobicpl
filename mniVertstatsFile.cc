#include "vertstatsFile.h"

mniVertstatsFile::mniVertstatsFile(char *filename) {

  // initialise variables
  this->mean = new string();
  this->formula = new string();
  this->matrix = new vector<string> ();
  this->dataheader = new vector<string> ();
  
  // open the file
  ifstream statsFile(filename);
  if (! statsFile) {
    cerr << "ERROR: could not read file " << filename << endl;
    exit(1);
  }

  // state bits
  enum state { HEADER, MEAN, FORMULA, MATRIX, DATA, NONE, DATAHEADER };

  state filestate = NONE;

  // loop over file
  while (! statsFile.eof()) {
    string line;
    int firstpos, lastpos;
    //    cout << filestate << endl;
    // check for which part of the file is currently being read
    switch (filestate) {
    case HEADER:
      cout << "In header" << endl;
      // do nothing for now other than setting statebit back to none
      filestate = NONE;
      break;
    case MEAN:
      // append to the means string until end of line means is reached
      cout << "in mean" << endl;
      getline(statsFile, line);

      if (line.find("</mean>") != string::npos) {
        cout << "mean ended" << endl;
        filestate = NONE;
      }
      else {
        this->mean->append(line);
        this->mean->append("\n");
      }
      break;
    case FORMULA:
      // the formula: one or more lines of text describing how the
      // data was computed.
      cout << "In formula" << endl;
      getline(statsFile, line);
      if (line.find("</formula>") != string::npos) {
        filestate = NONE;
      }
      else {
        this->formula->append(line);
        this->formula->append("\n");
      }
      break;
    case MATRIX:
      cout << "in matrix" << endl;
      getline(statsFile, line);
      if (line.find("</matrix>") != string::npos) {
        filestate = NONE;
      }
      else {
        this->matrix->push_back(line);
      }
      break;
    case DATA:
      cout << "in data" << endl;
      filestate = NONE;
      break;
    case DATAHEADER:
      // a single line which describes the columns that follow
      getline(statsFile, line);
      
      // split the line on white space
      firstpos = 0;
      lastpos = line.length();
      while (lastpos != string::npos) {
        lastpos = line.find(" ", firstpos);
        this->dataheader->push_back(line.substr(firstpos, lastpos - firstpos));
        firstpos = lastpos +1;
      }

      filestate = DATA;
    case NONE:
      // no state bit currently set. This means that the next readable 
      // element should set the state flag
      //      cout << "in none" << endl;
      getline(statsFile, line);

      if (line.find("<header>") != string::npos)
        filestate = HEADER;
      else if (line.find("<mean>") != string::npos)
        filestate = MEAN;
      else if (line.find("<formula>") != string::npos)
        filestate = FORMULA;
      else if (line.find("<matrix>") != string::npos)
        filestate = MATRIX;
      else if (line.find("</header>") != string::npos)
        filestate = DATAHEADER;
          
      break;
    }
  }
  cout << "MEAN" << endl << endl;
  cout << this->mean->c_str() << endl;
  cout << "Length of header: " << this->dataheader->size() << endl;
   for (int i=0; i < this->dataheader->size(); i++) {
     cout << i << ": " << (*this->dataheader)[i] << endl;
   }
}

int main(int argc, char *argv[]) {
  mniVertstatsFile f(argv[1]);
  return(0);
}

    
