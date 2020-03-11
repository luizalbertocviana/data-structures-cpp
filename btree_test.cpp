#include <cassert>

#include <btree.hpp>

int main(){
  BTree<int, int, 3> btree{};

  assert(btree.isEmpty());
  
  return 0;
}
