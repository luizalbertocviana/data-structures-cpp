#include <cassert>

#include <queue.hpp>

int main(){
  Queue<int> queue{};
  assert(queue.isEmpty());

  for (int e = 0; e < 10; e++){
    queue.insert(e);
  }

  for (int e = 0; e < 5; e++){
    assert(*queue.remove() == e);
  }

  assert(!queue.isEmpty());

  for (int e = 10; e < 15; e++){
    queue.insert(e);
  }

  for (int e = 5; e < 15; e++){
    assert(*queue.remove() == e);
  }

  return 0;
}
