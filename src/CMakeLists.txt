cmake_minimum_required(VERSION 3.14)

project(eda_gui)

# Set C++17 as required standard for the project
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Include the QScintilla library
set(QSCINTILLA_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Qsci")
set(QSCINTILLA_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")

include_directories(${QSCINTILLA_INCLUDE_DIR})
link_directories(${QSCINTILLA_LIBRARY_DIR})

# Add source files
set(PROJECT_SOURCES
    chipplanner.cpp
    editor.cpp
    infowidget.cpp
    main.cpp
    mainwindow.cpp
    navigator.cpp
    project.cpp
    wizard.cpp
    taskview.cpp
    processmanager.cpp
)

# Add header files
set(PROJECT_HEADERS
    chipplanner.h
    editor.h
    infowidget.h
    mainwindow.h
    navigator.h
    project.h
    wizard.h
    taskview.h
    processmanager.h
    ./Qsci/qsciscintilla.h
    ./Qsci/qsciscintillabase.h
    ./Qsci/qscilexercpp.h
    ./Qsci/qscilexercustom.h
    ./Qsci/qscilexer.h
    ./Qsci/qsciabstractapis.h
)

# Add resource files
set(PROJECT_RESOURCES
    res.qrc
)

# Tell CMake to create the executable
add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${PROJECT_RESOURCES} 
)

# Link the Qt5 Widgets library and the QScintilla library to the executable
target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets
    qscintilla2_qt5
)

# Specify installation paths (if required)
# install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION /opt/${PROJECT_NAME}/bin)
