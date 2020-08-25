#include <cassert>

#include <string>

#include <linked_list.hpp>

int main(){
  LinkedList<int, std::string> ll{};
  assert(ll.isEmpty());

  ll.push(3, "aaa");
  ll.push(4, "bbb");
  ll.push(8, "hhh");
  assert(*ll.search(3) == "aaa");
  assert(*ll.search(4) == "bbb");
  assert(*ll.remove(8) == "hhh");

  ll.insertAfter(4, 2, "ccc");
  ll.insertAfter(3, 5, "ddd");

  ll.pop();
  assert(!ll.isEmpty());
  assert(*ll.search(3) == "aaa");
  assert(*ll.search(2) == "ccc");
  assert(*ll.search(5) == "ddd");
  assert(ll.search(4) == std::nullopt);
  assert(*ll.remove(5) == "ddd");
  assert(ll.search(5) == std::nullopt);

  ll.pop();
  assert(!ll.isEmpty());
  assert(*ll.search(3) == "aaa");
  assert(ll.search(4) == std::nullopt);
  assert(ll.search(2) == std::nullopt);

  return 0;
}
