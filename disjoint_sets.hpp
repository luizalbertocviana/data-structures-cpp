#pragma once
// we will use a vector to store each element representative
#include <vector>
// A disjoint set implementation: each of n elements is uniquely
// assigned to a index between 0 and n - 1, and is initially
// represented by itself. Elements with a common representative can
// therefore be seen as disjoint from other group of elements. This
// way, each element starts in a unitary disjoint set
class DisjointSet{
public:
  // alias used as index type
  using size_type = unsigned long;
private:
  // alias for our data representation: each representative is given
  // by its index, so size_type seems a reasonable choice
  using Data = std::vector<size_type>;
  // this stores representative of element i in position of index i
  Data representative_;
public:
  // constructor of DisjointSet with number_elements initial elements
  DisjointSet(size_type number_elements) : representative_{}
  {
    // allocates space for number_elements representative indices
    representative_.resize(number_elements);
    // initially, each element is represented by itself
    for (size_type i {0}; i < number_elements; ++i){
      representative_[i] = i;
    }
  }
  // returns number of elements currently being represented by DisjointSet
  size_type size() const{
    return representative_.size();
  }
  // returns index of representative of element
  size_type representative(size_type element) const{
    // if element is represented by itself, returns its own index
    if (representative_[element] == element){
      return element;
    }
    // otherwise, goes recursively in search of a representative
    else{
      return representative(representative_[element]);
    }
  }
  // adds a new element index to DisjointSet
  size_type new_element(){
    // gets number of elements currently being represented by
    // DisjointSet
    size_type number_elements {representative_.size()};
    // adds a new element (with number_elements as index) being
    // represented by itself
    representative_.push_back(number_elements);
    // returns index of newly represented element
    return number_elements;
  }
  // joins two distinct groups of elements: if element_a and element_b
  // are represented by distinct representatives, representative of
  // element_b starts to represent all elements previously represented
  // by the representative of element_a. Returns true if a join has
  // occured
  bool join(size_type element_a, size_type element_b){
    // gets the representative of each element
    size_type representative_a {representative(element_a)};
    size_type representative_b {representative(element_b)};
    // if they are the same, no join needs to occur, and this is
    // signaled to the caller
    if (representative_a == representative_b){
      return false;
    }
    // otherwise, representative_a becomes represented by
    // representative_b, and as a consequence, each element formerly
    // represented by representative_a is now represneted by
    // representative_b
    else{
      representative_[representative_a] = representative_b;
      // a join has occured, so returns true
      return true;
    }
  }
};
