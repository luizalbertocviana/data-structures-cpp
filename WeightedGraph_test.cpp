#include <cassert>

#include <WeightedGraph.hpp>

int main(){
  WeightedGraph wg {10};

  wg.add_edge(1, 2, 10);
  wg.add_edge(8, 9, 20);

  assert(wg.has_edge(1, 2));
  assert(wg.has_edge(8, 9));
  assert(wg.edge_weight(1, 2) == 10);
  assert(wg.edge_weight(8, 9) == 20);

  wg.remove_edge(1, 2);
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

  return 0;
}
