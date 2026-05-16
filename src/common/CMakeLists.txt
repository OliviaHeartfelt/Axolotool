#cmake_minimum_required(VERSION 3.28)
#project(AxoCommon)
#
## Enable C++23 for modules
#set(CMAKE_CXX_STANDARD 23)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#
## Enable Qt Auto-tools
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTORCC ON)
#
#find_package(Qt6 REQUIRED COMPONENTS Widgets)
#
#file(GLOB_RECURSE COMMON_FILES CONFIGURE_DEPENDS "*.cpp" "*.h" "*.ui" "*.qss" "*.qrc")
#file(GLOB_RECURSE COMMON_MODULES CONFIGURE_DEPENDS "*.ixx")
#set_source_files_properties(${COMMON_MODULES} PROPERTIES 
#    SKIP_AUTOGEN OFF
#    SKIP_AUTOMOC OFF
#    LANGUAGE CXX
#)
#
#set(Qt6_MOC_EXECUTABLE_FLAGS "-D_CXX_MODULES_")
#
#add_library(axo_common OBJECT)
#target_sources(axo_common
#    PUBLIC 
#        FILE_SET my_modules 
#        TYPE CXX_MODULES 
#        FILES 
#            ${COMMON_MODULES}
#)
#target_sources(axo_common PRIVATE ${COMMON_FILES} "ACommonResources.qrc")
#target_link_libraries(axo_common PUBLIC Qt6::Widgets)
#
## Handle the global resources and styles right here
#add_library(axo_common_base OBJECT)
#target_sources(axo_common_base 
#    PUBLIC 
#        FILE_SET base_modules TYPE CXX_MODULES FILES "ACommonStyles.ixx"
#    PRIVATE 
#        "ACommonResources.qrc"
#)
#target_link_libraries(axo_common_base PUBLIC Qt6::Widgets)

cmake_minimum_required(VERSION 3.28)
project(AxoCommon)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

file(GLOB_RECURSE COMMON_FILES CONFIGURE_DEPENDS "*.cpp" "*.h" "*.ui" "*.qss")
file(GLOB_RECURSE COMMON_MODULES CONFIGURE_DEPENDS "*.ixx")

set_source_files_properties(${COMMON_MODULES} PROPERTIES
    SKIP_AUTOGEN OFF
    SKIP_AUTOMOC OFF
    LANGUAGE CXX
)

add_library(axo_common OBJECT)

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

target_link_libraries(axo_common PUBLIC Qt6::Widgets)