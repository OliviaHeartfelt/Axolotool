cmake_minimum_required(VERSION 3.28) # Required for C++20/23 modules
project(Axolotool)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include external dependencies
include(cmake/Dependencies.cmake)

# Add the sub-folders
#add_subdirectory(src/common)
add_subdirectory(src/editor)