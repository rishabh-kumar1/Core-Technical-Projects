// Project UID af1f95f547e44c8ea88730dfb185559d

#include "Matrix.h"
#include "Matrix_test_helpers.h"
#include "unit_test_framework.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>

using namespace std;


// Here's a free test for you! Model yours after this one.
// Test functions have no interface and thus no RMEs, but
// add a comment like the one here to say what it is testing.
// -----
// Fills a 3x5 Matrix with a value and checks
// that Matrix_at returns that value for each element.
TEST(test_fill_basic) {
  Matrix *mat = new Matrix; // create a Matrix in dynamic memory

  const int width = 3;
  const int height = 5;
  const int value = 40;
  Matrix_init(mat, 3, 5);
  Matrix_fill(mat, value);
  Matrix_print(mat,cout);
  for(int r = 0; r < height; ++r){
    for(int c = 0; c < width; ++c){
      ASSERT_EQUAL(*Matrix_at(mat, r, c), value);
    }
  }

  delete mat; // delete the Matrix
}

// ADD YOUR TESTS HERE
// You are encouraged to use any functions from Matrix_test_helpers.h as needed.
TEST(test_height) {
  Matrix *mat = new Matrix; // create a Matrix in dynamic memory
  Matrix *mat1 = new Matrix; 

  const int width = 3;
  const int height = 5;

  const int width1 = 9; 
  const int height1 = 10; 
  Matrix_init(mat, width, height);
  Matrix_init(mat1, width1, height1);
  ASSERT_EQUAL(Matrix_height(mat), 5);
  ASSERT_EQUAL(Matrix_height(mat1), 10); 
  delete mat; // delete the Matrix
  delete mat1; 
}

TEST(test_width) {
  Matrix *mat = new Matrix; // create a Matrix in dynamic memory
  Matrix *mat1 = new Matrix;
  const int width = 3;
  const int height = 5;

  const int width1 = 9; 
  const int height1 = 10; 
  Matrix_init(mat, width, height);
  Matrix_init(mat1, width1, height1);
  ASSERT_EQUAL(Matrix_width(mat), 3);
  ASSERT_EQUAL(Matrix_width(mat1), 9);
  delete mat; // delete the Matrix
  delete mat1;
}


TEST(test_at) {
  Matrix *mat = new Matrix; // create a Matrix in dynamic memory

  const int width = 3;
  const int height = 5;

  Matrix_init(mat, width, height);

  *Matrix_at(mat, 2,2) = 280;
  cout << *Matrix_at(mat,2,2) << endl;
  ASSERT_EQUAL(*Matrix_at(mat, 2,2), 280); 

  delete mat; // delete the Matrix
}

//tests to see if all values are printed correctly with matrix_at
TEST(test_print) {
  Matrix *mat1 = new Matrix; // create a Matrix in dynamic memory

  const int width = 3;
  const int height = 3;

  Matrix_init(mat1, width, height);
  Matrix_fill(mat1, 0);
  *Matrix_at(mat1, 1, 1) = 280;
  *Matrix_at(mat1, 2, 2) = 281;

  ostringstream s;
  Matrix_print(mat1, s);

  ostringstream correct; 
  correct << "3 3\n0 0 0 \n0 280 0 \n0 0 281 \n";   

  ASSERT_EQUAL(s.str(), correct.str());

  delete mat1; // delete the Matrix
}

TEST(test_fill) {
  Matrix *mat = new Matrix; // create a Matrix in dynamic memory

  const int width = 3;
  const int height = 5;
  const int value = 40;
  Matrix_init(mat, 3, 5);
  Matrix_fill(mat, value);
  Matrix_print(mat,cout);
  for(int r = 0; r < height; ++r){
    for(int c = 0; c < width; ++c){
      ASSERT_EQUAL(*Matrix_at(mat, r, c), value);
    }
  }
  delete mat;
}

TEST(test_fill_border) {
  Matrix *mat = new Matrix; // create a Matrix in dynamic memory

  const int value = 3;
  Matrix_init(mat, 10, 7);
  Matrix_fill_border(mat, value);

  for(int i = 0; i < Matrix_width(mat); i++) {
    ASSERT_EQUAL(*Matrix_at(mat, 0, i) , value);
    ASSERT_EQUAL(*Matrix_at(mat, Matrix_height(mat) - 1, i), value);
  }
  for(int i = 0; i < Matrix_height(mat); i++) {
    ASSERT_EQUAL(*Matrix_at(mat, i, 0) , value);
    ASSERT_EQUAL(*Matrix_at(mat, i, Matrix_width(mat) - 1), value);
  }
  
  delete mat;
}

TEST(test_fill_border_small){
  Matrix *mat = new Matrix;

  Matrix_init(mat, 1, 1);
  Matrix_fill_border(mat, 7);

  ASSERT_EQUAL(*Matrix_at(mat, 0, 0), 7);

  delete mat;
}

//tests to see if max value is correct with matrix_at
TEST(test_max) {
  Matrix *mat = new Matrix; 
  const int width = 7; 
  const int height = 7; 
  const int value = 133; 
  Matrix_init(mat, width, height);
  Matrix_fill(mat, value);

  ASSERT_EQUAL(Matrix_max(mat), value);
  delete mat;
}

TEST(test_max_2){
  Matrix *mat2 = new Matrix; // create a Matrix in dynamic memory
  const int width2 = 10; 
  const int height2 = 10; 
  const int value = 190000; 
  Matrix_init(mat2, width2, height2);
  Matrix_fill(mat2, 19029);
  *Matrix_at(mat2, 9, 9) = value;
  
  ASSERT_EQUAL(Matrix_max(mat2), value); 
  

  delete mat2; // delete the Matrix
}

TEST(test_row) {
  Matrix *mat1 = new Matrix; 

  const int width = 5; 
  const int height = 3;
  Matrix_init(mat1, width, height); 
  Matrix_fill(mat1, 3);
  const int* ptr = Matrix_at(mat1, 2, 3); 
  

  ASSERT_EQUAL(Matrix_row(mat1, ptr), 2);


  delete mat1; 
}

TEST(test_column) {
  Matrix *mat1 = new Matrix; 

  const int width = 5; 
  const int height = 3;
  Matrix_init(mat1, width, height); 
  Matrix_fill(mat1, 3);
  const int* ptr = Matrix_at(mat1, 1, 2); 
  
  ASSERT_EQUAL(Matrix_column(mat1, ptr), 2);


  delete mat1; 
}

TEST(test_row_col){
  Matrix *mat = new Matrix;
  
  Matrix_init(mat, 5, 5);
  Matrix_fill(mat, 3);
  const int* ptr = Matrix_at(mat, 0, 0);
  const int* ptr2 = Matrix_at(mat, 1, 1);
  const int* ptr3 = Matrix_at(mat, 2, 2);
  const int* ptr4 = Matrix_at(mat, 3, 3);
  const int* ptr5 = Matrix_at(mat, 4, 4); 

  ASSERT_EQUAL(Matrix_row(mat, ptr), 0);
  ASSERT_EQUAL(Matrix_column(mat, ptr), 0);

  ASSERT_EQUAL(Matrix_row(mat, ptr2), 1);
  ASSERT_EQUAL(Matrix_column(mat, ptr2), 1);

  ASSERT_EQUAL(Matrix_row(mat, ptr3), 2);
  ASSERT_EQUAL(Matrix_column(mat, ptr3), 2);

  ASSERT_EQUAL(Matrix_row(mat, ptr4), 3);
  ASSERT_EQUAL(Matrix_column(mat, ptr4), 3);

  ASSERT_EQUAL(Matrix_row(mat, ptr5), 4);
  ASSERT_EQUAL(Matrix_column(mat, ptr5), 4);

  delete mat;
}

TEST(test_init) {
  Matrix *mat1 = new Matrix; 
  Matrix *mat2 = new Matrix;
  Matrix *mat3 = new Matrix;
  const int width = 7; 
  const int height = 7; 

  Matrix_init(mat1, width, height); 
  Matrix_init(mat2, 1, 1);
  Matrix_init(mat3, MAX_MATRIX_WIDTH,  MAX_MATRIX_HEIGHT);

  ASSERT_EQUAL(Matrix_width(mat1), 7);
  ASSERT_EQUAL(Matrix_height(mat1), 7); 
  ASSERT_EQUAL(Matrix_width(mat2), 1);
  ASSERT_EQUAL(Matrix_height(mat2), 1); 
  ASSERT_EQUAL(Matrix_width(mat3), 500);
  ASSERT_EQUAL(Matrix_height(mat3), 500);
  

  delete mat1; 
  delete mat2;
  delete mat3;
}

TEST(test_small_border){
  Matrix *mat = new Matrix;
  
  Matrix_init(mat, 2, 2);
  Matrix_fill_border(mat, 2);

  ASSERT_EQUAL(*Matrix_at(mat, 0, 0), 2);
  ASSERT_EQUAL(*Matrix_at(mat, 0, 1), 2);
  ASSERT_EQUAL(*Matrix_at(mat, 1, 0), 2);
  ASSERT_EQUAL(*Matrix_at(mat, 1, 1), 2);

  delete mat;
}

TEST(test_max_negative){
  Matrix *mat = new Matrix;

  Matrix_init(mat, 4, 4);
  Matrix_fill(mat, -1);
  *Matrix_at(mat, 1, 1) = 1;

  ASSERT_EQUAL(Matrix_max(mat), 1);

  Matrix_fill(mat, -3);
  *Matrix_at(mat, 1, 1) = -1;
  Matrix_print(mat, cout);

  ASSERT_EQUAL(Matrix_max(mat), -1); 

  delete mat;
}

TEST(test_min_col) {
  Matrix *mat1 = new Matrix; // create a Matrix in dynamic memory

  const int width = 5;
  const int height = 3;
  const int value = 40; 
  const int row = 2; 
  const int col_start = 0; 
  const int col_end = 4; 

  Matrix_init(mat1, width, height);
  Matrix_fill(mat1, value);
  Matrix_print(mat1, cout);

  *Matrix_at(mat1, 2, 3) = 150;
  *Matrix_at(mat1, 2, 4) = 200;
  *Matrix_at(mat1, 2, 0) = 1;

  ASSERT_EQUAL(Matrix_column_of_min_value_in_row(mat1, row, col_start, col_end), 0); 
  ASSERT_EQUAL(Matrix_column_of_min_value_in_row(mat1, 1, 1, col_end), 1);
  
  delete mat1; // delete the Matrix
}

TEST(test_min_row) {
  Matrix *mat1 = new Matrix; // create a Matrix in dynamic memory

  const int width = 5;
  const int height = 3;
  const int value = 40; 
  const int row = 2; 
  const int col_start = 0; 
  const int col_end = 4; 

  Matrix_init(mat1, width, height);
  Matrix_fill(mat1, value);
  Matrix_print(mat1, cout);

  *Matrix_at(mat1, 2, 3) = 20;
  *Matrix_at(mat1, 2, 4) = 200;
  *Matrix_at(mat1, 1, 2) = 41;
  
  ASSERT_EQUAL(Matrix_min_value_in_row(mat1, row, col_start, col_end), 20);
  ASSERT_EQUAL(Matrix_min_value_in_row(mat1, 1, col_start, col_end), 40);


  delete mat1; // delete the Matrix
}
// NOTE: The unit test framework tutorial in Lab 2 originally
// had a semicolon after TEST_MAIN(). Although including and
// excluding the semicolon are both correct according to the c++
// standard, the version of g++ we are using reports a warning
// when there is a semicolon. Therefore do NOT add a semicolon
// after TEST_MAIN()
TEST_MAIN() // Do NOT put a semicolon here
