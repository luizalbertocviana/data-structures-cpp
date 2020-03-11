#pragma once

// array type
#include <array>
// smart pointers
#include <memory>
// optional type
#include <optional>

// generic b tree
template<typename Key, typename Val, std::size_t t>
class BTree{
private:
  // represents a page of BTree
  struct Page{
    // indicates whether page is a leaf
    bool leaf;
    // number of keys currently stored
    unsigned int numberKeys;
    // arrays of keys and vals
    std::array<Key, 2*t - 1> key;
    std::array<Val, 2*t - 1> val;
    // children array
    std::array<std::unique_ptr<Page>, 2*t> child;
    // constructor
    Page() : leaf{false},
             numberKeys{0},
             key{},
             val{},
             child{}
    {}
    bool isFull(){
      return numberKeys == 2*t - 1;
    }
    std::optional<Val> search(Key k){
      int i = 0;
      while (i < numberKeys && k > key[i]){
        i++;
      }
      if (i < numberKeys && k == key[i]){
        return val[i];
      }
      else if (leaf){
        return {};
      }
      else{
        return child[i]->search(k);
      }
    }
    // splits a full child of this node, if this node is nonfull.  In
    // case child index is invald or the former conditions are not
    // met, returns false
    bool splitChild(unsigned int childIndex){
      if (!isFull() && childIndex <= numberKeys && child[childIndex]->isFull()){
        // a reference to the child to be split
        auto& splittingChild = child[childIndex];
        // its new "right" sibling
        std::unique_ptr<Page> newSibling = std::make_unique<Page>();
        // newSibling is a leaf iff splittingChild is a leaf
        newSibling->leaf = splittingChild->leaf;
        // newSibling will be "on the right side" of splittingChild,
        // so newSibling receives the last t - 1 Key Val pairs of
        // splittingChild
        newSibling->numberKeys = t - 1;
        for (int i = 0; i < t - 1; i++){
          newSibling->key[i] = splittingChild->key[t + i];
          newSibling->val[i] = splittingChild->val[t + i];
        }
        // if newSibling is not a leaf, it also receives some
        // children from splittingChild
        if (!newSibling->leaf){
          for (int i = 0; i < t; i++){
            newSibling->child[i] = std::move(splittingChild->child[t + i]);
          }
        }
        // splittingChild has lost its last t - 1 keys, and its median
        // key will go up to its parent.  This way, as it was a full
        // page, splittingChild had 2t - 1 keys, and after this
        // operation it will have only its first t - 1 keys
        splittingChild->numberKeys = t - 1;
        // we make room for newSibling at position childIndex + 1 ...
        for (int i = numberKeys; i >= childIndex + 1; i--){
          child[i + 1] = std::move(child[i]);
        }
        // ... and put it right there
        child[childIndex + 1] = std::move(newSibling);
        // now we make room for the median key (and its val) of splittingChild at
        // position childIndex of this node ...
        for(int i = numberKeys - 1; i >= childIndex; i--){
          key[i + 1] = key[i];
          val[i + 1] = val[i];
        }
        // ... and put it right there
        key[childIndex] = splittingChild->key[t - 1];
        val[childIndex] = splittingChild->val[t - 1];
        // and this page just gained a new key
        numberKeys++;

        return true;
      }
      else{
        return false;
      }
    }
  };
  // root pointer
  std::unique_ptr<Page> root;
public:
  // constructor
  BTree() : root{nullptr}
  {}
  bool isEmpty(){
    return root == nullptr;
  }
  // search method
  std::optional<Val> search(Key key){
    if (root){
      return root->search(key);
    }
    else{
      return {};
    }
  }
  // insert method
  bool insert(Key key, Val val){
    // if key is present we just signal insertion did not occur
    if (search(key)){
      return false;
    }
    else{
      // if tree is not empty
      if (root){
        // if root is full ...
        if(root->isFull()){
          // ... we create a new root, ...
          std::unique_ptr<Page> newRoot = std::make_unique<Page>();
          // ... make it the parent of the old root, ...
          newRoot->child[0] = std::move(root);
          // ... update root pointer, ...
          root = std::move(newRoot);
          // and split the old root
          root->splitChild(0);
        }
        // here root is certain to be nonfull, so we make the insertion
        insertOnNonfullPage(root.get(), key, val);
      }
      // if there is no root
      else{
        // we make a new one ...
        root = std::make_unique<Page>();
        // ... which is surely a leaf ...
        root->leaf = true;
        // ... and then we perform the insertion
        insertOnNonfullPage(root.get(), key, val);
      }
      // we signal insertion took place
      return true;
    }
  }
};
