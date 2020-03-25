// smart pointers
#include <memory>
// optional type
#include <optional>

// we are going to inherit from BSTree
#include <bstree.hpp>

// RBTree is a subclass of BSTree under the same type parameters
template<typename Key, typename Val>
class RBTree : public BSTree<Key, Val>{
private:
  // type alias
  using BST = BSTree<Key, Val>;
  // Color type
  enum class Color {red, black};
  // RBTree node
  struct RBTreeNode{
    Key key;
    Val val;
    // smart pointers
    std::unique_ptr<RBTreeNode> left;
    std::unique_ptr<RBTreeNode> right;
    // node color
    Color color;
    // RBTreeNode constructor. A new node is always red
    RBTreeNode(Key k, Val v) : key{k},
                               val{v},
                               left{nullptr},
                               right{nullptr},
                               color{Color::red}
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
  // implementation of RBTree
  template<typename Node>
  class RBTreeWithNode : public BST::template BSTreeWithNode<Node>{
  private:
    // type alias. We help the compiler a little
    using BSTWithNode = typename BST::template BSTreeWithNode<Node>;
  public:
    // builds an empty tree, delegating all the work to the superclass
    RBTreeWithNode() : BSTWithNode{}
    {}
    // builds a tree with a nonempty root
    RBTreeWithNode(Key key, Val val) : BSTWithNode{key, val}{
      // root is red. This needs to be corrected
    }
    // inserts a Key Val pair into the tree. Return value indicates
    // whether insertion occurred
    bool insert(Key key, Val val){
      // the actual insertion is done by BSTreeWithNode
      bool hasInserted = BSTWithNode::insert(key, val);
      if (hasInserted){
        // do rb stuff
      }
      return hasInserted;
    }
    // we are not going to implement remove this time
    bool remove(Key key) = delete;
  };
  // RBTree with proper type node
  RBTreeWithNode<RBTreeNode> rbtree;
public:
  // builds an empty tree
  RBTree() : rbtree{}
  {}
  // builds a tree with nonempty root
  RBTree(Key key, Val val) : rbtree{key, val}
  {}
  // inserts Key Val pair. Return value indicates whether insertion
  // occurred
  bool insert(Key key, Val val){
    return rbtree.insert(key, val);
  }
  // we are not going to implement remove this time
  bool remove(Key key) = delete;
  // returns Val attached to Key. In case Key is not present, returns
  // nothing
  std::optional<Val> search(Key key) const {
    return rbtree.search(key);
  }
  // returns Key Val pair whose Val corresponds to the maximum BSTree
  // Key.  In case tree is empty, returns nothing
  std::optional<std::pair<Key, Val>> maxKey() const {
    return rbtree.maxKey();
  }
  // returns Key Val pair whose Val corresponds to the minimum BSTree
  // Key.  In case tree is empty, returns nothing
  std::optional<std::pair<Key, Val>> ninKey() const {
    return rbtree.ninKey();
  }
};
