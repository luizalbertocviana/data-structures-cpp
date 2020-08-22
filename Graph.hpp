// this is another way of achieving what #pragma once does
#ifndef graph_hpp
#define graph_hpp
// we are going to use matrices to represent our graphs
#include <Matrix.hpp>
// stack and vector will be used in depth first search
#include <stack>
#include <vector>
// a class to represent a directed graph represented with MatrixType
template<template<typename Type> typename MatrixType = SquareMatrix>
class Digraph{
private:
  // alias for MatrixType
  using Data = MatrixType<bool>;
public:
  // we use the same size_type as Data
  using size_type = typename Data::size_type;
private:
  // our digraph is represented with an adjacency matrix
  Data data_;
  // number of vertices and (directed) edges, respectively
  size_type num_verts_;
  size_type num_edges_;
public:
  // const references to the number of vertices and edges, respectively
  const size_type& num_verts;
  const size_type& num_edges;
  // builds a digraph with num_v vertices and no edges
  Digraph(size_type num_v)
    : data_{num_v}, num_verts_{num_v}, num_edges_{0}, num_verts{num_verts_}, num_edges{num_edges_}
  {
    data_ = false;
  }
  // determines whether there is an edge from u to v
  bool has_edge(size_type u, size_type v) const{
    return data_.const_at(u, v);
  }
  // adds edge from u to v. Returns false if edge is already present
  bool add_edge(size_type u, size_type v){
    // in case edge is present, signals to user that nothing was done
    if (has_edge(u, v)){
      return false;
    }
    // otherwise, adds edge and accounts it
    else{
      data_.at(u, v) = true;

      ++num_edges_;

      return true;
    }
  }
  // removes edge from u to v. Returns false if edge does not exist
  bool remove_edge(size_type u, size_type v){
    // in case edge is present, removes it and accounts its deletion
    if (has_edge(u, v)){
      data_.at(u, v) = false;

      --num_edges_;

      return true;
    }
    // otherwise, does nothing
    else{
      return false;
    }
  }
};
// performs a depth first search in D starting at vertex start. When a
// vertex is visited, visitor is executed using visited vertex as argument
template<typename Function>
void depth_first_search(const Digraph& D, Digraph::size_type start, Function visitor){
  // we are going to use some colors to represent vertex status: white
  // vertices have not been found yet; gray vertices have been found
  // and are to be visited; black vertices have been visited
  enum class Color{white, gray, black};
  // creates a vector to store each vertex status; initially, no
  // vertex has been found
  std::vector<Color> color {D.num_verts, Color::white};
  // stack to control the order in which vertices should be visited
  std::stack<Digraph::size_type> dfs {};
  // procedure to be executed when a vertex is found
  auto start_visit {[&dfs, &color](const auto i) {
                      // it is put in the stack
                      dfs.push(i);
                      // we mark it as found but not visited yet; it
                      // will be visited after all its descendants
                      // have been visited
                      color[i] = Color::gray;
                    }};
  // procedure to explore vertices that were not found yet
  auto neighbor_to_visit {[&D, &color](const auto i) {
                            // for each vertex ...
                            for (Digraph::size_type j {0}; j < D.num_verts; ++j){
                              // if vertex is reachable from i and has
                              // not been found yet, returns it
                              if (color[j] == Color::white && D.has_edge(i, j)){
                                return j;
                              }
                            }
                            // As D.num_verts is not a valid index, we
                            // return this to indicate that every
                            // neighbor of i has already been found
                            return D.num_verts;
                          }};
  // when every descendant of a vertex has been visited, we visit it
  auto finish_visit {[&visitor, &color](const auto i) {
                       // call visitor
                       visitor(i);
                       // mark as visited
                       color[i] = Color::black;
                     }};
  // we start by finding vertex start
  start_visit(start);
  // while there is vertices to be visited, explore its neighborhood
  while (!dfs.empty()){
    // selects a neighbor to explore
    auto neighbor {neighbor_to_visit(dfs.top())};
    // neighbor is a valid vertex, so it must be explored
    if (neighbor < D.num_verts){
      start_visit(neighbor);
    }
    // neighbor is not a valid index, which indicates that the vertex
    // at the top of statk had all its descendants visited, so it is
    // time to visit it, then discard it
    else{
      // visits it
      finish_visit(dfs.top());
      // discards it
      dfs.pop();
    }
  }
}
// a class to represnet an undirected graph 
template<typename MatrixType>
class GraphOn : public DigraphOn<MatrixType>{
private:
  // alias for superclass
  using Digraph = DigraphOn<MatrixType>;
public:
  // we use the same size_type as superclass
  using size_type = typename Digraph::size_type;
private:
  // ensures u <= v, which allows us to use a compact representation
  // of the graph
  static void adjust_endpoints_(size_type& u, size_type& v){
    if (u > v){
      std::swap(u, v);
    }
  }
public:
  // builds an undirected graph with num_v vertices
  GraphOn(size_type num_v) : Digraph{num_v}
  {}
  // determines whether there is an edge between u and v
  bool has_edge(size_type u, size_type v) const{
    // notice that this enforces the lack of direction in our representation
    adjust_endpoints_(u, v);
    // delegates all the work to the superclass
    return Digraph::has_edge(u, v);
  }
  // adds edge between u and v
  bool add_edge(size_type u, size_type v){
    adjust_endpoints_(u, v);

    return Digraph::add_edge(u, v);
  }
  // removes edge between u and v
  bool remove_edge(size_type u, size_type v){
    adjust_endpoints_(u, v);

    return Digraph::remove_edge(u, v);
  }
};

using Graph = GraphOn<UpperTriangularMatrix<bool>>;

#endif
