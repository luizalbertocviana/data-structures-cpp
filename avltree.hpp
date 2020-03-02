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
// if we are debugging, import printing facilities
#ifdef debug
#include <iostream>
#endif
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
    #ifdef debug
    std::cout << "height of node " << node->key << " is " << node->height << std::endl;
    std::cout << "balanceFactor of node " << node->key << " is " << (rightHeight - leftHeight) << std::endl;
    #endif
    return rightHeight - leftHeight;
  }
  // updates height of node based on the hights of its children.
  // Returns true in case height has been updated
  static void updateHeight(AVLTreeNode* node){
    // get height of left and right subtrees
    auto[leftHeight, rightHeight] = childrenHeights(node);
    // calculates new height
    unsigned int newHeight = std::max(leftHeight, rightHeight) + 1;
    #ifdef debug
    std::cout << "new height of node " << node->key << " is " << newHeight << std::endl;
    #endif
    node->height = newHeight;
  }
  static void rotateR(AVLTreeNode* node){
    #ifdef debug
    std::cout << "performing right rotation" << std::endl;
    #endif
    // first we set aside all the moving subtrees
    std::unique_ptr<AVLTreeNode> subtreeLL = std::move(node->left->left);
    std::unique_ptr<AVLTreeNode> subtreeLR = std::move(node->left->right);
    std::unique_ptr<AVLTreeNode> subtreeR  = std::move(node->right);
    // then we save the contents of moving nodes
    std::pair<Key, Val> nodeContent      = std::make_pair(node->key, node->val);
    std::pair<Key, Val> leftChildContent = std::make_pair(node->left->key, node->left->val);
    // left child becomes node
    node->key = leftChildContent.first;
    node->val = leftChildContent.second;
    // node becomes the right child
    node->right = std::make_unique<AVLTreeNode>(nodeContent.first, nodeContent.second);
    // finally we rearrange the moving subtrees ...
    node->left         = std::move(subtreeLL);
    node->right->left  = std::move(subtreeLR);
    node->right->right = std::move(subtreeR);
    // ... and update heights on the affected nodes
    updateHeight(node->right.get());
    updateHeight(node);
  }
  // rebalances a node
  static void rebalanceNode(AVLTreeNode* node){
    #ifdef debug
    std::cout << "rebalancing node " << node->key << std::endl;
    #endif
    // calculates balance factor
    int nodeBalanceFactor = balanceFactor(node);
    // node is left-heavy
    if (nodeBalanceFactor <= -2){
      int leftBalanceFactor = balanceFactor(node->left.get());
      if (leftBalanceFactor <= -1){
        rotateR(node);
      }
      else if(leftBalanceFactor >= 1){
        // left right rotation
      }
    }
    // node is right-heavy
    else if (nodeBalanceFactor >= 2){
      int rightBalanceFactor = balanceFactor(node->right.get());
      if (rightBalanceFactor >= 1){
        // left rotation
      }
      else if (rightBalanceFactor <= -1){
        // right left rotation
      }
    }
  }
  // the compiler will deduce what is Function for us
  template<typename Function>
  static void applyOnPath(Function func, std::stack<AVLTreeNode*> path){
    // node to have function applied on
    AVLTreeNode* currentNode = nullptr;
    // while there are nodes to be visited
    while (!path.empty()){
      // gets a new node
      currentNode = path.top();
      // apply function
      func(currentNode);
      // then discards its reference
      path.pop();
    }
  }
  // does the necessary height updates for nodes on path
  static void updateHeightsOnPath(std::stack<AVLTreeNode*> path){
    applyOnPath(updateHeight, path);
  }
  // rebalances each node on path
  static void rebalanceNodesOnPath(std::stack<AVLTreeNode*> path){
    applyOnPath(rebalanceNode, path);
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
      #ifdef debug
      std::cout << "inserting key " << key << std::endl;
      #endif
      // the actual insertion is made by BSTReeWithNode
      bool hasInserted = BSTWithNode::insert(key, val);
      // if insertion really happened ...
      if (hasInserted){
        // ... gets the nodes which have their subtrees modified
        std::stack<AVLTreeNode*> affectedPath = BSTWithNode::pathToExistingKey(key);
        #ifdef debug
        std::cout << "element on top of path is " << affectedPath.top()->key << std::endl;
        #endif
        // updates height of affected nodes
        updateHeightsOnPath(affectedPath);
        // rebalance each affected node
        rebalanceNodesOnPath(affectedPath);
      }
      
      return hasInserted;
    }
    // removes key in case it is present.  Return value indicates
    // whether removal has occurred
    bool remove(Key key){
      #ifdef debug
      std::cout << "removing node " << key << std::endl;
      #endif
      // first we verify if key is present
      bool hasKey = BSTWithNode::search(key);
      // in case it is, removal will occurr
      if (hasKey){
        // performs removal, and returns parent key in case root was not deleted
        std::optional<Key> parentKey = BSTWithNode::removeExistingKey(key);
        if (parentKey){
          std::stack<AVLTreeNode*> affectedPath = BSTWithNode::pathToExistingKey(*parentKey);
          // updates height of affected nodes
          updateHeightsOnPath(affectedPath);
          // rebalance each affected node
          rebalanceNodesOnPath(affectedPath);
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
