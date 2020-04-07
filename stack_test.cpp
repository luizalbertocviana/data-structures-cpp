#include <cassert>

#include <stack.hpp>

int main(){
  Stack<int> stk{};
  assert(stk.isEmpty());

  stk.push(3);
  stk.push(2);
  stk.push(1);

  assert(*stk.pop() == 1);
  assert(*stk.pop() == 2);
  assert(*stk.pop() == 3);

  return 0;
}
