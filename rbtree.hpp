#pragma once
// smart pointers
#include <memory>
// optional type
#include <optional>

// we are going to inherit from BSTree
#include <bstree.hpp>

enum class Color {red, black};

struct ColorNode{
  Color color;

  ColorNode() : color{Color::red}
  {}
};

template<typename Key, typename Val>
struct RBTreeNode : public DataNode<Key, Val>,
                    public BinaryNode<RBTreeNode<Key, Val>>,
                    public ColorNode
{
  RBTreeNode(const Key& key, const Val& val) : DataNode<Key, Val>{key, val},
                                               BinaryNode<RBTreeNode<Key, Val>>{},
                                               ColorNode{}
  {}
};

template<typename Key, typename Val>
class RBTree : protected BSTree<Key, Val, RBTreeNode<Key, Val>>{
  using BST      = BSTree<Key, Val, RBTreeNode<Key, Val>>;
  using node_ptr = typename BST::node_ptr;
public:
  RBTree() : BST{}
  {}

  bool insert(const Key& key, const Val& val){
    bool has_inserted {BST::insert(key, val)};

    if (has_inserted){
      // do rb stuff
    }

    return has_inserted;
  }

  bool remove(const Key& key){
    if (auto parent_key {BST::remove_(key)}){
      // do rb stuff
    }

    return !BST::contains(key);
  }

  using BST::empty;
  using BST::search;
  using BST::contains;
  using BST::max_key;
  using BST::min_key;
};
