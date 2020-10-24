#include <cassert>

#include <Graph.hpp>

void test_digraph(){
  Digraph D {20};

  assert(D.num_verts == 20);

  D.add_edge(1, 2);

  assert(D.has_edge(1,2));
  assert(D.num_edges == 1);

  D.add_edge(1,2);

  assert(D.has_edge(1,2));
  assert(D.num_edges == 1);

  D.add_edge(3, 4);

  assert(D.has_edge(3,4));
  assert(D.num_edges == 2);

  D.remove_edge(2,3);

  assert(D.num_edges == 2);

  D.remove_edge(1,2);

  assert(!D.has_edge(1,2));
  assert(D.num_edges == 1);
}

void test_graph(){
  Graph G {20};

  assert(G.num_verts == 20);

  G.add_edge(1, 2);

  assert(G.has_edge(1,2));
  assert(G.has_edge(2,1));
  assert(G.num_edges == 1);

  G.add_edge(3,4);

  assert(G.has_edge(3,4));
  assert(G.has_edge(4,3));
  assert(G.num_edges == 2);

  G.add_edge(3,4);
  G.add_edge(4,3);

  assert(G.has_edge(3,4));
  assert(G.has_edge(4,3));
  assert(G.num_edges == 2);

  G.remove_edge(2,3);

  assert(G.num_edges == 2);

  G.remove_edge(1,2);

  assert(!G.has_edge(1,2));
  assert(!G.has_edge(2,1));
  assert(G.num_edges == 1);
}

int main(){
  test_digraph();

  test_graph();

  return 0;
}
