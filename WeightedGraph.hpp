#ifndef weightedgraph_hpp
#define weightedgraph_hpp
// we use bstree to establish a relation between edges and weights
#include <bstree.hpp>
// we are going to use Graph class as a component
#include <Graph.hpp>
// class to represent an undirected graph with weight values
// associated to its edges. Default weight type is int
template<typename Weight = int>
class WeightedGraph{
public:
  // we use the same size_tyoe as Graph
  using size_type = Graph::size_type;
private:
  // we use a binary search tree, indexed by pairs of size_type and
  // valued by Weight, to represent a mapping from edges to weights
  using map_edge_weight = BSTree<std::pair<size_type, size_type>, Weight>;
  // our private members: an undirected graph and an edge-weight mapping
  Graph graph_;
  map_edge_weight edge_weight_;
public:
  // public const references to number of vertices and edges, respectively
  const size_type& num_verts;
  const size_type& num_edges;
  // simple constructor
  WeightedGraph(size_type num_verts)
    : graph_{num_verts}, edge_weight_{}, num_verts{graph_.num_verts}, num_edges{graph_.num_edges}
  {}
  // determines wheter an edge between u and v exists
  bool has_edge(size_type u, size_type v){
    return graph_.has_edge(u, v);
  }
  // adds edge betweem u and v associated with weight w. Returns false
  // if edge was already present
  bool add_edge(size_type u, size_type v, Weight w){
    // does nothing if edge already exists
    if (has_edge(u, v)){
      return false;
    }
    // otherwise ...
    else{
      // adds edge ...
      graph_.add_edge(u, v);
      // and associate it with weight w
      edge_weight_.insert({u, v}, w);
      // signals to caller that edge was added
      return true;
    }
  }
  // removes edge betweem vertices u and v. If such an edge is absent,
  // returns false
  bool remove_edge(size_type u, size_type v){
    // if graph has edge ...
    if (has_edge(u, v)){
      // removes it ...
      graph_.remove_edge(u, v);
      // and deletes its weight association
      edge_weight_.remove({u, v});
      // then signals to caller that removal has succeded
      return true;
    }
    // otherwise does nothing
    else{
      return false;
    }
  }
  // returns weight of edge between u amd v. In case such an edge does
  // not exist, returms default initialization value of Weight
  Weight edge_weight(size_type u, size_type v){
    if (has_edge(u, v)){
      return *edge_weight_.search({u, v});
    }
    else{
      return {};
    }
  }
  // sets weight of edge betweem u and v to w. In case of a
  // nonexisting edge, does nothing
  void set_edge_weight(size_type u, size_type v, Weight w){
    if (has_edge(u, v)){
      edge_weight_.update({u, v}, w);
    }
  }
};

#endif
