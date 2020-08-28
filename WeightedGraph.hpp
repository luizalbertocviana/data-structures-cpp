#ifndef weightedgraph_hpp
#define weightedgraph_hpp

#include <bstree.hpp>
#include <Graph.hpp>

template<typename Weight = int>
class WeightedGraph{
public:
  using size_type = Graph::size_type;
private:
  using map_edge_weight = BSTree<std::pair<size_type, size_type>, Weight>;

  Graph graph_;
  map_edge_weight edge_weight_;
public:
  const size_type num_verts;
  const size_type num_edges;
  
  WeightedGraph(size_type num_verts) : graph_{num_verts}, edge_weight_{}, num_verts{graph_.num_verts}, num_edges{graph_.num_edges}
  {}

  bool has_edge(size_type u, size_type v){
    return graph_.has_edge(u, v);
  }

  bool add_edge(size_type u, size_type v, Weight w){
    if (has_edge(u, v)){
      return false;
    }
    else{
      graph_.add_edge(u, v);

      edge_weight_.insert({u, v}, w);

      return true;
    }
  }

  bool remove_edge(size_type u, size_type v){
    if (has_edge(u, v)){
      graph_.remove_edge(u, v);

      edge_weight_.remove({u, v});

      return true;
    }
    else{
      return false;
    }
  }

  Weight edge_weight(size_type u, size_type v){
    if (has_edge(u, v)){
      return *edge_weight_.search({u, v});
    }
    else{
      return {};
    }
  }

  void set_edge_weight(size_type u, size_type v, Weight w){
    if (has_edge(u, v)){
      edge_weight_.update({u, v}, w);
    }
  }
};

#endif
