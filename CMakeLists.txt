cmake_minimum_required(VERSION 3.0)

# Initial Setup ============================================

set(PROJECT_NAME libraqm_test)
project(${PROJECT_NAME})

set( CMAKE_BUILD_TYPE Debug)
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )

if( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
      # using clang
      set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Weverything")
    elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
      # using gcc
      set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wall -Wextra")
    elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "Intel")
      # using intel c compiler
    elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
      # using visual studio c compiler
    endif()
endif()

set(CMAKE_AUTOMOC ON) # Automatically MOC source files

if(NOT CMAKE_VERSION VERSION_LESS 3.0)
  set(CMAKE_AUTOUIC ON) # Automatically UIC .ui files (CMake3+ only)
endif()

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(RAQM REQUIRED IMPORTED_TARGET raqm)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${RAQM_INCLUDE_DIRS})
set(SOURCES main.cc mainwindow.cc paintwidget.cc layoutengine.cc)

add_executable( ${PROJECT_NAME} ${SOURCES})
target_link_libraries( ${PROJECT_NAME} Qt5::Core Qt5::Widgets Qt5::Gui ${RAQM_LIBRARIES})
