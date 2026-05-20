cmake_minimum_required(VERSION 3.28)
project(AxoEditor)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets Svg)

file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS 
    "*.cpp" 
    "*.h" 
    "*.ui" 
    "*.qss"
)
file(GLOB_RECURSE EDITOR_MODULES CONFIGURE_DEPENDS 
    "*.ixx"
)


add_executable(axo_editor 
    main.cpp
    ${EDITOR_SOURCES}
)

target_sources(axo_editor 
    PRIVATE 
    FILE_SET CXX_MODULES 
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
    FILES ${EDITOR_MODULES}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${EDITOR_SOURCES} ${EDITOR_MODULES})

target_link_libraries(axo_editor PRIVATE 
    Qt6::Widgets
    Qt6::Svg
    axo_common
)

set_target_properties(axo_editor PROPERTIES
    WIN32_EXECUTABLE ON
)

if(WIN32)
    get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    
    # Leverages the specific environment variable rather than hardcoding '.exe'
    add_custom_command(TARGET axo_editor POST_BUILD
        COMMAND "${_qt_bin_dir}/windeployqt" 
                --debug 
                "$<TARGET_FILE:axo_editor>"
        COMMENT "Running Windows Qt deployment asset copies..."
    )
endif()