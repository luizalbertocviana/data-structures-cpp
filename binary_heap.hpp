#include <vector>

template<typename Element>
class BinaryHeap{
private:
  std::vector<Element> data_;
public:
  BinaryHeap() : data_{}
  {}
};
