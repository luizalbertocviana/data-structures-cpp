#pragma once
//optional type
#include <optional>

#include <linked_list.hpp>

template<typename Element>
class Stack : protected LinkedList<Element, std::nullptr_t>{
private:
  using LList = LinkedList<Element, std::nullptr_t>;
public:
  Stack() : LList{}
  {}
  Stack(Element e) : LList{e, nullptr}
  {}
  bool isEmpty(){
    return LList::isEmpty();
  }
  void push(Element e){
    LList::pushNonexistingKey(e, nullptr);
  }
  std::optional<Element> pop(){
    auto hasRemoved = LList::pop();
    if (hasRemoved){
      return (*hasRemoved).first;
    }
    else{
      return {};
    }
  }
};
