// stats.cpp
#include "stats.h"
#include <cassert>
#include <cmath>
#include <vector>
#include "p1_library.h"
using namespace std;

//REQUIRES: v is not empty
//EFFECTS: returns a summary of the dataset as (value, frequency) pairs
//  In the returned vector-of-vectors, the inner vector is a (value, frequency)
//  pair.  The outer vector contains many of these pairs.  The pairs should be
//  sorted by value.
//  {
//    {1, 2},
//    {2, 3},
//    {17, 1}
//   }
//
// This means that the value 1 occurred twice, the value 2 occurred 3 times,
// and the value 17 occurred once
vector<vector<double> > summarize(vector<double> v) {
  assert(!v.empty());
  sort(v); 
  vector<vector<double>> z; 
  double counter = 1;
  for(int i = 0; i < count(v); i++){
    if(i == (count(v) - 1)){
      z.push_back({v.at(i), counter}); 
      counter = 1; 
    }
   else if(v.at(i) == v.at(i+1)) {
      counter += 1; 
    }  
    else {
      z.push_back({v.at(i), counter}); 
      counter = 1; 
    }
  }
  return z;
}

//EFFECTS: returns the count of the numbers in v
int count(vector<double> v) {
  
  return static_cast<int>(v.size()); 
}

//REQUIRES: v is not empty
//EFFECTS: returns the sum of the numbers in v
double sum(vector<double> v) {
  assert(!v.empty());
  double sum = 0; 
  for(auto i:v) sum += i; 
  return sum; 
}

//REQUIRES: v is not empty
//EFFECTS: returns the arithmetic mean of the numbers in v
double mean(vector<double> v) {
  assert(!v.empty());
  return (sum(v) / count(v)); 
}

//REQUIRES: v is not empty
//EFFECTS: returns the sample median of the numbers in v
double median(vector<double> v) {
  assert(!v.empty());
  //sort and then find middle 
  sort(v); 
  if((count(v) % 2) == 0) {
    double middle = (v.at((count(v) / 2.0) - 1) + v.at(count(v) / 2.0)) / 2.0; 
    return middle; 
  }
  else {
    return (v.at(0) + v.at(count(v) - 1.0)) / 2.0; 
  }
}

//REQUIRES: v is not empty
//EFFECTS: returns the mode of the numbers in v
//  http://en.wikipedia.org/wiki/Mode_(statistics)
//  Example: mode({1,2,3}) = 1
//  Example: mode({1,1,2,2}) = 1
//  Example: mode({2,2,1,1}) = 1
//  Example: mode({1,2,1,2}) = 1
//  Example: mode({1,2,1,2,2}) = 2
//  In the event of a tie, return the smallest value (as in the 
//  above examples)
//HINT 1: use the sort() function provided in p1_library.h
//HINT 2: use a nested loop
//HINT 3: use a variable to remember the most frequent number seen so far
double mode(vector<double> v) {
  assert(!v.empty());
  sort(v); 
  int num = v[0];
  int mode = num;
  int count = 1;
  int modeCount = 1;

  for (int i = 0; i < int(v.size()); i++) {
    if (v[i] == num) { 
      count++;
      if (count > modeCount) {
        modeCount = count; 
        mode = num; 
      }
    }
    else { 
      count = 1; 
      num = v[i]; 
    }
}
  if (modeCount == 1) {
    mode = -1; 
  }
  return mode; 
}

//REQUIRES: v is not empty
//EFFECTS: returns the min number in v
double min(vector<double> v) {
  assert(!v.empty());
  sort(v); 
  return v.at(0); 
}

//REQUIRES: v is not empty
//EFFECTS: returns the max number in v
double max(vector<double> v) {
  assert(!v.empty());
  sort(v); 
  return v.at(count(v) - 1.0); 
}

//REQUIRES: v contains at least 2 elements
//EFFECTS: returns the corrected sample standard deviation of the numbers in v
double stdev(vector<double> v) {
  assert((count(v) - 1) >= 2); 
  double mean = sum(v) / count(v);
  double sumElements = 0; 
  for(auto i:v) sumElements += (pow((i - mean), 2));
  return (sqrt(sumElements / (count(v) - 1))); 
}

//REQUIRES: v is not empty
//          p is between 0 and 1, inclusive
//EFFECTS: returns the percentile p of the numbers in v like Microsoft Excel.
//  Refer to the Appendix in the spec for the formula to use.
//  NOTE: the definition in the wiki article uses indexing from 1.  You will
//  need to adapt it to use indexing from 0.
double percentile(vector<double> v, double p) {
  assert(!v.empty());
  assert(p >= 0.0 && p <= 1.0);
  sort(v);
  if (p == 0) {
    return v.at(0);
  }
  else if (p == 1) {
    return v.at(count(v) - 1); 
  }
  double k = 0; 
  double d = 0; 
  double n = (p*(count(v) - 1)) + 1.0; 
  d = modf(n, &k);
  return (v.at(k-1) + (d*(v.at(k) - v.at(k - 1)))); 
}