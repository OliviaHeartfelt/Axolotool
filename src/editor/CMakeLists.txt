cmake_minimum_required(VERSION 3.28)
project(AxoEditor)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

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
    ANode/cell/ACell.ixx
    ANode/cell/ACellItem.ixx
)

set(ANODE_NODE_MODULES
    ANode/node/ANode.ixx
    ANode/node/ANodeGrid.ixx
)

set(ANODE_DB_MODULES
    ANode/node_env_db/ANodeEnvDB.ixx
    ANode/node_env_db/NDCell.ixx
    ANode/node_env_db/NDConfig.ixx
    ANode/node_env_db/NDNode.ixx
)

set(ANODE_PIN_MODULES
    ANode/pin/APin.ixx
    ANode/pin/APinAllowLists.ixx
    ANode/pin/APinData.ixx
    ANode/pin/APinDrag.ixx
    ANode/pin/APinFactory.ixx
    ANode/pin/APinFlags.ixx
    ANode/pin/APinItem.ixx
    ANode/pin/APinRegistry.ixx
)

set(ANODE_REGISTRY_MODULES
    ANode/registry/ARegistry.ixx
    ANode/registry/FRegistryKey.ixx
    ANode/registry/IRegistry.ixx
)

set(ANODE_SCENE_MODULES
    ANode/scene/ANodeScene.ixx
)

set(ANODE_WIRE_MODULES
    ANode/wire/AWire.ixx
    ANode/wire/AWireData.ixx
    ANode/wire/AWireItem.ixx
    ANode/wire/AWireTemp.ixx
)

#AWindow
set(AWINDOW_MODULES
    AWindow/ACanvas.ixx
    AWindow/AMainWindow.ixx
    AWindow/AWindow.ixx
)

#AStyle
set(ASTYLE_MODULES
    AStyle/ACombineStyles.ixx
)

add_executable(axo_editor 
    main.cpp
    ${CMAKE_SOURCE_DIR}/assets/tabler/icons/TablerIconsResources.qrc
)

target_compile_features(axo_editor PUBLIC cxx_std_23)

target_precompile_headers(axo_editor PRIVATE
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

target_sources(axo_editor 
    PRIVATE 
    FILE_SET CXX_MODULES 
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
    FILES 
        ${ANODE_CELL_MODULES}
        ${ANODE_NODE_MODULES}
        ${ANODE_DB_MODULES}
        ${ANODE_PIN_MODULES}
        ${ANODE_REGISTRY_MODULES}
        ${ANODE_SCENE_MODULES}
        ${ANODE_WIRE_MODULES}
        ${AWINDOW_MODULES}
        ${ASTYLE_MODULES}
)

target_link_libraries(axo_editor PRIVATE 
    Qt6::Widgets
    Qt6::Svg
    Qt6::SvgWidgets
    Qt6::Gui 
    Qt6::Core
    Qt6::Sql
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