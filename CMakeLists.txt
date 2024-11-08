cmake_minimum_required(VERSION 3.5)

project(amplifiers VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required Qt packages
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Gui PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Gui PrintSupport)

# Add the directory where qcustomplot.h is located
include_directories(${CMAKE_SOURCE_DIR}/include)

# # Add the PCAN library directory
# link_directories(${CMAKE_SOURCE_DIR}/libs)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    qcustomplot.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(amplifiers
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        cardmodel.h cardmodel.cpp
        carddelegate.h carddelegate.cpp
        loadcell.h loadcell.cpp
        calibrationpopup.ui
        connectpopup.h connectpopup.cpp connectpopup.ui
        configdialog.h configdialog.cpp configdialog.ui
        samplingpage.h samplingpage.cpp samplingpage.ui
        style/material-blue.qss
        utility.cpp
        utility.h
        PCANBasic.h
        stdafx.h
        globaltimer.h globaltimer.cpp
        advanceddialog.h advanceddialog.cpp advanceddialog.ui
        globals.h globals.cpp
        filterconfig.h filterconfig.cpp filterconfig.ui







    )
else()
    if(ANDROID)
        add_library(amplifiers SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(amplifiers
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Link against Qt modules including PrintSupport
# target_link_libraries(amplifiers PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::PrintSupport)
target_link_libraries(amplifiers PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::PrintSupport
    "C:/Users/RobinLee/OneDrive - ANYLOAD Weigh & Measure/Documents/amplifiers/libs/PCANBasic.lib"
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_finalize_executable(amplifiers)
endif()

include(GNUInstallDirs)
install(TARGETS amplifiers
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
