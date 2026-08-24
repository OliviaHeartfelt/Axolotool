cmake_minimum_required(VERSION 3.28)
project(AxoCommon)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets Svg)

file(GLOB_RECURSE COMMON_FILES CONFIGURE_DEPENDS "*.cpp" "*.h" "*.ui" "*.qss")
file(GLOB_RECURSE COMMON_MODULES CONFIGURE_DEPENDS "*.ixx")

set_source_files_properties(${COMMON_MODULES} PROPERTIES
    SKIP_AUTOGEN OFF
    SKIP_AUTOMOC OFF
    LANGUAGE CXX
)

add_library(axo_common OBJECT)

target_compile_features(axo_common PUBLIC cxx_std_23)

target_sources(axo_common
    PUBLIC
        FILE_SET my_modules
        TYPE CXX_MODULES
        FILES
            ${COMMON_MODULES}
    PRIVATE
        ${COMMON_FILES}
        "ACommonResources.qrc"
)

target_include_directories(axo_common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/AButton
    ${CMAKE_CURRENT_SOURCE_DIR}/ACallback
)

target_link_libraries(axo_common 
    PRIVATE
        Qt6::Svg
    PUBLIC 
        Qt6::Widgets
)