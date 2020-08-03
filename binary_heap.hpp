// ensures file is read at most once per compilation unit
#pragma once
// for comparation functions
#include <functional>
// for contiguous memory management
#include <vector>
// alias for type used to represent priority values
using priority_type = unsigned long;
// A binary max heap assgning a priority to each Element
template<typename Element, typename Comparator = std::greater<priority_type>>
class BinaryMaxHeap{
private:
  // static comparator used to determine which elements have more
  // priority
  static constexpr const Comparator cmp_ {};
  // this helps to associate elements to priorities and to write the
  // algorithms
  struct Node{
    priority_type priority;
    Element       element;
    // very simple constructor
    Node(priority_type p, Element e) : priority{p}, element{e}
    {}
    // this allows to compare Node values in terms of the instantiated
    // Comparator
    bool operator>(const Node& n){
      return cmp_(priority, n.priority);
    }
  };
  // type aliases for easy refactoring (if necessary)
  using Data       = std::vector<Node>;
  using index_type = typename Data::size_type;
  // this will hold elements and their priorities
  Data data_;
  // current number of stored elements
  index_type size_;
  // calculates parent index of node whose index is i
  static index_type parent_index_(index_type i){
    return (i - 1) / 2;
  }
  // calculates left child index of node whose index is i
  static index_type left_child_index_(index_type i){
    return 2*i + 1;
  }
  // calculates right child index of node whose index is i
  static index_type right_child_index_(index_type i){
    return 2*i + 2;
  }
  // adjusts position of node at index i according to comparisons with
  // its children. If a node is moved, its position is adjusted
  // recursively
  void heapify_down_(index_type i){
    // this will hold the index of node with highest priority among i
    // and its children
    index_type largest {i};
    // indices of i children
    index_type left  {left_child_index_(i)};
    index_type right {right_child_index_(i)};
    // determines which of the three nodes has highest priority
    if (left < size_ && data_[left] > data_[largest]){
      largest = left;
    }
    if (right < size_ && data_[right] > data_[largest]){
      largest = right;
    }
    // if a child ahs more priority than i, swap them
    if (largest != i){
      std::swap(data_[largest], data_[i]);
      // then adjust position of the moved parent, now located at
      // largest, further down
      heapify_down_(largest);
    }
  }
  // adjusts position of node at index i according to a comparison
  // with its parent. If it is moved, its position is adjusted
  // iteratively
  void heapify_up_(index_type i){
    // while i is an (upwards) valid index and it has more priority
    // than its parent, move it up
    while (i > 0 && !(data_[parent_index_(i)] > data_[i])){
      std::swap(data_[i], data_[parent_index_(i)]);

      i = parent_index_(i);
    }
  }
public:
  // basic constructor
  BinaryMaxHeap() : data_{}, size_{0}
  {}
  // returns whether heap is empty
  bool empty(){
    return size_ == 0;
  }
  // gets the element with highest priority. Returns nothing in case
  // heap is empty
  std::optional<Element> priority_element(){
    if (empty()){
      return {};
    }
    else{
      return data_[0].element;
    }
  }
  // extracts the element with highest priority. Does nothing in case
  // heap is empty
  std::optional<Element> extract(){
    if (empty()){
      return {};
    }
    else{
      // copy element at position 0, since it is about to be
      // overwritten
      Element copy {data_[0].element};
      // overwrites heap root with the leaf of maximum valid index
      data_[0] = data_[size_ - 1];
      // accounts for removal of one element
      --size_;
      // adjust position of new root
      heapify_down_(0);
      // then return copy of the extracted element
      return copy;
    }
  }
  // inserts element with priority
  void insert(priority_type priority, const Element& element){
    // data might be using all its allocated space, so use push_back
    // to ensure reallocation (if needed)
    if (size_ == data_.size()){
      data_.push_back({priority, element});
    }
    // otherwise simply put element and its priority as the leaf with
    // maximum valid index
    else{
      data_[size_] = {priority, element};
    }
    // account for insertion of element
    ++size_;
    // adjusts position of newly inserted element
    heapify_up_(size_ - 1);
  }
};
// analogous data structure where a lower priority value means higher
// priority
template<typename Element>
using BinaryMinHeap = BinaryMaxHeap<Element, std::less<priority_type>>;
