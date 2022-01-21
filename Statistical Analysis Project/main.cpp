//main.cpp
#include "stats.h"
#include "p1_library.h"
#include <iostream>
using namespace std;

int main() {
  cout << "enter a filename" << endl; 
  string filename; 
  cin >> filename; 
  cout << "enter a column name" << endl; 
  string column; 
  cin >> column; 
  cout << "reading column " << column << " from " << filename << endl; 
  cout << "Summary (value: frequency)" << endl; 
  vector<double> x = extract_column(filename, column); 
  vector<vector<double>> z = summarize(x);
  for(int i = 0; i < count(x); i++){
    cout << z.at(i).at(0) << ": " << z.at(i).at(1) << endl; 
  }
  
  cout << endl; 
 
  cout << "count = " << count(x) << endl; 
  cout << "sum = " << sum(x) << endl;
  cout << "mean = " << mean(x) << endl;
  cout << "stdev = " <<  stdev(x) << endl;
  cout << "median = " << median(x) << endl;
  cout << "mode = " << mode(x) << endl;
  cout << "min = " << min(x) << endl;
  cout << "max = " << max(x) << endl;

  cout << "  0th percentile = " << percentile(x, 0) << endl;
  cout << " 25th percentile = " << percentile(x, .25) << endl;
  cout << " 50th percentile = " << percentile(x, .5) << endl;
  cout << " 75th percentile = " << percentile(x, .75) << endl;
  cout << "100th percentile = " << percentile(x, 1) << endl;

}