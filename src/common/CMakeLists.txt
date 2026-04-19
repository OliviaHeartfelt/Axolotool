cmake_minimum_required(VERSION 3.28)
project(AxoCommon)

# Enable C++23 for modules
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt Auto-tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Glob your common files
file(GLOB COMMON_FILES
    "*.cpp" 
    "*.h"
    "*.ui"
)
file(GLOB COMMON_MODULES
    "*.ixx"
)

# Define the library
add_library(axo_common)

target_sources(axo_common PUBLIC FILE_SET CXX_MODULES FILES ${COMMON_MODULES})
target_sources(axo_common PRIVATE ${COMMON_FILES})

target_link_libraries(axo_common PUBLIC Qt6::Widgets)