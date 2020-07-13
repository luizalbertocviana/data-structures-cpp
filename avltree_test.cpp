#include <cassert>

#include <avltree.hpp>

#include <string>

void test1(){
  AVLTree<int, std::string> avlt{};

  avlt.insert(4, "eita");
  avlt.insert(3, "haha");
  avlt.insert(2, "hehe");
  avlt.insert(1, "hihi");

  assert(*avlt.search(4) == "eita");
  assert(*avlt.search(3) == "haha");
  assert(*avlt.search(2) == "hehe");
  assert(*avlt.search(1) == "hihi");
}

void test2(){
  AVLTree<int, std::string> avlt{};

  avlt.insert(1, "eita");
  avlt.insert(2, "haha");
  avlt.insert(3, "hehe");
  avlt.insert(4, "hihi");

  assert(*avlt.search(4) == "hihi");
  assert(*avlt.search(3) == "hehe");
  assert(*avlt.search(2) == "haha");
  assert(*avlt.search(1) == "eita");
}

void test3(){
  AVLTree<int, std::string> avlt{};

  avlt.insert(3, "eita");
  avlt.insert(2, "haha");
  avlt.insert(4, "hehe");
  avlt.insert(1, "hihi");

  assert(*avlt.search(4) == "hehe");
  assert(*avlt.search(3) == "eita");
  assert(*avlt.search(2) == "haha");
  assert(*avlt.search(1) == "hihi");

  avlt.remove(4);

  assert(avlt.search(4) == std::nullopt);
  assert(*avlt.search(3) == "eita");
  assert(*avlt.search(2) == "haha");
  assert(*avlt.search(1) == "hihi");
}

int main(){
  test1();
  test2();
  test3();
}
