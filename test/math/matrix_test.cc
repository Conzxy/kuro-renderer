#include "kuro/math/matrix.hh"

#include <gtest/gtest.h>

using namespace kuro;

TEST (matrix_test, multiple) {
  Matrix4x4f a({
    { 1, 2, 3, 4 },
    { 1, 2, 3, 4 },
    { 1, 2, 3, 4 },
    { 1, 2, 3, 4 },
  });

  Matrix4x4f b = a;
  
  std::cout << "matrix(a) = \n";
  a.Print();
  std::cout << "matrix(b) = \n";
  b.Print();
  std::cout << "matrix(a*b) = \n";
  (a * b).Print(); 
}

TEST (matrix_test, multiple2) {
  Matrix4x4f a({
      { 1, 2, 3, 4},
      { 1, 2, 3, 4},
      { 1, 2, 3, 4},
      { 1, 2, 3, 4},
      });

  Vec4f b(1, 2, 3, 4);

  (a * b).Print();
}
