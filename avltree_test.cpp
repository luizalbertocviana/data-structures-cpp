#include <cassert>

#include <avltree.hpp>

int main(){
  AVLTree<int, std::string> avlt{1, "eita"};

  AVLTree<int, std::string> avlt2{};
  
  avlt.remove(1);
  
  return 0;
}
