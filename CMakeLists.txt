cmake_minimum_required(VERSION 3.16)
project(HurrigatorsProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_MINIMUM_VERSION 5.2)

# Point this to the absolute path of your personal Qt install
# This is annoying, but the 'offcial' way to configure this for Qt
# at the moment for some reason
list(APPEND CMAKE_PREFIX_PATH "D:/a/hurrigators-project/hurrigators-project/qt/Qt/5.15.2/mingw81_64/lib/cmake")

# Needed to ensure the AutoUic can find the ui form mocs
set(CMAKE_AUTOUIC_SEARCH_PATHS "forms")
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(headers)

# Find libosmscout files
add_subdirectory(lib/libosmscout)

# Find Qt packages after building libosmscout
find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core)

# Leaving these in place in case they're needed for debugging
# message(WARNING ${QT_VERSION_MAJOR})
# message(WARNING ${QT_VERSION_MINOR})

# Finding the components, ditto what I said below when linking OpenGL
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Widgets)

# Find Python components
find_package(Python COMPONENTS Interpreter Development REQUIRED)

# Manually add all headers and sources here since this is a relatively small project
set(PROJECT_SOURCES
    forms/mainwindow.ui

    headers/databridge.h
    headers/libosmhandler.h
    headers/mainwindow.h
    headers/maprenderer.h

    headers/rendereropenglwidget.h
    source/rendereropenglwidget.cpp

    source/databridge.cpp
    source/libosmhandler.cpp
    source/main.cpp
    source/mainwindow.cpp
    source/maprenderer.cpp
)

# Just in case compiling with Qt6 in the future
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(HurrigatorsProject
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
        add_executable(HurrigatorsProject
            ${PROJECT_SOURCES}
        )
endif()

# Don't allow Qt's "slots" keyword because that mucks up Python's includes
# NOTE:
# +---------+-----------+
# | emit    | Q_EMIT    |
# | signals | Q_SIGNALS |
# | slots   | Q_SLOTS   |
# +---------+-----------+
# Use the macro forms of these keywords for Qt signals/slots bindings
target_compile_definitions(HurrigatorsProject PRIVATE -DQT_NO_KEYWORDS)

# Link libosmscout
target_link_libraries(HurrigatorsProject PRIVATE OSMScout OSMScoutClientQt OSMScoutMapQt)

# Link the Qt modules
target_link_libraries(HurrigatorsProject PRIVATE
    Python::Python    

    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
)

set_target_properties(HurrigatorsProject PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS HurrigatorsProject
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(HurrigatorsProject)
endif()

# Include python files for the kmz crawler
configure_file(lib/nhc-crawler/main.py python/main.py COPYONLY)
configure_file(lib/nhc-crawler/kmzhtmlparser.py python/kmzhtmlparser.py COPYONLY)
configure_file(lib/nhc-crawler/fileoperations.py python/fileoperations.py COPYONLY)
configure_file(lib/nhc-crawler/archive.html python/archive.html COPYONLY)
