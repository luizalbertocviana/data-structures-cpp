#pragma once
// for max function
#include <algorithm>
// smart pointers
#include <memory>
// optional type
#include <optional>
// stack type
#include <stack>
// we are going to inherit from BSTree
#include <bstree.hpp>
// AVLTree is a subclass of BSTree (under the same type parameters)
template<typename Key, typename Val>
class AVLTree : public BSTree<Key, Val>{
private:
  // type aliases for saving us from some typing
  using BST = BSTree<Key, Val>;
  // since BST has not been instantiated yet, we need to tell the
  // compiler that BSTreeNode is a type name
  // the node of AVLTree is derived from the node of BSTree
  struct AVLTreeNode{
    Key key;
    Val val;
    // smart pointers
    std::unique_ptr<AVLTreeNode> left;
    std::unique_ptr<AVLTreeNode> right;
    // height of node
    unsigned int height;
    // initializes AVLTreeNode.  Since it has no children, it has
    // height zero
    AVLTreeNode(Key k, Val v) : key{k},
                                val{v},
                                left{nullptr},
                                right{nullptr},
                                height{0}
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
  };
  bool updateHeight(AVLTreeNode* node){
    // get height of left and right subtrees
    int leftHeight  = node->left  ? node->left->height  : -1;
    int rightHeight = node->right ? node->right->height : -1;
    // calculates new height
    unsigned int newHeight = std::max(leftHeight, rightHeight) + 1;
    // if height has changed...
    if (newHeight != node->height){
      node->height = newHeight;

      return true;
    }
    else{
      return false;
    }
  }
  // updates height of each node contained in the stack
  void updateHeightsOnPath(std::stack<AVLTreeNode*> path){
    // node to have height updated
    AVLTreeNode* currentNode = nullptr;
    // while there are nodes to update
    while (!path.empty()){
      // gets a new node
      currentNode = path.top();
      // updates its height
      updateHeight(currentNode);
      // then discards its reference
      path.pop();
    }
  }

  template<typename Node>
  class AVLTreeWithNode : public BST::template BSTreeWithNode<Node>{
  private:
    using BSTWithNode = typename BST::template BSTreeWithNode<Node>;
  public:
    // builds an empty AVLTree. Basically delegates all the work to BSTree
    AVLTreeWithNode() : BSTWithNode{}
    {}
    // Creates an AVLTree with a nonempty root.  Notice that we use the
    // protected initializeRoot of BSTree to pass an allocated root node
    AVLTreeWithNode(Key key, Val val) : BSTWithNode{key, val}
    {}
    // inserts a Key Val pair in case Key is not present.  Return
    // indicates whether insertion occurred
    bool insert(Key key, Val val){
      bool hasInserted = BSTWithNode::insert(key, val);
      if (hasInserted){
        // do avl stuff
      }
      
      return hasInserted;
    }
    // removes key in case it is present.  Return value indicates
    // whether removal has occurred
    bool remove(Key key){
      // first we verify if key is present
      bool hasKey = BSTWithNode::search(key);
      // in case it is, removal will occurr
      if (hasKey){
        // performs removal, and returns parent key in case root was not deleted
        std::optional<Key> parentKey = BSTWithNode::removeExistingKey(key);
        if (parentKey){
          std::stack<AVLTreeNode*> affectedPath = BSTWithNode::pathToExistingKey(*parentKey);

          // do avl stuff
        }
        return true;
      }
      // otherwise, we simply indicate that removal has not occurred
      else{
        return false;
      }
    }
  };
  AVLTreeWithNode<AVLTreeNode> avlt;
public:
  // builds an empty AVLTree. Basically delegates all the work to BSTree
  AVLTree() : avlt{}
  {}
  // Creates an AVLTree with a nonempty root.  Notice that we use the
  // protected initializeRoot of BSTree to pass an allocated root node
  AVLTree(Key key, Val val) : avlt{key, val}
  {}
  // inserts a Key Val pair in case Key is not present.  Return
  // indicates whether insertion occurred
  bool insert(Key key, Val val){
    return avlt.insert(key, val);
  }
  // removes key in case it is present.  Return value indicates
  // whether removal has occurred
  bool remove(Key key){
    return avlt.remove(key);
  }
  std::optional<Val> search(Key key){
    return avlt.search(key);
  }
  // returns Key Val pair whose Val corresponds to the maximum BSTree
  // Key
  std::optional<std::pair<Key, Val>> maxKey() const {
    return avlt.maxKey();
  }
  // returns Key Val pair whose Val corresponds to the minimum BSTree
  // Key
  std::optional<std::pair<Key, Val>> ninKey() const {
    return avlt.ninKey();
  }
};
