#include <cassert>

#include <hash_table.hpp>

void test_int_double(){
  HashTable<int, double> ht {};

  assert(ht.search(2) == std::nullopt);

  ht.insert(1, 10.0);
  ht.insert(2, 20.0);
  ht.insert(3, 30.0);
  ht.insert(4, 40.0);
  ht.insert(5, 50.0);
  ht.insert(6, 60.0);
  ht.insert(7, 70.0);
  ht.insert(8, 80.0);
  ht.insert(9, 90.0);
  ht.insert(10, 100.0);

  assert(*ht.search(1) == 10.0);
  assert(*ht.search(2) == 20.0);
  assert(*ht.search(3) == 30.0);
  assert(*ht.search(4) == 40.0);
  assert(*ht.search(5) == 50.0);
  assert(*ht.search(6) == 60.0);
  assert(*ht.search(7) == 70.0);
  assert(*ht.search(8) == 80.0);
  assert(*ht.search(9) == 90.0);
  assert(*ht.search(10) == 100.0);

  assert(*ht.remove(1) == 10.0);
  assert(ht.remove(1) == std::nullopt);
  assert(ht.search(1) == std::nullopt);
  assert(*ht.search(2) == 20.0);
  assert(*ht.search(3) == 30.0);
  assert(*ht.search(4) == 40.0);
  assert(*ht.search(5) == 50.0);
  assert(*ht.search(6) == 60.0);
  assert(*ht.search(7) == 70.0);
  assert(*ht.search(8) == 80.0);
  assert(*ht.search(9) == 90.0);
  assert(*ht.search(10) == 100.0);

  assert(*ht.remove(2) == 20.0);
  assert(ht.remove(2) == std::nullopt);
  assert(ht.search(2) == std::nullopt);
  assert(*ht.search(3) == 30.0);
  assert(*ht.search(4) == 40.0);
  assert(*ht.search(5) == 50.0);
  assert(*ht.search(6) == 60.0);
  assert(*ht.search(7) == 70.0);
  assert(*ht.search(8) == 80.0);
  assert(*ht.search(9) == 90.0);
  assert(*ht.search(10) == 100.0);

  assert(*ht.remove(7) == 70.0);
  assert(ht.remove(7) == std::nullopt);
  assert(ht.search(7) == std::nullopt);
  assert(*ht.search(3) == 30.0);
  assert(*ht.search(4) == 40.0);
  assert(*ht.search(5) == 50.0);
  assert(*ht.search(6) == 60.0);
  assert(*ht.search(8) == 80.0);
  assert(*ht.search(9) == 90.0);
  assert(*ht.search(10) == 100.0);

  assert(*ht.remove(5) == 50.0);
  assert(ht.remove(5) == std::nullopt);
  assert(ht.search(5) == std::nullopt);
  assert(*ht.search(3) == 30.0);
  assert(*ht.search(4) == 40.0);
  assert(*ht.search(6) == 60.0);
  assert(*ht.search(8) == 80.0);
  assert(*ht.search(9) == 90.0);
  assert(*ht.search(10) == 100.0);

  assert(*ht.remove(10) == 100.0);
  assert(ht.remove(10) == std::nullopt);
  assert(ht.search(10) == std::nullopt);
  assert(*ht.search(3) == 30.0);
  assert(*ht.search(4) == 40.0);
  assert(*ht.search(6) == 60.0);
  assert(*ht.search(8) == 80.0);
  assert(*ht.search(9) == 90.0);

  assert(*ht.remove(8) == 80.0);
  assert(ht.remove(8) == std::nullopt);
  assert(ht.search(8) == std::nullopt);
  assert(*ht.search(3) == 30.0);
  assert(*ht.search(4) == 40.0);
  assert(*ht.search(6) == 60.0);
  assert(*ht.search(9) == 90.0);
}

int main(){
  test_int_double();

  return 0;
}
