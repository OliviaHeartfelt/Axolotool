set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ------------------------------------------------------------------------------
# Dependencies
# ------------------------------------------------------------------------------
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

include(FetchContent)
FetchContent_Declare(modern-uuid
    GIT_REPOSITORY https://github.com/gershnik/modern-uuid.git
    GIT_TAG v2.4
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(modern-uuid)

# ------------------------------------------------------------------------------
# Source & Header Gathering
# ------------------------------------------------------------------------------
file(GLOB_RECURSE NE_MODULES CONFIGURE_DEPENDS "ANodeEnvironment/*.h")
file(GLOB_RECURSE AWINDOW_MODULES CONFIGURE_DEPENDS "AWindow/*.h")
file(GLOB_RECURSE UTILITY_MODULES CONFIGURE_DEPENDS "Utility/*.h")

add_executable(axo_editor 
    main.cpp
    MockPlugin.h
    ${CMAKE_SOURCE_DIR}/assets/tabler/icons/TablerIconsResources.qrc
    
    ${NE_MODULES}
    ${AWINDOW_MODULES}
    ${UTILITY_MODULES}
)

# ------------------------------------------------------------------------------
# Target Configuration & Features
# ------------------------------------------------------------------------------
target_compile_features(axo_editor PUBLIC cxx_std_23)

set_target_properties(axo_editor PROPERTIES
    WIN32_EXECUTABLE ON
    CXX_SCAN_FOR_MODULES ON
)

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

# ------------------------------------------------------------------------------
# Precompiled Headers
# ------------------------------------------------------------------------------
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
    <cstddef>
    <new>

    # Core Qt
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
    
    # Database
    <QSqlDatabase>
    <QSqlQuery>
    <QSqlError>
    <QSqlDriver>
    
    # UI / Canvas
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

# ------------------------------------------------------------------------------
# Custom Build Commands
# ------------------------------------------------------------------------------
add_custom_command(
    TARGET axo_editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/MockPluginJson.json"
        "${CMAKE_CURRENT_BINARY_DIR}/plugin/MockPluginJson.json"
    COMMENT "Copying JSON config to build directory"
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