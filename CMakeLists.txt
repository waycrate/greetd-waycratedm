cmake_minimum_required(VERSION 3.16)

project(waycratelock
  VERSION 0.1
  LANGUAGES CXX
)

option(DEBUG_MODE "Debug not use sessionlock" OFF)

if(DEBUG_MODE)
  add_definitions(-DDEBUG_MODE)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX /usr)
endif ()

include(GNUInstallDirs)

find_package(Qt6 COMPONENTS Quick QuickControls2 Concurrent WaylandClient REQUIRED)

find_package(tomlplusplus REQUIRED)
find_package(SessionLockQt REQUIRED)

find_package(PkgConfig REQUIRED)

qt_add_executable(waycratelock
  src/main.cpp
  asserts/source.qrc
)

target_include_directories(waycratelock PUBLIC
  ${CMAKE_SOURCE_DIR}/src
)

qt_add_qml_module(waycratelock
  URI WayCrateLock
  VERSION 1.0
  SOURCES
    src/CommandLine.h
    src/CommandLine.cpp
  QML_FILES
    qml/main.qml
    qml/TimePanel.qml
    qml/MainView.qml
)

target_link_libraries(waycratelock
  PRIVATE
  Qt6::Quick
  Qt6::WaylandClient
  Qt6::QuickControls2
  Qt6::Concurrent
  SessionLockQtInterface)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/misc/waycratelock DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/pam.d)

install(TARGETS waycratelock DESTINATION ${CMAKE_INSTALL_BINDIR})
