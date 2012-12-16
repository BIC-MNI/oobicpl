#ifndef __MNIVERTSTATSMATH__
#define __MNIVERTSTATSMATH__

#include <vector>
#include <iostream>
#include <stdlib.h>
 
/*! \addtogroup vertstats Dealing with vertstats files */
/*@{*/

/*! \file mniVertstatsMath.h
 * Mathematical routines for dealing with vectors
 */

using namespace std;

/*! Computes, holds, and prints a series of simple statistics 
 *
 * Computes a series of simple statistics, namely max, min, median,
 * mean, and sum of a vector. These elements are all available as
 * public members of the class.
 */
class mniVectorStats {
public:
  //! The minimum value of the vector
  float vmin;
  //! The maximum value of the vector
  float vmax;
  //! The sum of all the vector's elements
  float vsum;
  //! The mean of all the vector's elements
  float vmean;
  //! The median of all the vector's elements
  float vmedian;

  /*! The constructor
   *
   * Takes a vector as an input, computes the statistics, then makes
   * them available in the individual data members.
   */
  mniVectorStats(vector<float> input);
private:
  //! Actually performs the computation
  void computeStats(vector<float> input);
  //! Prints the results
  friend ostream& operator<<(ostream& out, const mniVectorStats& v) {
    out << " Maximum: " << v.vmax << endl;
    out << " Minimum: " << v.vmin << endl;
    out << " Median:  " << v.vmedian << endl;
    out << " Mean:    " << v.vmean << endl;
    out << " Sum:     " << v.vsum << endl;
  }
};


/*! Computes the sum of all the elements of a vector */
template <class T>
T vectorSum(vector<T> input) {
  T sum = 0;
  typename vector<T>::iterator it;
  for (it = input.begin(); it != input.end(); ++it) {
    sum += *it;
  }
  return sum;
}

/*! Computes the mean of a vector */
template <class T>
T vectorMean(vector<T> &v1) {
  T sum = vectorSum(v1);
  T mean = sum / v1.size();
  return mean;
}

/*! Ensures that size of two vectors is the same
 *
 * Checks the size of two vectors, exits with an exit(1) call should
 * the sizes differ.
 */
template <class T>
void vectorSizeCheck(vector<T> &v1, vector<T> &v2) {
  if (v1.size() != v2.size()) {
    cerr << "ERROR: tried adding two vectors of unequal dimensions!" <<endl;
    exit(1);
  }
}

/*! Normalise a vector
 *
 * Normalises a vector by dividing each element by the mean 
 */
template <class T>
vector<T> vectorNormalise(vector<T> &v1) {
  T mean = vectorMean(v1);
  vector<T> result( v1 );
  for (int i=0; i < v1.size(); i++) {
    result[i] = v1[i] / mean;
  }
  return result;
}

/*! Take absolute value of a vector
 */
template <class T>
vector<T> vectorAbsolute(vector<T> &v1) {

  vector<T> result( v1 );
  for (int i=0; i < v1.size(); i++) {
    result[i] = ( v1[i] < (T)0 ) ? -v1[i] : v1[i];
  }
  return result;
}

/*! Element by element addition of two vectors */
template <class T>
vector<T> vectorAdd(vector<T> &v1, vector<T> &v2) {
  vectorSizeCheck(v1, v2);
  vector<T> result( v1 );
  for (int i=0; i < v1.size(); i++) {
    result[i] = v1[i] + v2[i];
  }
  return result;
}

/*! Adds a constant to a vector */
template <class T, class T2>
vector<T> vectorAdd(vector<T> &v1, const T2 constant) {
  vector<T> result( v1 );
  for (int i=0; i < v1.size(); i++) {
    result[i] = v1[i] + constant;
  }
  return result;
}

/*! Subtracts two vectors */
template <class T>
vector<T> vectorSub(vector<T> &v1, vector<T> &v2) {
  vectorSizeCheck(v1, v2);
  vector<T> result( v1 );
  for (int i=0; i < v1.size(); i++) {
    result[i] = v1[i] - v2[i];
  }
  return result;
}

/*! Subtracts a constant from a vector */
template <class T, class T2>
vector<T> vectorSub(vector<T> &v1, const T2 constant) {
  vector<T> result( v1 );
  for (int i=0; i < v1.size(); i++) {
    result[i] = v1[i] - constant;
  }
  return result;
}

/*! Finds all elements in the range specified.  Returns a vector of
 *  ints containing the indices where the elements were in the
 *  range. */
template <class T, class T2>
vector<int> vectorFind(vector<T> &v1, 
                       const T2 lowerLimit, 
                       const T2 upperLimit) {
  vector<int> result;
  for (int i=0; i < v1.size(); i++) {
    if (v1[i] > lowerLimit && v1[i] < upperLimit)
      result.push_back(i);
  }
  return result;
}

/*! Segments a vector. Returns a vector of type T, containing a 1 at
 * each element where the input vector is in the specified range, a 0
 * otherwise. */
template <class T, class T2>
vector<T> vectorSeg(vector<T> &v1,
                      const T2 lowerLimit,
                      const T2 upperLimit,
                    float outputValue=1.0) {
  vector<T> result( v1 );
  for (int i=0; i < v1.size(); i++) {
    if (v1[i] > lowerLimit && v1[i] < upperLimit) {
      result[i] = outputValue;
    }
    else {
      result[i] = 0;
    }
  }
  return result;
}

/*! Element by element division of two vectors 
 *
 * \bug Does not check for a divide by zero error
 */
template <class T>
vector<T> vectorDiv(vector<T> &v1, vector<T> &v2) {
  vectorSizeCheck(v1, v2);
  vector<T> result( v1 );
  for (int i=0; i< v1.size(); i++) {
    result[i] = v1[i] / v2[i];
  }
  return result;
}

/*! Divides each element of a vector by a constant */
template <class T, class T2>
vector<T> vectorDiv(vector<T> &v1, const T2 constant) {
  vector<T> result( v1 );
  for (int i=0; i < v1.size(); i++) {
    result[i] = v1[i] / constant;
  }
  return result;
}

/*! Element by element multiplication of two vectors */
template <class T>
vector<T> vectorMult(vector<T> &v1, vector<T> &v2) {
  vectorSizeCheck(v1, v2);
  vector<T> result( v1 );
  for (int i=0; i < v1.size(); i++) {
    result[i] = v1[i] * v2[i];
  }
  return result;
}

/*! Multiplies each element of a vector by a constant */
template <class T, class T2>
vector<T> vectorMult(vector<T> &v1, const T2 constant) {
  vector<T> result( v1 );
  for (int i=0; i < v1.size(); i++) {
    result[i] = v1[i] * constant;
  }
  return result;
}

/*@}*/

#endif // __MNIVERTSTATSMATH__

