cmake_minimum_required(VERSION 3.28)
project(AxoEditor)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

file(GLOB EDITOR_FILES 
    "*.cpp" 
    "*.h" 
    "*.ixx"
    "*.ui"
)

add_executable(axo_editor 
    main.cpp
)

find_package(Qt6 REQUIRED COMPONENTS Core)
get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
add_custom_command(TARGET axo_editor POST_BUILD
    COMMAND "${_qt_bin_dir}/windeployqt.exe" 
            --debug 
            "$<TARGET_FILE:axo_editor>"
    COMMENT "Deploying Qt dependencies..."
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${EDITOR_FILES})
set_source_files_properties(${EDITOR_FILES} PROPERTIES LANGUAGE CXX)

target_link_libraries(axo_editor PRIVATE 
    Qt6::Widgets
    axo_common
)

set_target_properties(axo_editor PROPERTIES
    WIN32_EXECUTABLE ON
)