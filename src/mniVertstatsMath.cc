
#include "mniVertstatsMath.h"
#include <algorithm>


mniVectorStats::mniVectorStats(vector<float> input) {
  computeStats(input);
}


void mniVectorStats::computeStats(vector<float> input) {
  vector<float>::iterator it;

  sort(input.begin(), input.end());
  int midIndex = (int) input.size() / 2;
  if (input.size() % 2 == 1) {
    vmedian = (input[midIndex] + input[midIndex+1]) / 2;
  }
  else {
    vmedian = input[midIndex];
  }
  vmax = input[input.size()-1];
  vmin = input[0];
  vsum = vectorSum(input);
  vmean = vsum / (input.size()+1);

}
  
