#pragma once

// for swap function
#include <algorithm>
// vector type
#include <vector>

template<typename Collection>
void bubbleSort(Collection& vec){
  for (auto i = vec.size() - 1; i > 0; i--){
    for (unsigned long j = 0; j < i; j++){
      if (vec[j] > vec[j + 1]){
        std::swap(vec[j], vec[j + 1]);
      }
    }
  }
}

template<typename Collection>
void insertionSort(Collection& vec){
  for (auto i = vec.size() - 1; i > 0; i--){
    auto j = i - 1;

    while (j < vec.size() - 1 && vec[j] > vec[j + 1]){
      std::swap(vec[j], vec[j + 1]);
      j++;
    }
  }
}

std::vector<int> merge(const std::vector<int>& vecA, const std::vector<int>& vecB){
  std::vector<int> result{};

  unsigned long i = 0;
  unsigned long j = 0;

  while (i < vecA.size() && j < vecB.size()){
    if (vecA[i] < vecB[j]){
      result.push_back(vecA[i]);
      i++;
    }
    else{
      result.push_back(vecB[j]);
      j++;
    }
  }

  if (i == vecA.size()){
    while (j < vecB.size()){
      result.push_back(vecB[j]);
      j++;
    }
  }
  else{
    while (i < vecA.size()){
      result.push_back(vecA[i]);
      i++;
    }
  }

  return result;
}

std::pair<std::vector<int>, std::vector<int>> split(const std::vector<int>& vec){
  std::vector<int> splitA{};
  std::vector<int> splitB{};

  bool insertOnA = true;

  for (auto e : vec){
    if (insertOnA){
      splitA.push_back(e);

      insertOnA = false;
    }
    else{
      splitB.push_back(e);

      insertOnA = true;
    }
  }

  return std::make_pair(splitA, splitB);
}

std::vector<int> mergesort(const std::vector<int>& vec){
  if (vec.size() > 1){
    auto[splitA, splitB] = split(vec);

    auto sortedA = mergesort(splitA);
    auto sortedB = mergesort(splitB);

    return merge(sortedA, sortedB);
  }
  else{
    return vec;
  }
}
