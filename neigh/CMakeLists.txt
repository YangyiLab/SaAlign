cmake_minimum_required(VERSION 3.5)
include(CMakeDependentOption)
include(CheckCCompilerFlag)

project(neigh)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")

# Source files
set(neigh_SOURCES
        src/main.c
        src/dist_matrix.c src/dist_matrix.h
        src/io.c src/io.h
        src/neighbour_joining.c src/neighbour_joining.h
        src/phylogenetic_tree.c src/phylogenetic_tree.h
        src/utilities.c src/utilities.h
    )

set(neigh_LIBS )



# config.h header
configure_file(src/config.h.in config.h @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(neigh ${neigh_SOURCES})
target_link_libraries(neigh ${neigh_LIBS})
add_library(nneigh SHARED ${neigh_SOURCES})
