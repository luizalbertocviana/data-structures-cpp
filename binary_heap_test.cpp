#include <cassert>
#include <string>

#include <binary_heap.hpp>

void test_max(){
  BinaryMaxHeap<std::string> heap {};

  assert(heap.empty());

  heap.insert(10, "hahaha");
  heap.insert(40, "whwhwhw");
  heap.insert(25, "eita bahia");
  heap.insert(30, "aaa");
  heap.insert(70, "jjjj");
  heap.insert(20, "bbb");

  assert(!heap.empty());
  assert(*heap.extract() == "jjjj");
  assert(!heap.empty());
  assert(*heap.extract() == "whwhwhw");
  assert(!heap.empty());
  assert(*heap.extract() == "aaa");
  assert(!heap.empty());
  assert(*heap.extract() == "eita bahia");
  assert(!heap.empty());
  assert(*heap.extract() == "bbb");
  assert(!heap.empty());
  assert(*heap.extract() == "hahaha");
  assert(heap.empty());
}

void test_min(){
  BinaryMinHeap<std::string> heap {};

  assert(heap.empty());

  heap.insert(10, "hahaha");
  heap.insert(40, "whwhwhw");
  heap.insert(25, "eita bahia");
  heap.insert(30, "aaa");
  heap.insert(70, "jjjj");
  heap.insert(20, "bbb");

  assert(!heap.empty());
  assert(*heap.extract() == "hahaha");
  assert(!heap.empty());
  assert(*heap.extract() == "bbb");
  assert(!heap.empty());
  assert(*heap.extract() == "eita bahia");
  assert(!heap.empty());
  assert(*heap.extract() == "aaa");
  assert(!heap.empty());
  assert(*heap.extract() == "whwhwhw");
  assert(!heap.empty());
  assert(*heap.extract() == "jjjj");
  assert(heap.empty());
}

int main(){
  test_max();

  test_min();

  return 0;
}
