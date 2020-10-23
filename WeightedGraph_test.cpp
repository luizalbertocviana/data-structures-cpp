#include <cassert>

#include <WeightedGraph.hpp>

void test1(){
  WeightedGraph wg {10};
  assert(wg.num_verts == 10);
  assert(wg.num_edges == 0);

  wg.add_edge(1, 2, 10);
  wg.add_edge(8, 9, 20);

  assert(wg.num_edges == 2);
  assert(wg.has_edge(1, 2));
  assert(wg.has_edge(2, 1));
  assert(wg.has_edge(8, 9));
  assert(wg.has_edge(9, 8));
  assert(wg.edge_weight(1, 2) == 10);
  assert(wg.edge_weight(2, 1) == 10);
  assert(wg.edge_weight(8, 9) == 20);
  assert(wg.edge_weight(9, 8) == 20);

  wg.remove_edge(1, 2);
  assert(wg.num_edges == 1);
  assert(!wg.has_edge(1, 2));

  int w {wg.edge_weight(8, 9)};
  assert(w == 20);

  wg.set_edge_weight(8, 9, 1);

  w = wg.edge_weight(8, 9);
  assert(w == 1);

  wg.set_edge_weight(8, 9, 3);

  w = wg.edge_weight(8, 9);
  assert(w == 3);

  assert(!wg.has_edge(1, 2));
  assert(wg.edge_weight(1, 2) == 0);

  wg.set_edge_weight(1, 2, 100);
  assert(wg.edge_weight(1, 2) == 0);
}

void test2(){
  WeightedGraph wg {6};
  assert(wg.num_verts == 6);
  assert(wg.num_edges == 0);

  wg.add_edge(0, 1, 11);
  assert(wg.num_edges == 1);
  wg.add_edge(0, 2, 11);
  assert(wg.num_edges == 2);
  wg.add_edge(0, 3, 8);
  assert(wg.num_edges == 3);
  wg.add_edge(0, 4, 3);
  assert(wg.num_edges == 4);
  wg.add_edge(1, 3, 1);
  assert(wg.num_edges == 5);
  wg.add_edge(1, 5, 42);
  assert(wg.num_edges == 6);
  wg.add_edge(2, 3, 7);
  assert(wg.num_edges == 7);
  assert(wg.has_edge(2, 4) == false);
  wg.add_edge(2, 4, 1);
  assert(wg.has_edge(2, 4) == true);
  assert(wg.num_edges == 8);
  assert(wg.has_edge(3, 5) == false);
  wg.add_edge(3, 5, 5);
  assert(wg.has_edge(3, 5) == true);
  assert(wg.num_edges == 9);
}

int main(){
  test1();
  test2();

  return 0;
}
