add_library(graph INTERFACE)
target_include_directories(graph INTERFACE .)

target_link_libraries(graph INTERFACE matrix)
