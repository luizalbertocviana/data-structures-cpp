#include <cassert>

#include <Matrix.hpp>

int main(){
  Matrix<bool> m {3, 4};

  m.at(0,0) = false;
  m.at(1,1) = true;

  assert(m.at(0,0) == false);
  assert(m.at(1,1) == true);

  return 0;
}
