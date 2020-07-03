#ifndef bstree_hpp
#define bstree_hpp

#include <memory>
#include <optional>

template<typename Key, typename Val>
struct DataNode{
  Key key;
  Val val;

  DataNode(Key k, Val v) : key{k}, val{v}
  {}
};

template<typename Node>
struct BinaryNode{
  std::unique_ptr<Node> left;
  std::unique_ptr<Node> right;

  BinaryNode() : left{nullptr}, right{nullptr}
  {}
};

template<typename Key, typename Val>
struct BSTreeNode : public DataNode<Key, Val>, public BinaryNode<BSTreeNode<Key, Val>>{
  BSTreeNode(Key key, Val val) : DataNode<Key, Val>{key, val}, BinaryNode<BSTreeNode<Key, Val>>{}
  {}
};

template<typename Key, typename Val, typename Node = BSTreeNode<Key, Val>>
class BSTree{
  using node_ptr = std::unique_ptr<Node>;
  
  node_ptr root_;

  static std::optional<Val> search_(const node_ptr& node, const Key& key){
    if (node){
      if (key == node->key){
        return node->val;
      }
      else if (key < node->key){
        return search_(node->left, key);
      }
      else{
        return search_(node->right, key);
      }
    }
    else{
      return {};
    }
  }

  static bool insert_(node_ptr& node, const Key& key, const Val& val){
    if (node){
      if (key < node->key){
        return insert_(node->left, key, val);
      }
      else if (key > node->key){
        return insert_(node->right, key, val);
      }
      else{
        return false;
      }
    }
    else{
      node = std::make_unique<Node>(key, val);

      return true;
    }
  }

  template<typename Function>
  static std::optional<std::pair<Key, Val>> deepest_key_(const Node* node, const Function& child_selector){
    if (node){
      if (child_selector(node)){
        return deepest_key_(child_selector(node), child_selector);
      }
      else{
        return {{node->key, node->val}};
      }
    }
    else{
      return {};
    }
  }

  static std::optional<std::pair<Key, Val>> max_key_(const node_ptr& node){
    return deepest_key_(node.get(), [](const Node* n) {return n->right.get();});
  }

  static std::optional<std::pair<Key, Val>> min_key_(const node_ptr& node){
    return deepest_key_(node.get(), [](const Node* n) {return n->left.get();});
  }

  static bool remove_(node_ptr& node, const Key& key){
    if (node){
      if (key < node->key){
        return remove_(node->left, key);
      }
      else if (key > node->key){
        return remove_(node->right, key);
      }
      else{
        if (node->left && node->right){
          auto[max_left_key, max_left_val] {*max_key_(node->left)};

          remove_(node->left, max_left_key);

          node->key = max_left_key;
          node->val = max_left_val;
        }
        else{
          node = std::move(node->left ? node->left : node->right);
        }

        return true;
      }
    }
    else{
      return false;
    }
  }
public:
  BSTree() : root_{nullptr}
  {}

  bool empty() const{
    return root_ == nullptr;
  }

  std::optional<std::pair<Key, Val>> max_key() const{
    return max_key_(root_);
  }

  std::optional<std::pair<Key, Val>> min_key() const{
    return min_key_(root_);
  }

  std::optional<Val> search(const Key& key) const{
    return search_(root_, key);
  }

  bool insert(const Key& key, const Val& val){
    return insert_(root_, key, val);
  }

  bool remove(const Key& key){
    return remove_(root_, key);
  }
};

#endif
