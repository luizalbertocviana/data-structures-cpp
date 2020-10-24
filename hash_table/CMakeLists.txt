add_library(hash_table INTERFACE)
target_include_directories(hash_table INTERFACE .)

target_link_libraries(hash_table INTERFACE linked_list)
