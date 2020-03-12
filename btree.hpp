#pragma once

// array type
#include <array>
// smart pointers
#include <memory>
// optional type
#include <optional>
#ifdef debug
#include <iostream>
#endif

// generic b tree
template<typename Key, typename Val, unsigned int t>
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
      unsigned int i = 0;
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
        #ifdef debug
        std::cout << "conditions were met for splitting child " << childIndex << std::endl;
        #endif
        // a reference to the child to be split
        auto& splittingChild = child[childIndex];
        #ifdef debug
        std::cout << "took splittingChild reference" << std::endl;
        #endif
        // its new "right" sibling
        std::unique_ptr<Page> newSibling = std::make_unique<Page>();
        #ifdef debug
        std::cout << "created newSibling" << std::endl;
        #endif
        // newSibling is a leaf iff splittingChild is a leaf
        newSibling->leaf = splittingChild->leaf;
        // newSibling will be "on the right side" of splittingChild,
        // so newSibling receives the last t - 1 Key Val pairs of
        // splittingChild
        newSibling->numberKeys = t - 1;
        for (unsigned int i = 0; i < t - 1; i++){
          newSibling->key[i] = splittingChild->key[t + i];
          newSibling->val[i] = splittingChild->val[t + i];
        }
        #ifdef debug
        std::cout << "copied key val pairs from splittingChild to newSibling" << std::endl;
        #endif
        // if newSibling is not a leaf, it also receives some
        // children from splittingChild
        if (!newSibling->leaf){
          #ifdef debug
          std::cout << "newSibling is not leaf" << std::endl;
          #endif
          for (unsigned int i = 0; i < t; i++){
            newSibling->child[i] = std::move(splittingChild->child[t + i]);
          }
          #ifdef debug
          std::cout << "newSibling is not leaf. Moving some children" << std::endl;
          #endif
        }
        // splittingChild has lost its last t - 1 keys, and its median
        // key will go up to its parent.  This way, as it was a full
        // page, splittingChild had 2t - 1 keys, and after this
        // operation it will have only its first t - 1 keys
        splittingChild->numberKeys = t - 1;
        // we make room for newSibling at position childIndex + 1 ...
        for (unsigned int i = numberKeys; i >= childIndex + 1; i--){
          child[i + 1] = std::move(child[i]);
        }
        #ifdef debug
        std::cout << "moved some children to the right" << std::endl;
        #endif
        // ... and put it right there
        child[childIndex + 1] = std::move(newSibling);
        #ifdef debug
        std::cout << "put newSibling into its position" << std::endl;
        #endif
        // now we make room for the median key (and its val) of splittingChild at
        // position childIndex of this node ...
        if (numberKeys > 0){
          for(unsigned int i = numberKeys - 1; i >= childIndex; i--){
            #ifdef debug
            std::cout << "moving pair from position " << i << " to position " << i + 1 << std::endl;
            #endif
            key[i + 1] = key[i];
            val[i + 1] = val[i];
          }
        }
        #ifdef debug
        std::cout << "moved some key val pairs to the right" << std::endl;
        #endif
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
  // inserts key val pair on nonfull page
  static void insertOnNonfullPage(Page* page, Key key, Val val){
    // index of "rightest" key
    int i = page->numberKeys - 1;
    // if page is a nonfull leaf, we do the insertion
    if (page->leaf){
      // while we search for the appropriate place to put key and val,
      // we also make room for them
      while (i >= 0 && key < page->key[i]){
        page->key[i + 1] = page->key[i];
        page->val[i + 1] = page->val[i];
        i--;
      }
      // we put key and val into their appropriate places ...
      page->key[i + 1] = key;
      page->val[i + 1] = val;
      // ... and update numberKeys
      page->numberKeys++;
    }
    // if page is not a leaf, we recurse to its appropriate child
    else{
      // looking for the child index to recurse
      while (i >= 0 && key < page->key[i]){
        i--;
      }
      // when we exit the while loop,
      // page->key[i] <= key < page->key[i+1], so insertion will
      // recurse into page->child[i + 1]
      i++;
      // before going down, we verify whether page->child[i] is full.
      // In case it is, we split it ...
      if (page->child[i]->isFull()){
        page->splitChild(i);
        // ... and see if the insertion should take place in the newly
        // created sibling, that is, we see if key is greater than the
        // median key that just went up from page->child[i] to page
        if (key > page->key[i]){
          i++;
        }
      }
      // finally, we perform the recursive insertion
      insertOnNonfullPage(page->child[i].get(), key, val);
    }
  }
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
    #ifdef debug
    std::cout << "inserting key " << key << std::endl;
    #endif
    if (search(key)){
      #ifdef debug
      std::cout << "key " << key << " already inserted" << std::endl;
      #endif
      return false;
    }
    // otherwise we do the insertion
    else{
      // if tree is not empty
      if (root){
        // if root is full ...
        if (root->isFull()){
          #ifdef debug
          std::cout << "root is full. Splitting root" << std::endl;
          #endif
          // ... we create a new root, ...
          std::unique_ptr<Page> newRoot = std::make_unique<Page>();
          #ifdef debug
          std::cout << "created newRoot" << std::endl;
          #endif
          // ... make it the parent of the old root, ...
          newRoot->child[0] = std::move(root);
          #ifdef debug
          std::cout << "moved root as child 0 of newRoot" << std::endl;
          #endif
          // ... update root pointer, ...
          root = std::move(newRoot);
          #ifdef debug
          std::cout << "replaced root with newRoot" << std::endl;
          #endif
          // and split the old root
          root->splitChild(0);
          #ifdef debug
          std::cout << "split the old root (child 0 of root)" << std::endl;
          #endif
        }
        // here root is certain to be nonfull, so we make the insertion
        insertOnNonfullPage(root.get(), key, val);
      }
      // if there is no root
      else{
        #ifdef debug
        std::cout << "no root yet" << std::endl;
        std::cout << "creating root" << std::endl;
        #endif
        // we make a new one ...
        root = std::make_unique<Page>();
        // ... which is surely a leaf ...
        root->leaf = true;
        // ... and then we perform the insertion
        insertOnNonfullPage(root.get(), key, val);
      }
      // in either case, insertion took place
      return true;
    }
  }
};
