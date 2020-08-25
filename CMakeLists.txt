cmake_minimum_required(VERSION 3.7..3.18)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(data-structures-cpp
  VERSION 1.0
  DESCRIPTION "classical data structures implemented in C++ for lecturing purposes"
  LANGUAGES CXX)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

set(default_build_type "Release")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(avltree)
add_subdirectory(binary_heap)
add_subdirectory(bstree)
add_subdirectory(btree)
add_subdirectory(disjoint_sets)
add_subdirectory(Graph)
add_subdirectory(linked_list)
add_subdirectory(Matrix)
add_subdirectory(queue)
add_subdirectory(rbtree)
add_subdirectory(sorting)
add_subdirectory(stack)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(tests)
endif()
