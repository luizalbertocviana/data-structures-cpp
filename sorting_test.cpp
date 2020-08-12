#include <cassert>
#include <string>
#include <vector>

// #define debug

#ifdef debug
#include <iostream>
#endif

#include <sorting.hpp>

void bubbleSortTest(){
  std::vector<int> vec{2, 1, 4, 3, 7, 4, 6, 4, 9};
  std::vector<int> vec2{1, 2, 3, 4, 4, 4, 6, 7, 9};

  bubbleSort(vec);

  #ifdef debug
  for (auto e : vec){
    std::cout << e << std::endl;
  }
  #endif
  
  assert(vec == vec2);

  std::vector<std::string> vec3{"ccc", "bbb", "aaa"};
  std::vector<std::string> vec4{"aaa", "bbb", "ccc"};
  
  bubbleSort(vec3);

  assert(vec3 == vec4);
}

void insertionSortTest(){
  std::vector<int> vec{2, 1, 4, 3, 7, 4, 6, 4, 9};
  std::vector<int> vec2{1, 2, 3, 4, 4, 4, 6, 7, 9};

  insertionSort(vec);

  #ifdef debug
  for (auto e : vec){
    std::cout << e << std::endl;
  }
  #endif
  
  assert(vec == vec2);

  std::vector<std::string> vec3{"ccc", "bbb", "aaa"};
  std::vector<std::string> vec4{"aaa", "bbb", "ccc"};
  
  insertionSort(vec3);

  assert(vec3 == vec4);
}

void quicksortTest(){
  std::vector<int> vec{2, 1, 4, 3, 7, 4, 6, 4, 9};
  std::vector<int> vec2{1, 2, 3, 4, 4, 4, 6, 7, 9};

  quicksort(vec, 0, vec.size());

  #ifdef debug
  for (auto e : vec){
    std::cout << e << std::endl;
  }
  #endif
  
  assert(vec == vec2);

  std::vector<std::string> vec3{"ccc", "bbb", "aaa"};
  std::vector<std::string> vec4{"aaa", "bbb", "ccc"};
  
  quicksort(vec3, 0, vec3.size());

  assert(vec3 == vec4);
}

void mergesortTest(){
  std::vector<int> vec{2, 1, 4, 3, 7, 4, 6, 4, 9};
  std::vector<int> vec2{1, 2, 3, 4, 4, 4, 6, 7, 9};

  auto vec3 = mergesort(vec);

  #ifdef debug
  for (auto e : vec3){
    std::cout << e << std::endl;
  }
  #endif
  
  assert(vec3 == vec2);

  std::vector<std::string> vec4{"ccc", "bbb", "aaa"};
  std::vector<std::string> vec5{"aaa", "bbb", "ccc"};

  auto vec6 = mergesort(vec4);

  assert(vec6 == vec5);
}

int main(){
  bubbleSortTest();
  insertionSortTest();
  mergesortTest();
  quicksortTest();

  return 0;
}
