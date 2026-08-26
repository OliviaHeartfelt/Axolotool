set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS 
    Core 
    Gui
    Widgets
    OpenGLWidgets
    Svg
    SvgWidgets
    Sql
    REQUIRED
)

set(NE_MODULES
    ANodeEnvironment/ANodeEnvironment.h
)

#ANodeEnvironment - Registry
set(NE_REGISTRY_MODULES
    ANodeEnvironment/Registry/ARegistry.h
    ANodeEnvironment/Registry/details/Context.h
    ANodeEnvironment/Registry/details/RegistryTraits.h

    ANodeEnvironment/Registry/registry_base/RGBaseRegistry.h

    ANodeEnvironment/Registry/visual_registry/RGVisualRegistry.h
)

# ANodeEnvironment - Streaming
set(NE_STREAMING_MODULES

    ANodeEnvironment/Streaming/STStreamingManager.h
    ANodeEnvironment/Streaming/details/Helper.h
    ANodeEnvironment/Streaming/details/BoundedQueue.h

    # Loading Node Consumer
    ANodeEnvironment/Streaming/loading_node_consumer/STLoadingNodeConsumer.h

    # Loading Wire Consumer
    ANodeEnvironment/Streaming/loading_wire_consumer/STLoadingWireConsumer.h

    # Loading Node Streamer
    ANodeEnvironment/Streaming/loading_node_streamer/STLoadingNodeStreamer.h
    ANodeEnvironment/Streaming/loading_node_streamer/details/Config.h
    ANodeEnvironment/Streaming/loading_node_streamer/details/NodeProcessing.h

    # Loading Wire Streamer
    ANodeEnvironment/Streaming/loading_wire_streamer/STLoadingWireStreamer.h
    ANodeEnvironment/Streaming/loading_wire_streamer/details/Config.h
    ANodeEnvironment/Streaming/loading_wire_streamer/details/WireProcessing.h

    # Saving Consumer
    ANodeEnvironment/Streaming/saving_consumer/STSavingConsumer.h
    ANodeEnvironment/Streaming/saving_consumer/details/SavingApplier.h

    # Saving Streamer
    ANodeEnvironment/Streaming/saving_streamer/STSavingStreamer.h
)

# ANodeEnvironment - View
set(NE_vIEW_MODULES

    ANodeEnvironment/View/AView.h

    # Canvas
    ANodeEnvironment/View/canvas/VWCanvas.h
    ANodeEnvironment/View/canvas/details/CanvasView.h

    # Scene
    ANodeEnvironment/View/scene/VWNodeScene.h

    # Drag Drop
    ANodeEnvironment/View/drag_drop/VWDragDrop.h
    ANodeEnvironment/View/drag_drop/details/Context.h

    # Node
    ANodeEnvironment/View/node/VWNode.h
    ANodeEnvironment/View/node/details/Context.h
    ANodeEnvironment/View/node/details/CreateNode.h
    ANodeEnvironment/View/node/details/NodeGrid.h
    ANodeEnvironment/View/node/details/NodeItem.h

    # Cell
    ANodeEnvironment/View/cell/VWCell.h
    ANodeEnvironment/View/cell/details/CellItem.h
    ANodeEnvironment/View/cell/details/Concepts.h
    ANodeEnvironment/View/cell/details/Context.h
    ANodeEnvironment/View/cell/details/CreateCell.h
    ANodeEnvironment/View/cell/details/CreatePin.h
    ANodeEnvironment/View/cell/details/CreateWidget.h
    ANodeEnvironment/View/cell/details/Helpers.h

    # Pin
    ANodeEnvironment/View/pin/VWPin.h
    ANodeEnvironment/View/pin/details/PinAllowSet.h
    ANodeEnvironment/View/pin/details/PinData.h
    ANodeEnvironment/View/pin/details/PinItem.h
    ANodeEnvironment/View/pin/details/Concepts.h
    ANodeEnvironment/View/pin/details/Context.h

    # Widget
    ANodeEnvironment/View/widget/VWWidget.h
    ANodeEnvironment/View/widget/details/Context.h
    ANodeEnvironment/View/widget/details/WidgetWrapper.h

    # Wire
    ANodeEnvironment/View/wire/VWWire.h
    ANodeEnvironment/View/wire/details/Concepts.h
    ANodeEnvironment/View/wire/details/Context.h
    ANodeEnvironment/View/wire/details/CreateWire.h
    ANodeEnvironment/View/wire/details/Helpers.h
    ANodeEnvironment/View/wire/details/WireItem.h
    ANodeEnvironment/View/wire/details/WireTemp.h
)

# ANodeEnvironment - Storage
set(NE_STORAGE_MODULES

    # Root
    ANodeEnvironment/Storage/ANodeEnvDB.h
    ANodeEnvironment/Storage/NDConfig.h
    ANodeEnvironment/Storage/NDConcepts.h
    ANodeEnvironment/Storage/NDHelpers.h
    ANodeEnvironment/Storage/NDPool.h

    # Node
    ANodeEnvironment/Storage/node_source/NDNodeSource.h
    ANodeEnvironment/Storage/node_source/details/Config.h
    ANodeEnvironment/Storage/node_source/details/Init.h
    ANodeEnvironment/Storage/node_source/details/Create.h
    ANodeEnvironment/Storage/node_source/details/Read.h
    ANodeEnvironment/Storage/node_source/details/Update.h
    ANodeEnvironment/Storage/node_source/details/Delete.h

    # Node Source
    ANodeEnvironment/Storage/node/NDNode.h
    ANodeEnvironment/Storage/node/details/Config.h
    ANodeEnvironment/Storage/node/details/Init.h
    ANodeEnvironment/Storage/node/details/Create.h
    ANodeEnvironment/Storage/node/details/Read.h
    ANodeEnvironment/Storage/node/details/Update.h
    ANodeEnvironment/Storage/node/details/Delete.h

    # Cell
    ANodeEnvironment/Storage/cell/NDCell.h
    ANodeEnvironment/Storage/cell/details/Config.h
    ANodeEnvironment/Storage/cell/details/Init.h
    ANodeEnvironment/Storage/cell/details/Helper.h
    ANodeEnvironment/Storage/cell/details/Create.h
    ANodeEnvironment/Storage/cell/details/Read.h
    ANodeEnvironment/Storage/cell/details/Update.h
    ANodeEnvironment/Storage/cell/details/Delete.h

    # Pin
    ANodeEnvironment/Storage/pin/NDPin.h
    ANodeEnvironment/Storage/pin/details/Config.h
    ANodeEnvironment/Storage/pin/details/Init.h
    ANodeEnvironment/Storage/pin/details/Create.h
    ANodeEnvironment/Storage/pin/details/Read.h
    ANodeEnvironment/Storage/pin/details/Update.h
    ANodeEnvironment/Storage/pin/details/Delete.h

    # Pin Source
    ANodeEnvironment/Storage/pin_source/NDPinSource.h
    ANodeEnvironment/Storage/pin_source/details/Config.h
    ANodeEnvironment/Storage/pin_source/details/Init.h
    ANodeEnvironment/Storage/pin_source/details/Create.h
    ANodeEnvironment/Storage/pin_source/details/Read.h
    ANodeEnvironment/Storage/pin_source/details/Update.h
    ANodeEnvironment/Storage/pin_source/details/Delete.h

    # Widget
    ANodeEnvironment/Storage/widget/NDWidget.h
    ANodeEnvironment/Storage/widget/details/Config.h
    ANodeEnvironment/Storage/widget/details/Init.h
    ANodeEnvironment/Storage/widget/details/Create.h
    ANodeEnvironment/Storage/widget/details/Read.h
    ANodeEnvironment/Storage/widget/details/Update.h
    ANodeEnvironment/Storage/widget/details/Delete.h

    # Widget Source
    ANodeEnvironment/Storage/widget_source/NDWidgetSource.h
    ANodeEnvironment/Storage/widget_source/details/Config.h
    ANodeEnvironment/Storage/widget_source/details/Init.h
    ANodeEnvironment/Storage/widget_source/details/Create.h
    ANodeEnvironment/Storage/widget_source/details/Read.h
    ANodeEnvironment/Storage/widget_source/details/Update.h
    ANodeEnvironment/Storage/widget_source/details/Delete.h

    # Wire
    ANodeEnvironment/Storage/wire/NDWire.h
    ANodeEnvironment/Storage/wire/details/Config.h
    ANodeEnvironment/Storage/wire/details/Init.h
    ANodeEnvironment/Storage/wire/details/Create.h
    ANodeEnvironment/Storage/wire/details/Read.h
    ANodeEnvironment/Storage/wire/details/Update.h
    ANodeEnvironment/Storage/wire/details/Delete.h

    # Wire Source
    ANodeEnvironment/Storage/wire_source/NDWireSource.h
    ANodeEnvironment/Storage/wire_source/details/Config.h
    ANodeEnvironment/Storage/wire_source/details/Init.h
    ANodeEnvironment/Storage/wire_source/details/Create.h
    ANodeEnvironment/Storage/wire_source/details/Read.h
    ANodeEnvironment/Storage/wire_source/details/Update.h
    ANodeEnvironment/Storage/wire_source/details/Delete.h

    # Global Source
    ANodeEnvironment/Storage/global_source/NDGlobalSource.h
    ANodeEnvironment/Storage/global_source/details/Config.h
    ANodeEnvironment/Storage/global_source/details/Init.h
    ANodeEnvironment/Storage/global_source/details/Create.h
    ANodeEnvironment/Storage/global_source/details/Read.h
    ANodeEnvironment/Storage/global_source/details/Update.h
    ANodeEnvironment/Storage/global_source/details/Delete.h
)

# Window
set(AWINDOW_MODULES
    AWindow/AMainWindow.h
    AWindow/AWindow.h
    AWindow/MockPlugin.h
)

# Utility
set(UTILITY_MODULES
    Utility/Utility.h
    Utility/details/UUID.h
    Utility/details/StylesUtility.h
    Utility/details/SqlTransaction.h
    Utility/details/ByteArray.h
)

add_executable(axo_editor 
    main.cpp
    ${CMAKE_SOURCE_DIR}/assets/tabler/icons/TablerIconsResources.qrc
    
    ${NE_MODULES}
    ${NE_REGISTRY_MODULES}
    ${NE_STREAMING_MODULES}
    ${NE_vIEW_MODULES}
    ${NE_STORAGE_MODULES}

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
    <mutex>
    <shared_mutex>
    <condition_variable>
    <cassert>
    <expected>
    <generator>
    <queue>
    <unordered_map>

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
    <QTimer>
    <QElapsedTimer>
    <QFuture>
    <QtConcurrent/QtConcurrent>

    # Dir
    <QIODevice>
    <QDir>
    <QDirIterator>
    <QFile>
    
    # Database subsystem headers
    <QSqlDatabase>
    <QSqlQuery>
    <QSqlError>
    <QSqlDriver>
    
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
    <QMarginsF>
    <QMouseEvent>
    <QScrollBar>
    <QSurfaceFormat>
    <QOpenGLWidget>
    <QKeyEvent>
    <QKeySequence>
    <QGraphicsOpacityEffect>
    <QPropertyAnimation>
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
    Qt6::OpenGLWidgets
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