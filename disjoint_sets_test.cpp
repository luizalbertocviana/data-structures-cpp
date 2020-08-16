#include <cassert>

#include <disjoint_sets.hpp>

int main(){
  unsigned long n {10};

  DisjointSet ds {n};

  assert(ds.size() == n);

  for (unsigned long i {0}; i < n; ++i){
    assert(ds.representative(i) == i);
  }

  ds.join(0, 9);
  assert(ds.representative(0) == ds.representative(9));

  ds.join(0, 3);
  assert(ds.representative(0) == ds.representative(3));
  assert(ds.representative(9) == ds.representative(3));
  assert(ds.representative(3) == ds.representative(9));

  assert(!ds.join(3, 9));

  ds.join(2, 9);

  assert(ds.representative(0) == ds.representative(2));

  ds.join(5, 6);
  assert(ds.representative(5) == ds.representative(6));
  assert(ds.representative(6) != ds.representative(9));

  return 0;
}
