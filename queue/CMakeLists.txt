add_library(queue INTERFACE)
target_include_directories(queue INTERFACE .)

target_link_libraries(queue INTERFACE stack)
