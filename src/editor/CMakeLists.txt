set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS 
    Core 
    Gui
    Widgets
    Svg
    SvgWidgets
    Sql
    REQUIRED
)

#ANode
set(ANODE_CELL_MODULES
    ANode/cell/ACell.h
    ANode/cell/ACellItem.h
)

set(ANODE_NODE_MODULES
    ANode/node/ANode.h
    ANode/node/ANodeGrid.h
)

set(ANODE_PIN_MODULES
    ANode/pin/APin.h
    ANode/pin/APinAllowLists.h
    ANode/pin/APinData.h
    ANode/pin/APinDrag.h
    ANode/pin/APinFactory.h
    ANode/pin/APinFlags.h
    ANode/pin/APinItem.h
    ANode/pin/APinRegistry.h
)

set(ANODE_REGISTRY_MODULES
    ANode/registry/ARegistry.h
    ANode/registry/FRegistryKey.h
    ANode/registry/IRegistry.h
)

set(ANODE_SCENE_MODULES
    ANode/scene/ANodeScene.h
)

set(ANODE_WIRE_MODULES
    ANode/wire/AWire.h
    ANode/wire/AWireData.h
    ANode/wire/AWireItem.h
    ANode/wire/AWireTemp.h
)

set(ANODE_DB_MODULES
    ANode/node_env_db/ANodeEnvDB.h
    ANode/node_env_db/NDConfig.h
    ANode/node_env_db/NDConcepts.h

    #Node
    ANode/node_env_db/node/NDNode.h
    ANode/node_env_db/node/details/Config.h
    ANode/node_env_db/node/details/Init.h
    ANode/node_env_db/node/details/Create.h
    ANode/node_env_db/node/details/Read.h
    ANode/node_env_db/node/details/Update.h
    ANode/node_env_db/node/details/Delete.h

    #Cell
    ANode/node_env_db/cell/NDCell.h
    ANode/node_env_db/cell/details/Config.h
    ANode/node_env_db/cell/details/Init.h
    ANode/node_env_db/cell/details/Helper.h
    ANode/node_env_db/cell/details/Create.h
    ANode/node_env_db/cell/details/Read.h
    ANode/node_env_db/cell/details/Update.h
    ANode/node_env_db/cell/details/Delete.h

    #Pin
    ANode/node_env_db/pin/NDPin.h
    ANode/node_env_db/pin/details/Config.h
    ANode/node_env_db/pin/details/Init.h
    ANode/node_env_db/pin/details/Create.h
    ANode/node_env_db/pin/details/Read.h
    ANode/node_env_db/pin/details/Update.h
    ANode/node_env_db/pin/details/Delete.h

    #Pin Source
    ANode/node_env_db/pin_source/NDPinSource.h
    ANode/node_env_db/pin_source/details/Config.h
    ANode/node_env_db/pin_source/details/Init.h
    ANode/node_env_db/pin_source/details/Create.h
    ANode/node_env_db/pin_source/details/Read.h
    ANode/node_env_db/pin_source/details/Update.h
    ANode/node_env_db/pin_source/details/Delete.h
)

#AWindow
set(AWINDOW_MODULES
    AWindow/ACanvas.h
    AWindow/AMainWindow.h
    AWindow/AWindow.h
)

#AStyle
set(ASTYLE_MODULES
    AStyle/ACombineStyles.h
)

set(UTILITY_MODULES
    Utility/Utility.h
    Utility/details/UUID.h
    Utility/details/SqlTransaction.h
)

add_executable(axo_editor 
    main.cpp
    ${CMAKE_SOURCE_DIR}/assets/tabler/icons/TablerIconsResources.qrc
    
    ${ANODE_CELL_MODULES}
    ${ANODE_NODE_MODULES}
    ${ANODE_DB_MODULES}
    ${ANODE_PIN_MODULES}
    ${ANODE_REGISTRY_MODULES}
    ${ANODE_SCENE_MODULES}
    ${ANODE_WIRE_MODULES}
    ${AWINDOW_MODULES}
    ${ASTYLE_MODULES}
    ${UTILITY_MODULES}
)

target_compile_features(axo_editor PUBLIC cxx_std_23)

target_precompile_headers(axo_editor PRIVATE
    # Third-party
    <modern-uuid/uuid.h>

    # C++
    <algorithm>
    <initializer_list>
    <concepts>
    <format>
    <cstdint>
    <memory>
    <atomic>
    <utility>
    <type_traits>
    <optional>
    <string>
    <vector>
    <variant>

    # Core Qt used everywhere
    <QString>
    <QList>
    <QSet>
    <QDebug>
    <QPointF>
    <QReadWriteLock>
    <QVariant>
    <QApplication>
    <QHash>
    <QtAlgorithms>
    <QtMath>
    <QByteArray>
    <QtTypes>
    <QMetaObject>
    <QCoreApplication>
    <QTextStream>
    <QStringList>

    # Dir
    <QIODevice>
    <QDir>
    <QDirIterator>
    <QFile>
    
    # Database subsystem headers
    <QSqlDatabase>
    <QSqlQuery>
    <QSqlError>
    
    # UI / Canvas subsystem headers
    <QWidget>
    <QGraphicsScene>
    <QGraphicsItem>
    <QMimeData>
    <QDrag>
    <QDataStream>
    <QGraphicsSceneMouseEvent>
    <QGraphicsSceneDragDropEvent>
    <QGraphicsSvgItem>
    <QSvgRenderer>
    <QPainter>
    <QImage>
    <QColor>
    <QGraphicsPathItem>
    <QPen>
    <QPainterPath>
    <QGraphicsView>
    <QGraphicsRectItem>
    <QMainWindow>
    <QHBoxLayout>
    <QPushButton>
    <QFrame>
    <QVBoxLayout>
    <QLabel>
    <QRandomGenerator>
    <QGraphicsSimpleTextItem>
    <QStyleOptionGraphicsItem>
    <QGraphicsColorizeEffect>
)

include(FetchContent)
FetchContent_Declare(modern-uuid
  GIT_REPOSITORY https://github.com/gershnik/modern-uuid.git
  GIT_TAG v2.4
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(modern-uuid)

target_link_libraries(axo_editor PRIVATE 
    Qt6::Widgets
    Qt6::Svg
    Qt6::SvgWidgets
    Qt6::Gui 
    Qt6::Core
    Qt6::Sql
    modern-uuid::modern-uuid-static
)

set_target_properties(axo_editor PROPERTIES
    WIN32_EXECUTABLE ON
    CXX_SCAN_FOR_MODULES ON
)

if(WIN32)
    get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    
    add_custom_command(TARGET axo_editor POST_BUILD
        COMMAND "${_qt_bin_dir}/windeployqt" 
                --debug 
                "$<TARGET_FILE:axo_editor>"
        COMMENT "Running Windows Qt deployment asset copies..."
    )
endif()