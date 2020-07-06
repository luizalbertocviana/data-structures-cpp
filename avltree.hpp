#ifndef avltree_hpp
#define avltree_hpp

#include <bstree.hpp>

#include <algorithm>

struct HeightNode{
  unsigned long long height;

  HeightNode() : height{0}
  {}
};

template<typename Key, typename Val>
struct AVLTreeNode : public DataNode<Key, Val>,
                     public BinaryNode<AVLTreeNode<Key, Val>>,
                     public HeightNode
{
  AVLTreeNode(const Key& key, const Val& val) : DataNode<Key, Val>{key, val},
                                                BinaryNode<AVLTreeNode<Key, Val>>{},
                                                HeightNode{}
  {}
};

template<typename Key, typename Val>
class AVLTree : public BSTree<Key, Val, AVLTreeNode<Key, Val>>{
  using BSTree      = BSTree<Key, Val, AVLTreeNode<Key, Val>>;
  using node_ptr    = typename BSTree::node_ptr;
  using height_type = long long;

  static height_type height_(const node_ptr& node){
    return node ? node->height : -1;
  }

  static height_type balance_factor_(const node_ptr& node){
    return height_(node->right) - height_(node->left);
  }

  static void update_height_(node_ptr& node){
    node->height = std::max(height_(node->left), height_(node->right)) + 1;
  }

  static void rotate_r_(node_ptr& node){
    node_ptr left {std::move(node->left)};

    node->left  = std::move(left->right);
    left->right = std::move(node);
    node        = std::move(left);

    update_height_(node->right);
    update_height_(node);
  }

  static void rotate_l_(node_ptr& node){
    node_ptr right {std::move(node->right)};

    node->right = std::move(right->left);
    right->left = std::move(node);
    node        = std::move(right);

    update_height_(node->left);
    update_height_(node);
  }

  static void rotate_lr_(node_ptr& node){
    rotate_l_(node->left);
    rotate_r_(node);
  }

  static void rotate_rl_(node_ptr& node){
    rotate_r_(node->right);
    rotate_l_(node);
  }

  static void balance_node_(node_ptr& node){
    height_type node_bf {balance_factor_(node)};

    if (node_bf <= -2){
      height_type left_bf {balance_factor_(node->left)};

      if (left_bf <= -1){
        
      }
      else{
        
      }
    }
    else if (node_bf >= 2){
      height_type right_bf {balance_factor_(node->right)};

      if (right_bf >= 1){
        
      }
      else{
        
      }
    }
  }
public:
  AVLTree() : BSTree{}
  {}

  bool insert(const Key& key, const Val& val){
    bool has_inserted {BSTree::insert(key, val)};

    if (has_inserted){
      
    }

    return has_inserted;
  }
};

void f(){
  AVLTree<int, int> avltree {};

  avltree.empty();

  avltree.search(2);

  avltree.insert(2, 2);
}

#endif
