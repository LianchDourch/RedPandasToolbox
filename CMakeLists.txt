cmake_minimum_required(VERSION 3.16)

project(RedPandasToolbox LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Gui)
qt_standard_project_setup()

add_library(RedPandasToolbox SHARED
  RedPandasToolbox_global.h
  redpandastoolbox.cpp
  redpandastoolbox.h
  rmath.h rmath.cpp
  geometry.h geometry.cpp
  array.h array.cpp
  groups.h groups.cpp
  types.h types.cpp
  util.h util.cpp

  queue.h queue.cpp
  arraylist.h arraylist.cpp
  rstringutils.h rstringutils.cpp
  saveable.h saveable.cpp
  uniqueid.h uniqueid.cpp
  imagewidget.h imagewidget.cpp
  rserializable.h rserializable.cpp
  masteredobject.h masteredobject.cpp
  widgetplacer.h widgetplacer.cpp
  floatingtext.h floatingtext.cpp
  graphicsscrollarea.h graphicsscrollarea.cpp
  robject.h robject.cpp
  vector.h vector.cpp
  rfwd.h
  rfunctions.h rfunctions.cpp
  pointerlifekeeper.h pointerlifekeeper.cpp
  rlinkedlist.h rlinkedlist.cpp
  flagmanager.h flagmanager.cpp
  cowptr.h cowptr.cpp
  rpointers.h rpointers.cpp
  hashmap.h hashmap.cpp
  itemdialog.h itemdialog.cpp


)

target_include_directories(RedPandasToolbox
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(RedPandasToolbox
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets
)

target_compile_definitions(RedPandasToolbox PRIVATE REDPANDASTOOLBOX_LIBRARY)

include(GNUInstallDirs)

# Installer la bibliothèque compilée
install(TARGETS RedPandasToolbox
    EXPORT RedPandasToolboxTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}      # pour Windows DLL
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}      # pour Linux/Mac .so/.dylib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}      # pour .lib ou .a
)

# Installer les headers
install(FILES
    RedPandasToolbox_global.h
    redpandastoolbox.h
    rmath.h
    geometry.h
    array.h
    groups.h
    types.h
    util.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/RedPandasToolbox
)

# Exporter la cible pour find_package
install(EXPORT RedPandasToolboxTargets
    FILE RedPandasToolboxConfig.cmake
    NAMESPACE RedPandas::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RedPandasToolbox
)
