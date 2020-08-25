#include <cassert>
#include <string>

// #define debug

#include <btree.hpp>

#ifdef debug
#include <iostream>
#endif

int main(){
  BTree<int, std::string, 3> btree{};
  #ifdef debug
  std::cout << "created empty tree" << std::endl;
  #endif

  assert(btree.isEmpty());

  std::string letter{"a"};
  for (int i = 0; i < 26; i++){
    btree.insert(i, letter);
    letter[0]++;
    #ifdef debug
    std::cout << "inserted key " << i << std::endl;
    #endif
  }

  letter[0] = 'a';
  for ( int i = 0; i < 26; i++){
    assert(*btree.search(i) == letter);
    letter[0]++;
  }
  
  return 0;
}
