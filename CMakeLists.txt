cmake_minimum_required(VERSION 3.16)
project(simple-fibers)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(external)

add_subdirectory(psi)

add_subdirectory(tests)
