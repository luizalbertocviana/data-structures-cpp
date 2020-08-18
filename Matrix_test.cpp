#include <cassert>

#include <Matrix.hpp>

void test_matrix(){
  Matrix<bool> m {3, 4};

  m.at(0,0) = false;
  m.at(1,1) = true;

  assert(m.at(0,0) == false);
  assert(m.at(1,1) == true);
}

void test_square_matrix(){
  SquareMatrix<bool> m {4};

  m = true;

  m.at(0,0) = false;
  m.at(3,2) = false;

  assert(m.at(0,0) == false);
  assert(m.at(3,2) == false);
  assert(m.at(3,3) == true);
}

void test_upper_triangular_matrix(){
  UpperTriangularMatrix<bool> ut_m {4};

  ut_m.at(3,3) = false;

  ut_m.at(1,0) = true;

  assert(ut_m.at(1,0) == false);
  assert(ut_m.at(0,1) == false);

  ut_m.at(0, 1) = true;

  assert(ut_m.at(0,1) == true);

  ut_m.at(2,3) = true;

  assert(ut_m.at(2,3) == true);
}

int main(){
  test_matrix();

  test_square_matrix();

  test_upper_triangular_matrix();

  return 0;
}
