#include <cassert>

#include <avltree.hpp>

int main(){
  AVLTree<int, std::string> avlt{};

  avlt.insert(1, "eita");
  avlt.insert(2, "haha");
  avlt.insert(3, "eita");
  avlt.insert(4, "dorime");
  avlt.insert(5, "ameno");

  assert(*avlt.search(1) == "eita");
  assert(*avlt.search(2) == "haha");
  assert(*avlt.search(3) == "eita");
  assert(*avlt.search(4) == "dorime");
  assert(*avlt.search(5) == "ameno");
  
  avlt.remove(1);
  
  assert(avlt.search(1) == std::nullopt);
  assert(*avlt.search(2) == "haha");
  assert(*avlt.search(3) == "eita");
  assert(*avlt.search(4) == "dorime");
  assert(*avlt.search(5) == "ameno");

  return 0;
}
