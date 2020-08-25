add_executable(avltree_tester avltree.cpp)
target_link_libraries(avltree_tester PRIVATE avltree)

add_test(NAME avltree_test COMMAND avltree_tester)

add_executable(binary_heap_tester binary_heap.cpp)
target_link_libraries(binary_heap_tester PRIVATE binary_heap)

add_test(NAME binary_heap_test COMMAND binary_heap_tester)

add_executable(bstree_tester bstree.cpp)
target_link_libraries(bstree_tester PRIVATE bstree)

add_test(NAME bstree_test COMMAND bstree_tester)

add_executable(btree_tester btree.cpp)
target_link_libraries(btree_tester PRIVATE btree)

add_test(NAME btree_test COMMAND btree_tester)

add_executable(disjoint_sets_tester disjoint_sets.cpp)
target_link_libraries(disjoint_sets_tester PRIVATE disjoint_sets)

add_test(NAME disjoint_sets_test COMMAND disjoint_sets_tester)

add_executable(Graph_tester Graph.cpp)
target_link_libraries(Graph_tester PRIVATE Graph)

add_test(NAME Graph_test COMMAND Graph_tester)

add_executable(linked_list_tester linked_list.cpp)
target_link_libraries(linked_list_tester PRIVATE linked_list)

add_test(NAME linked_list_test COMMAND linked_list_tester)

add_executable(Matrix_tester Matrix.cpp)
target_link_libraries(Matrix_tester PRIVATE Matrix)

add_test(NAME Matrix_test COMMAND Matrix_tester)

add_executable(queue_tester queue.cpp)
target_link_libraries(queue_tester PRIVATE queue)

add_test(NAME queue_test COMMAND queue_tester)

add_executable(rbtree_tester rbtree.cpp)
target_link_libraries(rbtree_tester PRIVATE rbtree)

add_test(NAME rbtree_test COMMAND rbtree_tester)

add_executable(sorting_tester sorting.cpp)
target_link_libraries(sorting_tester PRIVATE sorting)

add_test(NAME sorting_test COMMAND sorting_tester)

add_executable(stack_tester stack.cpp)
target_link_libraries(stack_tester PRIVATE stack)

add_test(NAME stack_test COMMAND stack_tester)
