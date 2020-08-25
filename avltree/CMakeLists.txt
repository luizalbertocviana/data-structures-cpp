add_library(avltree INTERFACE)
target_include_directories(avltree INTERFACE .)

target_link_libraries(avltree INTERFACE bstree)
