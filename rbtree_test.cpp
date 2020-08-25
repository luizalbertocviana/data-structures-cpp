// assert macro
#include <cassert>

#include <string>

// we are going to test this data structure
#include <rbtree.hpp>

int main(){
  RBTree<int, std::string> rbtree{};
  RBTree<int, std::string> rbtree2{1, "eita"};
  
  rbtree.insert(1, "eita");

  assert(*rbtree.search(1) == "eita");
  
  return 0;
}
