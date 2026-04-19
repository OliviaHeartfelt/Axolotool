find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(axo_editor main.cpp)

target_link_libraries(axo_editor PRIVATE 
    Qt6::Widgets
    axo_common
)

set_target_properties(axo_editor PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)