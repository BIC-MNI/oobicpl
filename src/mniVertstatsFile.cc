#include "mniVertstatsFile.h"


mniVertstatsFile::mniVertstatsFile() {
  // do nothing - the real work is in loadFile

  this->initialiseVariables();

}

mniVertstatsFile::mniVertstatsFile(char *filename) {

  this->initialiseVariables();
  this->loadFile(filename);

}

mniVertstatsFileType mniVertstatsFile::determineFileType(char *filename) {

  mniVertstatsFileType thisFile;
  ifstream statsFile(filename);
  if (! statsFile) {
    cerr << "ERROR: could not read file " << filename << endl;
    exit(1);
  }
  string line;
  getline(statsFile, line);
  if (line.find("<header>") != string::npos) {
    thisFile = NEWSTYLE;
  }
  else {
    thisFile = OLDSTYLE;
  }
  statsFile.close();
  return thisFile;
}

void mniVertstatsFile::initialiseVariables() {

  this->mean = new string();
  this->formula = new string();
  this->matrix = new vector<string> ();
  this->dataheader = new vector<string> ();
  this->data = new vertexMatrix();

}

void mniVertstatsFile::loadFile(char *filename) {

  mniVertstatsFileType ft = this->determineFileType(filename);

  switch(ft) {
  case NEWSTYLE:
    this->loadNewStyleFile(filename);
    break;
  case OLDSTYLE:
    this->loadOldStyleFile(filename);
    break;
  default:
    cerr << "ERROR: there should not be a default file type!" << endl;
    exit(1);
  }

}

void mniVertstatsFile::loadOldStyleFile(char *filename) {

  string line;

  // open the file 
  ifstream statsFile(filename);
  if (! statsFile) {
    cerr << "ERROR: could not read file " << filename << endl;
    exit(1);
  }

  // determine how many columns this file contains
  getline(statsFile, line);
  
  // split the line on white space
  int firstpos = 0;
  int lastpos = line.length();
  int numSpaces = 0;
  while (lastpos != string::npos) {
    lastpos = line.find(" ", firstpos);
    firstpos = lastpos +1;
    numSpaces++;
  }
  this->numColumns = numSpaces;
  
  // initialise the data member
  this->data->resize( this->numColumns );

  // reset the file position
  statsFile.seekg(0);

  // get the data out of the file
  while (! statsFile.eof() ) {
    for (int i=0; i < this->numColumns; i++) {
      float currentVal;
      statsFile >> currentVal;
      (*this->data)[i].push_back(currentVal);
    }
  }
  
  /* the above file reading will always read the last number
     twice, since the inner loop does not check whether eof has
     been reached. So pop the last element */
  for (int i=0; i < this->numColumns; i++) {
    (*this->data)[i].pop_back();
  }

  // generate some header names, e.g. Column1, Column2, etc.
  for (int i=0; i < this->numColumns; i++) {
    char cName[15];
    sprintf(cName, "%s%i", "Column", i);
    this->dataheader->push_back(cName);
  }
}



void mniVertstatsFile::loadNewStyleFile(char *filename) {

  // initialise variables
  
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
      //cout << "In header" << endl;
      // do nothing for now other than setting statebit back to none
      filestate = NONE;
      break;
    case MEAN:
      // append to the means string until end of line means is reached
      //cout << "in mean" << endl;
      getline(statsFile, line);

      if (line.find("</mean>") != string::npos) {
        //cout << "mean ended" << endl;
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
      //cout << "In formula" << endl;
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
      //cout << "in matrix" << endl;
      getline(statsFile, line);
      if (line.find("</matrix>") != string::npos) {
        filestate = NONE;
      }
      else {
        this->matrix->push_back(line);
      }
      break;
    case DATA:
      //cout << "in data" << endl;
      //cout << line << endl;
      while (! statsFile.eof() ) {
        for (int i=0; i < this->numColumns; i++) {
          float currentVal;
          statsFile >> currentVal;
          (*this->data)[i].push_back(currentVal);
        }
      }

      /* the above file reading will always read the last number
         twice, since the inner loop does not check whether eof has
         been reached. So pop the last element */
      for (int i=0; i < this->numColumns; i++) {
        (*this->data)[i].pop_back();
      }
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
      this->numColumns = this->dataheader->size();
      // initialise the data member
      this->data->resize( this->numColumns );
      filestate = DATA;
      break;
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
}

vertexColumn mniVertstatsFile::getDataColumn(string columnName) {
  // stl::find does not work as it returns an iterator rather than
  // the position - so do it manually;
  int position = -1;
  for (int i=0; i < this->numColumns; i++) {
    if ((*this->dataheader)[i].find(columnName) != string::npos)
      position = i;
  }
  if (position == -1) {
    throw InvalidColumnError();
    vertexColumn empty;
    return empty;
  }
  else 
    return (*this->data)[position];
}

vertexColumn mniVertstatsFile::getDataColumn(int index) {
  if (index >= this->numColumns || index < 0) {
    throw InvalidColumnError();
    vertexColumn empty;
    return empty;
  }
  else
    return (*this->data)[index];
}
  

/*
int main(int argc, char *argv[]) {
  mniVertstatsFile f(argv[1]);
  vertexColumn test = f.getDataColumn("adj.r.square");
  vertexColumn test2 = f.getDataColumn("slope.1");
  cout << test[0] << endl;
  cout << test2[1] << endl;
  cout << test.size() << endl;
  vector<string> t = f.getDataHeader();
  cout << t[0] << endl;
  try { 
    vertexColumn test3 = f.getDataColumn("slope.2");
  }
  catch (mniVertstatsFile::InvalidColumnError) {
    cerr << "invalid row" << endl;
  }
  return(0);
}
*/
    
