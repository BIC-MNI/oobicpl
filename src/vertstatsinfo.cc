// Provides a bit of info about a vertex file

#include "mniVertstatsFile.h"
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {

  if (argc != 2) {
    cerr << "USAGE: verstatsinfo filename" << endl;
    return(1);
  }

  mniVertstatsFile f(argv[1]);
  
  cout << "Number of columns: " << f.getNumColumns() << endl;
  cout << "Number of rows:    " << f.getNumRows() << endl;
  cout << "Data headers:" << endl;

  vector<string> header = f.getDataHeader();
  for (int i=0; i < f.getNumColumns(); i++) {
    cout << "    " << header[i] << endl;
  }

  return(0);
}
