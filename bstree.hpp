// each compilation session must consider this file at most once
#pragma once
// we are going to use smart pointer facilities
#include <memory>
// this type is helpful to represent optional value returning
#include <optional>
// stack data structure
#include <stack>
// generic types
template<typename Key, typename Val>
class BSTree{
protected:
  // implements BSTree but does not define which node type is going to
  // be used
  template<typename Node>
  class BSTreeWithNode{
  protected:
    // this method should be called only for keys contained in BSTree.
    // Removes Key and corresponding attached Val.  In case deleted node
    // is not root, returns key of node whose child has been deleted
    std::optional<Key> removeExistingKey(Key key){
      // raw pointers to the node to be deleted and its parent
      Node* currentNode = nullptr;
      Node* parentNode  = nullptr;
      // the only intent of this scope is to allow path to die quickly
      {
        // here we have the path from root node to node containing key
        std::stack<Node*> path = pathToExistingKey(key);
        // node that should be deleted is at the top of path...
        currentNode = path.top();
        // ... and its parent is right below ...
        path.pop();
        // ... if it has a parent, of course
        if (!path.empty()){
          parentNode = path.top();
        }
        // and here path dies, so we are not wasting memory
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
        // currentNode is root, so we simply deallocate Node
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

        std::optional<Key> parentKey = removeExistingKey(leftMaxKey);

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
    // returns a stack containing pointers to tree nodes that form a
    // path to an existing key of the tree.  Don't call this before
    // verifying key is present in the tree
    std::stack<Node*> pathToExistingKey(Key key) const {
      // a stack of references to Node
      std::stack<Node*> stk{};
      // raw pointer to perform a traversal
      Node* currentNode = root.get();

      // if current key is not what we want, we go down
      while (key != currentNode->key){
        // store reference to currentNode...
        stk.push(currentNode);
        // ... and go down accordingly
        if (key < currentNode->key){
          currentNode = currentNode->left.get();
        }
        else if (key > currentNode->key){
          currentNode = currentNode->right.get();
        }
      }
      // we put the destination node on top of stk
      stk.push(currentNode);

      return stk;
    }
  private:
    // root node of BSTree
    std::unique_ptr<Node> root;
  public:
    // constructor to create an empty BSTree
    BSTreeWithNode() : root{nullptr}
    {}
    // constructor to create a BSTree rooted by Key and Val
    BSTreeWithNode(Key key, Val val) : root{std::make_unique<Node>(key, val)}
    {}
    // returns whether tree is empty
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
      // if tree is not empty ...
      if (root){
        // ... we traverse the tree looking for key until an empty
        // subtree is reached
        Node* curremtNoode = root.get();
        while (curremtNoode){
          // either we find key ...
          if (key == curremtNoode->key){
            return curremtNoode->val;
          }
          // ... or we go down accordingly
          else{
            if (key < curremtNoode->key){
              curremtNoode = curremtNoode->left.get();
            }
            else if (key > curremtNoode->key){
              curremtNoode = curremtNoode->right.get();
            }
          }
        }
        // if we exit the while loop, we have reached an empty
        // subtree, so we return nothing
        return {};
      }
      // we return nothing in case tree is empty
      else{
        return {};
      }
    }
    // inserts Val attached to Key in case Key is not present
    // yet. Return value indicates whether insertion really took place
    bool insert(Key key, Val val){
      // if tree is not empty ...
      if (root){
        // we perform a traversal until we reach an empty subtree
        Node* currentNode = root.get();
        while (currentNode){
          // if key is present, no insertion is made
          if (key == currentNode->key){
            return false;
          }
          // otherwise we go down
          else{
            if (key < currentNode->key){
              // either we recursively insert into a nonempty subtree
              // ...
              if (currentNode->left){
                currentNode = currentNode->left.get();
              }
              // ... or we perform insertion in an empty subtree
              else{
                currentNode->left = std::make_unique<Node>(key, val);
              }
            }
            // the same goes for right descent
            else if (key > currentNode->key){
              if (currentNode->right){
                currentNode = currentNode->right.get();
              }
              else{
                currentNode->right = std::make_unique<Node>(key, val);
              }
            }
          }
        }
        // if the while has not returned, then insertion has occurred
        return true;
      }
      // if tree is empty, we insert at root
      else{
        root = std::make_unique<Node>(key, val);
        return true;
      }
    }
    // removes Key and corresponding attached Val. Return value
    // indicates whether removal really took place
    bool remove(Key key){
      // first we verify whether key is present in the tree
      bool hasKey = search(key);
      // in case it is, we call removeExistingKey to do the proper removal.
      // Notice that removeExistingKey should only be called for existing keys
      if (hasKey){
        removeExistingKey(key);

        return true;
      }
      // in case it is not, no removal takes place
      else{
        return false;
      }
    }
  };
private:
  // node of BSTree
  struct BSTreeNode{
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
    // returns Key Val pair whose Key is maximum.  We need this
    // information on every subtree due to the removal algorithm
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
  };
  // A binary search tree with the proper type of node
  BSTreeWithNode<BSTreeNode> bst;
public:
  // constructor to create an empty BSTree
  BSTree() : bst{}
  {}
  // constructor to create a BSTree rooted by Key and Val
  BSTree(Key key, Val val) : bst{key, val}
  {}
  bool isEmpty() const {
    return bst.isEmpty();
  }
  // returns Key Val pair whose Val corresponds to the maximum BSTree
  // Key
  std::optional<std::pair<Key, Val>> maxKey() const {
    return bst.maxKey();
  }
  // returns Key Val pair whose Val corresponds to the minimum BSTree
  // Key
  std::optional<std::pair<Key, Val>> ninKey() const {
    return bst.ninKey();
  }
  // searches for Key, returning the corresponding Value or nothing
  std::optional<Val> search(Key key) const {
    return bst.search(key);
  }
  // inserts Val attached to Key in case Key is not present
  // yet. Return value indicates whether insertion really took place
  bool insert(Key key, Val val){
    return bst.insert(key, val);
  }
  // removes Key and corresponding attached Val. Return value
  // indicates whether removal really took place
  bool remove(Key key){
    return bst.remove(key);
  }
};
