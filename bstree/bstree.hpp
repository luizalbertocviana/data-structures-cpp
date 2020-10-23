#ifndef bstree_hpp
#define bstree_hpp

#include <memory>
#include <optional>

template<typename Key, typename Val>
struct DataNode{
  Key key;
  Val val;

  DataNode(const Key& k, const Val& v) : key{k}, val{v}
  {}
};

template<typename Node>
struct BinaryNode{
  using node_ptr = std::unique_ptr<Node>;

  node_ptr left;
  node_ptr right;

  BinaryNode() : left{nullptr}, right{nullptr}
  {}
};

template<typename Key, typename Val>
struct BSTreeNode : public DataNode<Key, Val>, public BinaryNode<BSTreeNode<Key, Val>>{
  BSTreeNode(const Key& key, const Val& val) : DataNode<Key, Val>{key, val}, BinaryNode<BSTreeNode<Key, Val>>{}
  {}
};

template<typename Key, typename Val, typename Node = BSTreeNode<Key, Val>>
class BSTree{
protected:
  using node_ptr = typename Node::node_ptr;
private:
  node_ptr root_;

  static std::optional<Val> search_(const node_ptr& node, const Key& key){
    if (node){
      if (key < node->key){
        return search_(node->left, key);
      }
      else if (key > node->key){
        return search_(node->right, key);
      }
      else{
        return node->val;
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

  static bool update_(node_ptr& node, const Key& key, const Val& new_val){
    if (node) {
      if (key < node->key) {
        return update_(node->left, key, new_val);
      }
      else if (key > node->key) {
        return update_(node->left, key, new_val);
      }
      else {
        node->val = new_val;

        return true;
      }
    }
    else {
      return false;
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

  template<typename Function>
  static void bottom_up_apply__(node_ptr& node, const Key& key, const Function& f){
    if (node){
      if (key < node->key){
        bottom_up_apply__(node->left, key, f);
      }
      else if (key > node->key){
        bottom_up_apply__(node->right, key, f);
      }

      f(node);
    }
  }

  static std::optional<Key> remove__(node_ptr& node, const Key& key){
    auto recurse_into {[&node](node_ptr& n, const Key& key) {
                         auto opt_key = remove__(n, key);

                         return opt_key ? opt_key : node->key;
                       }};

    if (node){
      if (key < node->key){
        return recurse_into(node->left, key);
      }
      else if (key > node->key){
        return recurse_into(node->right, key);
      }
      else{
        if (node->left && node->right){
          auto[max_left_key, max_left_val] {*max_key_(node->left)};

          node->key = max_left_key;
          node->val = max_left_val;

          return recurse_into(node->left, max_left_key);
        }
        else{
          node = std::move(node->left ? node->left : node->right);

          return {};
        }
      }
    }
    else{
      return {};
    }
  }
protected:
  std::optional<Key> remove_(const Key& key){
    return remove__(root_, key);
  }

  template<typename Function>
  void bottom_up_apply_(const Key& key, const Function& f){
    bottom_up_apply__(root_, key, f);
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

  bool contains(const Key& key) const{
    return search(key) ? true : false;
  }

  bool insert(const Key& key, const Val& val){
    return insert_(root_, key, val);
  }

  bool update(const Key& key, const Val& new_val){
    return update_(root_, key, new_val);
  }

  bool remove(const Key& key){
    remove_(key);

    return !contains(key);
  }
};

#endif
