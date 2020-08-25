add_library(rbtree INTERFACE)
target_include_directories(rbtree INTERFACE .)

target_link_libraries(rbtree INTERFACE bstree)
