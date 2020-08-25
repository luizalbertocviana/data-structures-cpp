add_library(Graph INTERFACE)
target_include_directories(Graph INTERFACE .)

target_link_libraries(Graph INTERFACE Matrix)
