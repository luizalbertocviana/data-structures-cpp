#pragma once
// optional type
#include <optional>

#include <stack.hpp>

template<typename Element>
class Queue{
private:
  Stack<Element> inStk;
  Stack<Element> outStk;
public:
  Queue() : inStk{},
            outStk{}
  {}
  Queue(Element e) : inStk{e},
                     outStk{}
  {}
  bool isEmpty(){
    return inStk.isEmpty() && outStk.isEmpty();
  }
  void insert(Element e){
    inStk.push(e);
  }
  std::optional<Element> remove(){
    if (isEmpty()){
      return {};
    }
    else{
      if (outStk.isEmpty()){
        while (!inStk.isEmpty()){
          auto e = inStk.pop();
          outStk.push(*e);
        }
      }

      return outStk.pop();
    }
  }
};
