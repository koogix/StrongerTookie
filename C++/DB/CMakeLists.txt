cmake_minimum_required (VERSION 2.8)

project (DBDemo)

add_subdirectory (classes lib)
add_subdirectory (example bin)

add_dependencies(demo dbutil)
