// each compilation session must consider this file at most once
#pragma once

// we are going to use smart pointer facilities
#include <memory>
// this type is helpful to represent optional value returning
#include <optional>

// generic types
template<typename Key, typename Val>
class BSTree{
protected:
  // node of BSTree
  class BSTreeNode{
  public:
    // since this is an internal private class, there is no need to
    // private members
    Key key;
    Val val;
    // smart pointers to left and right subtrees: these guys deal with
    // memory deallocation by themselves
    std::unique_ptr<BSTreeNode> left;
    std::unique_ptr<BSTreeNode> right;

    // constructor: notice that member initialization is done outside
    // of the constructor body
    BSTreeNode(Key k, Val v) : key{k},
                               val{v},
                               left{nullptr},
                               right{nullptr}
    {}

    // returns Key Val pair whose Key is maximum
    std::pair<Key, Val> maxKey() const {
      if (right){
        return right->maxKey();
      }
      else{
        return std::make_pair(key, val);
      }
    }

    // returns Key Val pair whose Key is minimum
    std::pair<Key, Val> minKey() const {
      if (left){
        return left->minKey();
      }
      else{
        return std::make_pair(key, val);
      }
    }

    // searches for Key, returning a Val or nothing
    std::optional<Val> search(Key k) const {
      // current node contains requested key
      if (k == key){
        return val;
      }
      // left subtree is not empty and requested key may be at it
      else if (left && k < key){
        return left->search(k);
      }
      // the same in regard of right subtree
      else if (right && k > key){
        return right->search(k);
      }
      // if requested key cannot be found, return nothing
      return {};
    }

    // inserts Val attached to Key in case Key is not present. Return
    // value indicates whether insertion really happened
    template<typename Node>
    bool insert(Key k, Val v){
      // current node already contains Key, so does nothing
      if (k == key){
        return false;
      }
      // insertion may occur at left subtree
      else if (k < key){
        // if left subtree is not empty, recursively inserts into it
        if (left){
          return left->template insert<Node>(k, v);
        }
        // if left subtree is empty, insertion will occur
        else{
          left = std::make_unique<Node>(k, v);
        }
      }
      // same idea but applied to right subtree
      else if (k > key){
        if (right){
          return right->template insert<Node>(k, v);
        }
        else{
          right = std::make_unique<Node>(k, v);
        }
      }
      // if execution reaches this line, insertion indeed has occured,
      // so returns accordingly
      return true;
    }
  };

  // this method should be called only for keys contained in BSTree.
  // Removes Key and corresponding attached Val.  In case deleted node
  // is not root, returns key of node whose child has been deleted
  std::optional<Key> remove_(Key key){
    // raw pointers to perform a traversal
    BSTreeNode* currentNode = root.get();
    // since currentNode starts pointing towards root, it has no
    // parent node
    BSTreeNode* parentNode  = nullptr;

    // finds requested key
    while (key != currentNode->key){
      // updates parentNode
      parentNode = currentNode;
      // goes either left or right accordingly
      if (key < currentNode->key){
        currentNode = currentNode->left.get();
      }
      else if (key > currentNode->key){
        currentNode = currentNode->right.get();
      }
    }
    // currentNode has no subtrees, so it can safely be deleted
    if (currentNode->left == nullptr && currentNode->right == nullptr){
      // currentNode is not root
      if (parentNode){
        // currentNode is parentNode's left child
        if (currentNode->key < parentNode->key){
          // this assignment is enough to deallocate currentNode
          parentNode->left = nullptr;
        }
        // currentNode is parentNode's right child
        else if (currentNode->key >  parentNode->key){
          parentNode->right = nullptr;
        }

        return parentNode->key;
      }
      // currentNode is root, so we simply deallocate BSTreeNode
      // at root
      else{
        root = nullptr;

        return {};
      }
    }
    // currentNode has both subtrees not empty
    else if (currentNode->left && currentNode->right){
      // we could also have taken currentNode->right->minKey
      auto[leftMaxKey, leftMaxVal] = currentNode->left->maxKey();

      std::optional<Key> parentKey = remove_(leftMaxKey);

      currentNode->key = leftMaxKey;
      currentNode->val = leftMaxVal;

      return parentKey;
    }
    // currentNode has exactly one subtree not empty
    else{
      // currentNode is not root
      if (parentNode){
        // currentNode is parentNode's left child
        if (currentNode->key < parentNode->key){
          if (currentNode->left){
            // notice how we don't copy the unique_ptr. We move it
            // instead
            parentNode->left = std::move(currentNode->left);
          }
          else if (currentNode->right){
            parentNode->left = std::move(currentNode->right);
          }
        }
        // currentNode is parentNode's right child
        else if (currentNode->key > parentNode->key){
          if (currentNode->left){
            parentNode->right = std::move(currentNode->left);
          }
          else if (currentNode->right){
            parentNode->right = std::move(currentNode->right);
          }
        }

        return parentNode->key;
      }
      // currentNode is root, so we update root to be its only
      // nonempty subtree
      else{
        if (currentNode->left){
          root = std::move(currentNode->left);
        }
        else if (currentNode->right){
          root = std::move(currentNode->right);
        }

        return {};
      }
    }
  }

private:
  // root node of BSTree
  std::unique_ptr<BSTreeNode> root;

public:
  // constructor to create an empty BSTree
  BSTree() : root{nullptr}
  {}

  // constructor to create a BSTree rooted by Key and Val
  BSTree(Key key, Val val) : root{std::make_unique<BSTreeNode>(key, val)}
  {}

  bool isEmpty() const {
    return root == nullptr;
  }

  // returns Key Val pair whose Val corresponds to the maximum BSTree
  // Key
  std::optional<std::pair<Key, Val>> maxKey() const {
    if (root){
      return root->maxKey();
    }
    else{
      return {};
    }
  }

  // returns Key Val pair whose Val corresponds to the minimum BSTree
  // Key
  std::optional<std::pair<Key, Val>> ninKey() const {
    if (root){
      return root->minKey();
    }
    else{
      return {};
    }
  }

  // searches for Key, returning the corresponding Value or nothing
  std::optional<Val> search(Key key) const {
    if (root){
      return root->search(key);
    }
    else{
      return {};
    }
  }

  // inserts Val attached to Key in case Key is not present
  // yet. Return value indicates whether insertion really took place
  bool insert(Key key, Val val){
    if (root){
      return root->template insert<BSTreeNode>(key, val);
    }
    else{
      root = std::make_unique<BSTreeNode>(key, val);
      return true;
    }
  }

  // removes Key and corresponding attached Val. Return value
  // indicates whether removal really took place
  bool remove(Key key){
    // if BSTree is not empty, we may have something to delete
    if (root){
      // raw pointers to perform a traversal
      BSTreeNode* currentNode = root.get();
      // since currentNode starts pointing towards root, it has no
      // parent node
      BSTreeNode* parentNode  = nullptr;

      // tries to find requested key. This may end up in an empty
      // subtree
      while (currentNode && key != currentNode->key){
        // updates parentNode
        parentNode = currentNode;
        // goes either left or right accordingly
        if (key < currentNode->key){
          currentNode = currentNode->left.get();
        }
        else if (key > currentNode->key){
          currentNode = currentNode->right.get();
        }
      }
      // now we must verify why we exited the while loop

      // if currentNode is not nullptr, we exited the while loop
      // because key == currentNode->key, so currentNode content must
      // be deleted
      if (currentNode){
        // currentNode has no subtrees, so it can safely be deleted
        if (currentNode->left == nullptr && currentNode->right == nullptr){
          // currentNode is not root
          if (parentNode){
            // currentNode is parentNode's left child
            if (currentNode->key < parentNode->key){
              // this assignment is enough to deallocate currentNode
              parentNode->left = nullptr;
            }
            // currentNode is parentNode's right child
            else if (currentNode->key >  parentNode->key){
              parentNode->right = nullptr;
            }
          }
          // currentNode is root, so we simply deallocate BSTreeNode
          // at root
          else{
            root = nullptr;
          }
        }
        // currentNode has both subtrees not empty
        else if (currentNode->left && currentNode->right){
          // we could also have taken currentNode->right->minKey
          auto[leftMaxKey, leftMaxVal] = currentNode->left->maxKey();

          remove(leftMaxKey);

          currentNode->key = leftMaxKey;
          currentNode->val = leftMaxVal;
        }
        // currentNode has exactly one subtree not empty
        else{
          // currentNode is not root
          if (parentNode){
            // currentNode is parentNode's left child
            if (currentNode->key < parentNode->key){
              if (currentNode->left){
                // notice how we don't copy the unique_ptr. We move it
                // instead
                parentNode->left = std::move(currentNode->left);
              }
              else if (currentNode->right){
                parentNode->left = std::move(currentNode->right);
              }
            }
            // currentNode is parentNode's right child
            else if (currentNode->key > parentNode->key){
              if (currentNode->left){
                parentNode->right = std::move(currentNode->left);
              }
              else if (currentNode->right){
                parentNode->right = std::move(currentNode->right);
              }
            }
          }
          // currentNode is root, so we update root to be its only
          // nonempty subtree
          else{
            if (currentNode->left){
              root = std::move(currentNode->left);
            }
            else if (currentNode->right){
              root = std::move(currentNode->right);
            }
          }
        }
        
        return true;
      }
      // if currentNode is nullptr, then the only subtree that could
      // contain key is empty, so no deletion is performed
      else{
        return false;
      }
    }
    // if BSTree is empty, we simply indicate nothing was deleted
    else{
      return false;
    }
  }
};
