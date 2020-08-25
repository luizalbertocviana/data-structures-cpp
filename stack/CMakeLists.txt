add_library(stack INTERFACE)
target_include_directories(stack INTERFACE .)

target_link_libraries(stack INTERFACE linked_list)
