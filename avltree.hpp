#pragma once

// smart pointers
#include <memory>

// we are going to inherit from BSTree
#include <bstree.hpp>

// AVLTree is a subclass of BSTree (under the same type parameters)
template<typename Key, typename Val>
class AVLTree : public BSTree<Key, Val>{
private:
  // type aliases for saving us from some typing
  using BST     = BSTree<Key, Val>;
  // since BST has not been instantiated yet, we need to tell the
  // compiler that BSTreeNode is a type name
  using BSTNode = typename BST::BSTreeNode;
  // the node of AVLTree is derived from the node of BSTree
  class AVLTreeNode : public BSTNode{
  public:
    // besides the members of BSTreeNode, AVLTreeNode has height,
    // which cannot take a negative value
    unsigned int height;

    // initializes AVLTreeNode.  Since it has no children, it has
    // height zero. Notice the base class constructor in the
    // initialization list
    AVLTreeNode(Key k, Val v) : BSTNode{k, v},
                                height{0}
    {}

    // inserts Val attached to Key in case Key is not present. Return
    // value indicates whether insertion really happened. Besides,
    // perform height update on the nodes affected by the insertion
    // and then do some rotations to preserve a good balance factor
    bool insert(Key k, Val v){
      // delegates real insertion to BSTreeNode.  Since its insert
      // method is a template, we need to tell that to the compiler
      bool hasInserted = BSTNode::template insert<AVLTreeNode>(k, v);
      // if insertion happened, we need to update some heights and
      // maybe do some rotations
      if (hasInserted){
        return true;
      }
      // otherwise, we simply signal that insertion has not occured
      else{
        return false;
      }
    }
  };

public:
  // builds an empty AVLTree. Basically delegates all the work to BSTree
  AVLTree() : BST()
  {}
  // Creates an AVLTree with a nonempty root.  Notice that we use the
  // protected initializeRoot of BSTree to pass an allocated root node
  AVLTree(Key key, Val val){
    BST::initializeRoot(std::make_unique<AVLTreeNode>(key, val));
  }
  bool insert(Key key, Val val){
    if (BST::isEmpty()){
      BST::initializeRoot(std::make_unique<AVLTreeNode>(key, val));

      return true;
    }
    else{
      return BST::insert(key, val);
    }
  }
  bool remove(Key key){
    bool hasKey = BST::search(key);
    if (hasKey){
      std::optional<Key> parentKey = BST::removeExistingKey(key);

      // do some avl stuff

      return parentKey != std::nullopt;
    }

    return hasKey;
  }
};
