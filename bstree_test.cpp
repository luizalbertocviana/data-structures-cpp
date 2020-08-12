#include <string>
#include <vector>

#include <cassert>

#include <bstree.hpp>

int main(){
  BSTree<int, std::string> bst{};

  assert(bst.isEmpty());

  bst.insert(2, "hue");
  bst.insert(1, "eita");
  bst.insert(3, "haha");

  assert(!bst.insert(3, "eita"));
  
  assert(*bst.search(1) == "eita");
  assert(*bst.search(2) == "hue");
  assert(*bst.search(3) == "haha");
  assert(bst.search(4) == std::nullopt);
  assert(!bst.isEmpty());

  bst.remove(2);
  
  assert(*bst.search(1) == "eita");
  assert(bst.search(2) == std::nullopt);
  assert(*bst.search(3) == "haha");
  assert(bst.search(4) == std::nullopt);
  assert(!bst.isEmpty());

  bst.remove(1);

  assert(bst.search(1) == std::nullopt);
  assert(bst.search(2) == std::nullopt);
  assert(*bst.search(3) == "haha");
  assert(bst.search(4) == std::nullopt);
  assert(!bst.isEmpty());

  auto treeStr = *bst.search(3);

  bst.remove(3);

  assert(bst.search(1) == std::nullopt);
  assert(bst.search(2) == std::nullopt);
  assert(bst.search(3) == std::nullopt);
  assert(bst.search(4) == std::nullopt);
  assert(bst.isEmpty());

  std::vector<int> vec{4, 2, 1, 3, 6, 5, 7};

  BSTree<int, std::string> bst2{};

  for (int e : vec){
    bst2.insert(e, treeStr);
  }

  for (int e : vec){
    assert(bst2.search(e));
  }

  bst2.remove(4);

  assert(!bst2.search(4));
  assert(bst2.search(3));
  assert(bst2.search(7));
  assert(!bst2.search(8));
  
  return 0;
}
