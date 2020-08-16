#ifndef graph_hpp
#define graph_hpp

#include <Matrix.hpp>

#include <stack>
#include <vector>

template<typename MatrixType>
class DigraphOn{
private:
  using Data = MatrixType;
public:
  using size_type = typename Data::size_type;
private:
  Data data_;

  size_type num_verts_;
  size_type num_edges_;
public:
  const size_type& num_verts;
  const size_type& num_edges;

  DigraphOn(size_type num_v)
    : data_{num_v}, num_verts_{num_v}, num_edges_{0}, num_verts{num_verts_}, num_edges{num_edges_}
  {
    data_ = 0;
  }

  bool has_edge(size_type u, size_type v) const{
    return data_.const_at(u, v);
  }

  bool add_edge(size_type u, size_type v){
    if (has_edge(u, v)){
      return false;
    }
    else{
      data_.at(u, v) = true;

      ++num_edges_;

      return true;
    }
  }

  bool remove_edge(size_type u, size_type v){
    if (has_edge(u, v)){
      data_.at(u, v) = false;

      --num_edges_;

      return true;
    }
    else{
      return false;
    }
  }
};

using Digraph = DigraphOn<SquareMatrix<bool>>;

template<typename Function>
void depth_first_search(const Digraph& D, Digraph::size_type start, Function visitor){
  enum class Color{white, gray, black};

  std::vector<Color> color {D.num_verts, Color::white};

  std::stack<Digraph::size_type> dfs {};

  auto start_visit {[&dfs, &color](const auto i) {
                      dfs.push(i);
                      color[i] = Color::gray;
                    }};

  auto neighbor_to_visit {[&D, &color](const auto i) {
                            for (Digraph::size_type j {0}; j < D.num_verts; ++j){
                              if (color[j] == Color::white && D.has_edge(i, j)){
                                return j;
                              }
                            }

                            return D.num_verts;
                          }};

  auto finish_visit {[&visitor, &color](const auto i) {
                       visitor(i);
                       color[i] = Color::black;
                     }};

  start_visit(start);

  while (!dfs.empty()){
    auto neighbor {neighbor_to_visit(dfs.top())};

    if (neighbor < D.num_verts){
      start_visit(neighbor);
    }
    else{
      finish_visit(dfs.top());

      dfs.pop();
    }
  }
}

#endif
