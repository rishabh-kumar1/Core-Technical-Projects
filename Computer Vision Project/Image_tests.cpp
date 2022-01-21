// Project UID af1f95f547e44c8ea88730dfb185559d

#include "Matrix.h"
#include "Image_test_helpers.h"
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
// Sets various pixels in a 2x2 Image and checks
// that Image_print produces the correct output.
TEST(test_print_basic) {
  Image *img = new Image; // create an Image in dynamic memory

  const Pixel red = {255, 0, 0};
  const Pixel green = {0, 255, 0};
  const Pixel blue = {0, 0, 255};
  const Pixel white = {255, 255, 255};

  Image_init(img, 2, 2);
  Image_set_pixel(img, 0, 0, red);
  Image_set_pixel(img, 0, 1, green);
  Image_set_pixel(img, 1, 0, blue);
  Image_set_pixel(img, 1, 1, white);
  // Capture our output
  ostringstream s;
  Image_print(img, s);
  Image_print(img, cout);

  // Correct output
  ostringstream correct;
  correct << "P3\n2 2\n255\n";
  correct << "255 0 0 0 255 0 \n";
  correct << "0 0 255 255 255 255 \n";
  ASSERT_EQUAL(s.str(), correct.str());

  delete img; // delete the Image
}



// IMPLEMENT YOUR TEST FUNCTIONS HERE
// You are encouraged to use any functions from Image_test_helpers.h as needed.
//tests width of 3x5 image, should return 3
TEST(test_image_width) {
  Image *img = new Image; // create an Image in dynamic memory

  Pixel red = {255, 0, 0};
  Pixel green = {0, 255, 0};
  Pixel blue = {0, 0, 255};
  Pixel white = {255, 255, 255};

  Image_init(img, 3, 5);
  Image_set_pixel(img, 0, 0, red);
  Image_set_pixel(img, 0, 1, green);
  Image_set_pixel(img, 1, 0, blue);
  Image_set_pixel(img, 1, 1, white);

  ASSERT_EQUAL(Image_width(img), 3);

  delete img; // delete the Image
}


//tests height of 3x5 image, should return 5
TEST(test_image_height) {
  Image *img = new Image; // create an Image in dynamic memory

  Pixel red = {255, 0, 0};
  Pixel green = {0, 255, 0};
  Pixel blue = {0, 0, 255};
  Pixel white = {255, 255, 255};

  Image_init(img, 3, 5);
  Image_set_pixel(img, 0, 0, red);
  Image_set_pixel(img, 0, 1, green);
  Image_set_pixel(img, 1, 0, blue);
  Image_set_pixel(img, 1, 1, white);

  ASSERT_EQUAL(Image_height(img), 5);

  delete img; // delete the Image
}


//tests fill with pixel with all values at 40
TEST(test_image_fill) {
  Image *img = new Image; // create an Image in dynamic memory

  int row = 3;
  int col = 5;
  Image_init(img, col, row);

  Pixel val = {40, 40, 40};
  Image_fill(img, val);

  for(int i = 0; i < row; i++){
    for(int j = 0; j < col; j++){
      ASSERT_EQUAL(Image_get_pixel(img,i,j).r, 40);
      ASSERT_EQUAL(Image_get_pixel(img,i,j).g, 40);
      ASSERT_EQUAL(Image_get_pixel(img,i,j).b, 40);
    }
  }

  delete img; // delete the Image
}


//tests the version of init taken from the istream
TEST(test_init_stream) {
  Image *img = new Image; // create an Image in dynamic memory

  const Pixel red = {255, 0, 0};
  const Pixel green = {0, 255, 0};
  const Pixel blue = {0, 0, 255};
  const Pixel white = {255, 255, 255};
  string input = "P3\n2 2\n255\n255 0 0 0 255 0 \n 0 0 255 255 255 255 \n";
  istringstream given(input);
  Image_init(img, given);
  Image_set_pixel(img, 0, 0, red);
  Image_set_pixel(img, 0, 1, green);
  Image_set_pixel(img, 1, 0, blue);
  Image_set_pixel(img, 1, 1, white);
  // Capture our output
  ostringstream s;
  Image_print(img, s);

  // Correct output
  ostringstream correct;
  correct << "P3\n2 2\n255\n";
  correct << "255 0 0 0 255 0 \n";
  correct << "0 0 255 255 255 255 \n";
  ASSERT_EQUAL(Image_height(img), 2);
  ASSERT_EQUAL(Image_width(img), 2);
  ASSERT_EQUAL(s.str(), correct.str());

  delete img; // delete the Image
}

//tests get pixel at position 1,1
TEST(test_get_pixel) {
  Image *img = new Image; // create an Image in dynamic memory

  Pixel red = {255, 0, 0};
  Pixel green = {0, 255, 0};
  Pixel blue = {0, 0, 255};
  Pixel white = {255, 255, 255};

  Image_init(img, 3, 5);
  Image_set_pixel(img, 0, 0, red);
  Image_set_pixel(img, 0, 1, green);
  Image_set_pixel(img, 1, 0, blue);
  Image_set_pixel(img, 1, 1, white);

  ASSERT_EQUAL(Image_get_pixel(img,1,0).r, 0);
  ASSERT_EQUAL(Image_get_pixel(img,1,0).g, 0);
  ASSERT_EQUAL(Image_get_pixel(img,1,0).b, 255);

  delete img; // delete the Image
}
// NOTE: The unit test framework tutorial in Lab 2 originally
// had a semicolon after TEST_MAIN(). Although including and
// excluding the semicolon are both correct according to the c++
// standard, the version of g++ we are using reports a warning
// when there is a semicolon. Therefore do NOT add a semicolon
// after TEST_MAIN()
TEST_MAIN() // Do NOT put a semicolon here
