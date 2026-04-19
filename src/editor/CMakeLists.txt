cmake_minimum_required(VERSION 3.28)
project(AxoEditor)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt Auto-tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Glob all files
file(GLOB EDITOR_FILES 
    "*.cpp" 
    "*.h" 
    "*.ixx"
    "*.ui"
)

# Create the executable
add_executable(axo_editor ${EDITOR_FILES})

#
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${EDITOR_FILES})
set_source_files_properties(${EDITOR_FILES} PROPERTIES LANGUAGE CXX)

target_link_libraries(axo_editor PRIVATE 
    Qt6::Widgets
    axo_common
)

set_target_properties(axo_editor PROPERTIES
    WIN32_EXECUTABLE ON
)