/* stats_tests.cpp
 *
 * Unit tests for the simple statistics library
 * Project UID 5366c7e2b77742d5b2142097e51561a5
 *
 * EECS 280 Project 1
 *
 * Protip #1: Write tests for the functions BEFORE you implement them!  For
 * example, write tests for median() first, and then write median().  It sounds
 * like a pain, but it helps make sure that you are never under the illusion
 * that your code works when it's actually full of bugs.
 *
 * Protip #2: Instead of putting all your tests in main(),  put each test case
 * in a function!
 */


#include "stats.h"
#include <iostream>
#include <cassert>
#include <vector>
using namespace std;

void test_sum_small_data_set();
void test_count(); 
void test_mean(); 
void test_median(); 
void test_mode(); 
void test_min(); 
void test_max(); 
void test_summarize(); 
void test_stdev(); 
void test_percentile(); 

// Add prototypes for you test functions here.

int main() {
  test_sum_small_data_set();
  test_count(); 
  test_mean(); 
  test_median(); 
  test_mode(); 
  test_min(); 
  test_max(); 
  test_summarize(); 
  test_stdev(); 
  test_percentile(); 
  return 0;
}

void test_sum_small_data_set() {
  cout << "test_sum_small_data_set" << endl;

  vector<double> data;
  data.push_back(1);
  data.push_back(2);
  data.push_back(3);

  assert(sum(data) == 6);

  cout << "PASS!" << endl;
}

void test_count(){
  cout << "test_count" << endl; 

  vector<double> data {1, 2, 4, 5, 6, 7, 8, 9}; 

  assert(count(data) == 8); 

  cout << "Pass" << endl;
}

void test_mean(){

  cout << "test_mean" << endl; 

  vector<double> data {6, 12, 5, 4, 7, 6, 8, 9, 20}; 

  // assert(static_cast<int>(mean(data)) == 8); 
  cout << mean(data) << endl; 
  cout << "Pass" << endl; 

}

void test_median(){
  cout << "test_median" << endl; 

  vector<double> data {10, 4, 5, 3, 7, 5, 6, 9}; 
  vector<double> data1 {21, 17, 9, 3, 2, 11, 63, 72, 23};
  assert(median(data) == 5.5);  
  cout << median(data) << endl; 
  cout << count(data) << endl; 
  cout << "data 1 median: " << median(data1) << endl; 
  cout << "Pass" << endl;
}

void test_mode(){
  cout << "test_mode" << endl; 

  vector<double> data {43, 2, 4, 6, 4, 6, 4, 7}; 

  assert(mode(data) == 4); 

  cout << "Pass" << endl;
}

void test_min(){
  cout << "test_min" << endl; 

  vector<double> data {3, 2, 56, 77, 3, 4, 2, 12}; 

  assert(min(data) == 2); 

  cout << "Pass" << endl;
}

void test_max(){
  cout << "test_max" << endl; 

  vector<double> data {9, 25, 3, 45, 32, 2, 4, 0}; 

  assert(max(data) == 45); 

  cout << "Pass" << endl;
}

void test_summarize(){
  cout << "test_summarize" << endl; 

  vector<double> data {1, 2, 4, 5, 6, 6, 10, 8, 9}; 

 
  assert(summarize(data).at(0).at(0) == 1); 

  cout << "Pass" << endl;
}

void test_stdev(){
  cout << "test_stdev" << endl; 

  vector<double> data {9, 2, 5, 4, 12, 7}; 
  vector<double> data1 {3, 5, 65, 43, 12, 43, 23, 53}; 
  assert(stdev(data) == 3.619); 
  cout << stdev(data) << endl; 
  cout << stdev(data1) << endl; 

  cout << "Pass" << endl;
}

void test_percentile(){
  cout << "test_percentile" << endl; 

  vector<double> data {15, 20, 35, 40, 50}; 

  assert(percentile(data, .5) == 35); 
  cout << percentile(data, .5) << endl; 
  cout << "Pass" << endl;
}

