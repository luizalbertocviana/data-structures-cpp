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
  // type alias for saving us from some typing
  using BST = BSTree<Key, Val>;
  // the node of AVLTree
  struct AVLTreeNode{
    Key key;
    Val val;
    // smart pointers
    std::unique_ptr<AVLTreeNode> left;
    std::unique_ptr<AVLTreeNode> right;
    // height of node (no negative value makes sense)
    unsigned int height;
    // initializes AVLTreeNode.  Since it has no children, it has
    // height zero
    AVLTreeNode(Key k, Val v) : key{k},
                                val{v},
                                left{nullptr},
                                right{nullptr},
                                height{0}
    {}
    // returns Key Val pair whose Key is maximum.  We need this
    // information on every subtree for the removal algorithm
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
  static std::pair<int, int> childrenHeights(const AVLTreeNode* node){
    // get height of left and right subtrees
    int leftHeight  = node->left  ? node->left->height  : -1;
    int rightHeight = node->right ? node->right->height : -1;

    return std::make_pair(leftHeight, rightHeight);
  }
  static int balanceFactor(const AVLTreeNode* node){
    auto[leftHeight, rightHeight] = childrenHeights(node);

    return rightHeight - leftHeight;
  }
  // updates height of node based on the hights of its children.
  // Returns true in case height has been updated
  static bool updateHeight(AVLTreeNode* node){
    // get height of left and right subtrees
    auto[leftHeight, rightHeight] = childrenHeights(node);
    // calculates new height
    unsigned int newHeight = std::max(leftHeight, rightHeight) + 1;
    // if height has changed ...
    if (newHeight != node->height){
      node->height = newHeight;

      return true;
    }
    else{
      return false;
    }
  }
  // updates height of each node contained in path
  static void updateHeightsOnPath(std::stack<AVLTreeNode*> path){
    // node to have height updated
    AVLTreeNode* currentNode = nullptr;
    // indicates whether last update really changed node height.
    // Initial value is true so we can enter the while loop
    bool hasChanged = true;
    // while there are nodes to be updated
    while (hasChanged && !path.empty()){
      // gets a new node
      currentNode = path.top();
      // updates its height
      hasChanged = updateHeight(currentNode);
      // then discards its reference
      path.pop();
    }
  }
  // rebalances a node
  static bool rebalanceNode(AVLTreeNode* node){
    // calculates balance factor
    int balanceFactor = balanceFactor(node);
    // node is left-heavy
    if (balanceFactor <= -2){
      int leftBalanceFactor = balanceFactor(node->left);
      if (leftBalanceFactor <= -1){
        // right rotation
      }
      else if(leftBalanceFactor >= 1){
        // left right rotation
      }
      return true;
    }
    // node is right-heavy
    else if (balanceFactor >= 2){
      int rightBalanceFactor = balanceFactor(node->right);
      if (rightBalanceFactor >= 1){
        // left rotation
      }
      else if (rightBalanceFactor <= -1){
        // right left rotation
      }
      return true;
    }
    return false;
  }
  // implementation of AVLTree
  template<typename Node>
  class AVLTreeWithNode : public BST::template BSTreeWithNode<Node>{
  private:
    // since BST is not instantiated yet, we need to tell the compiler
    // that BSTreeWithNode is a template and a type name when instantiated
    using BSTWithNode = typename BST::template BSTreeWithNode<Node>;
  public:
    // builds an empty AVLTree. Basically delegates all the work to BSTreeWithNode
    AVLTreeWithNode() : BSTWithNode{}
    {}
    // Creates an AVLTree with a nonempty root
    AVLTreeWithNode(Key key, Val val) : BSTWithNode{key, val}
    {}
    // inserts a Key Val pair in case Key is not present.  Return
    // indicates whether insertion occurred
    bool insert(Key key, Val val){
      bool hasInserted = BSTWithNode::insert(key, val);
      if (hasInserted){
        // gets the nodes which have their subtrees modified
        std::stack<AVLTreeNode*> affectedPath = BSTWithNode::pathToExistingKey(key);
        // updates height of affected nodes
        updateHeightsOnPath(affectedPath);
        // rebalance each affected node
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
  // AVLTree with proper node type
  AVLTreeWithNode<AVLTreeNode> avlt;
public:
  // builds an empty AVLTree
  AVLTree() : avlt{}
  {}
  // Creates an AVLTree with a nonempty root
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
  // returns Val attached to Key.  In case Key is not present, returns
  // nothing
  std::optional<Val> search(Key key) const {
    return avlt.search(key);
  }
  // returns Key Val pair whose Val corresponds to the maximum BSTree
  // Key.  In case tree is empty, returns nothing
  std::optional<std::pair<Key, Val>> maxKey() const {
    return avlt.maxKey();
  }
  // returns Key Val pair whose Val corresponds to the minimum BSTree
  // Key.  In case tree is empty, returns nothing
  std::optional<std::pair<Key, Val>> ninKey() const {
    return avlt.ninKey();
  }
};
