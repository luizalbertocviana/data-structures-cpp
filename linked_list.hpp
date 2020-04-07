#pragma once
// smart pointers
#include <memory>
// optional type
#include <optional>

template<typename Key, typename Val>
class LinkedList{
private:
  struct Node{
    Key key;
    Val val;
    // pointer to the next node
    std::unique_ptr<Node> next;

    Node(Key k, Val v) : key{k},
                         val{v},
                         next{nullptr}
    {}
  };
  static std::pair<Node*, Node*> nodesWithExistingKey(Node* node, Key key){
    Node* previousNode = nullptr;
    while (key != node->key){
      previousNode = node;
      node = node->next.get();
    }

    return std::make_pair(previousNode, node);
  }

  std::unique_ptr<Node> first;
protected:
  void pushNonexistingKey(Key key, Val val){
    std::unique_ptr<Node> newNode{std::make_unique<Node>(key, val)};
    newNode->next = std::move(first);
    first = std::move(newNode);
  }
public:
  LinkedList() : first{nullptr}
  {}
  LinkedList(Key key, Val val) : first{std::make_unique<Node>(key, val)}
  {}
  bool isEmpty() const{
    return first == nullptr;
  }
  std::optional<Val> search(Key key) const{
    Node* currentNode = first.get();

    while (currentNode){
      if (key == currentNode->key){
        return currentNode->val;
      }
      else{
        currentNode = currentNode->next.get();
      }
    }

    return {};
  }
  bool push(Key key, Val val){
    if (search(key)){
      return false;
    }
    else{
      pushNonexistingKey(key, val);

      return true;
    }
  }
  std::optional<std::pair<Key, Val>> pop(){
    if (isEmpty()){
      return {};
    }
    else{
      auto keyval = std::make_pair(first->key, first->val);
      first = std::move(first->next);

      return keyval;
    }
  }
  bool insertAfter(Key refKey, Key key, Val val){
    if (search(refKey) && !search(key)){
      std::unique_ptr<Node> newNode{std::make_unique<Node>(key, val)};

      auto[previousNode, currentNode] = nodesWithExistingKey(first.get(), refKey);

      newNode->next = std::move(currentNode->next);
      currentNode->next = std::move(newNode);
      
      return true;
    }
    else{
      return false;
    }
  }
  std::optional<Val> remove(Key key){
    if (search(key)){
      auto[previousNode, currentNode] = nodesWithExistingKey(first.get(), key);
      Val deletedVal = currentNode->val;

      if (previousNode){
        previousNode->next = std::move(currentNode->next);
      }
      else{
        first = std::move(currentNode->next);
      }

      return deletedVal;
    }
    else{
      return {};
    }
  }
};
