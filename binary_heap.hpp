#include <functional>
#include <vector>

using priority_type = unsigned long;

template<typename Element, typename Comparator = std::greater<priority_type>>
class BinaryMaxHeap{
private:
  static constexpr const Comparator cmp_ {};

  struct Node{
    priority_type priority;

    Element element;

    Node(priority_type p, Element e) : priority{p}, element{e}
    {}

    bool operator>(const Node& n){
      return cmp_(priority, n.priority);
    }
  };

  using Data       = std::vector<Node>;
  using index_type = typename Data::size_type;

  Data data_;

  index_type size_;

  static index_type parent_index_(index_type i){
    return (i - 1) / 2;
  }

  static index_type left_child_index_(index_type i){
    return 2*i + 1;
  }

  static index_type right_child_index_(index_type i){
    return 2*i + 2;
  }

  void heapify_down_(index_type i){
    index_type largest {i};

    index_type left  {left_child_index_(i)};
    index_type right {right_child_index_(i)};

    if (left < size_ && data_[left] > data_[largest]){
      largest = left;
    }
    if (right < size_ && data_[right] > data_[largest]){
      largest = right;
    }

    if (largest != i){
      std::swap(data_[largest], data_[i]);

      heapify_down_(largest);
    }
  }

  void heapify_up_(index_type i){
    while (i > 0 && !(data_[parent_index_(i)] > data_[i])){
      std::swap(data_[i], data_[parent_index_(i)]);

      i = parent_index_(i);
    }
  }
public:
  BinaryMaxHeap() : data_{}, size_{0}
  {}

  bool empty(){
    return size_ == 0;
  }

  std::optional<Element> priority_element(){
    if (empty()){
      return {};
    }
    else{
      return data_[0].element;
    }
  }

  std::optional<Element> extract(){
    if (empty()){
      return {};
    }
    else{
      Element copy {data_[0].element};

      data_[0] = data_[size_ - 1];

      --size_;

      heapify_down_(0);

      return copy;
    }
  }

  void insert(priority_type priority, const Element& element){
    if (size_ == data_.size()){
      data_.push_back({priority, element});
    }
    else{
      data_[size_] = {priority, element};
    }

    ++size_;

    heapify_up_(size_ - 1);
  }
};

template<typename Element>
using BinaryMinHeap = BinaryMaxHeap<Element, std::less<priority_type>>;
