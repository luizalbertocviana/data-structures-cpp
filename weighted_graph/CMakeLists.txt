add_library(weighted_graph INTERFACE)
target_include_directories(weighted_graph INTERFACE .)

target_link_libraries(weighted_graph INTERFACE graph bstree)
