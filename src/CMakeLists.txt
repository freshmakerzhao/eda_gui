cmake_minimum_required(VERSION 3.14)

project(HybrdLink)

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
find_package(Qt5 COMPONENTS PrintSupport)
find_package(Qt5 COMPONENTS OpenGL)

# Include the QScintilla library
set(QSCINTILLA_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Qsci")
set(QSCINTILLA_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")

include_directories(${QSCINTILLA_INCLUDE_DIR})
link_directories(${QSCINTILLA_LIBRARY_DIR})

# Include the YAML library
set(YAML_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/yaml-cpp")
set(YAML_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")

include_directories(${YAML_INCLUDE_DIR})
link_directories(${YAML_LIBRARY_DIR})

# Add source files
set(PROJECT_SOURCES
    chipplanner.cpp
    editor.cpp
    infowidget.cpp
    main.cpp
    mainwindow.cpp
    navigator.cpp
    project.cpp
    taskmanager.cpp
    wizard/Wizard.cpp
    wizard/SourcePage.cpp
    wizard/ConstraintPage.cpp
    wizard/DefaultPartPage.cpp
    wizard/ProjectNamePage.cpp
    utils/json.hpp
    utils/ProcessManager.cpp
    utils/StringUtilities.cpp
    utils/CommandBuilder.cpp
    widgets/FrameView.cpp
    grid/ChipGridOperations.cpp
    grid/view.cpp
    blocks/Blocks.cpp
    blocks/Sites.cpp
    blocks/SitesBlock.cpp
    blocks/SitesSliceL.cpp
    blocks/Tiles.cpp
    base/Globals.cpp
    utils/TimeUtilities.cpp
)

# Add header files
set(PROJECT_HEADERS
    chipplanner.h
    editor.h
    infowidget.h
    mainwindow.h
    navigator.h
    project.h
    taskmanager.h
    wizard/Wizard.h
    wizard/SourcePage.h
    wizard/ConstraintPage.h
    wizard/DefaultPartPage.h
    wizard/ProjectNamePage.h
    Qsci/qsciscintilla.h
    Qsci/qsciscintillabase.h
    Qsci/qscilexercpp.h
    Qsci/qscilexercustom.h
    Qsci/qscilexer.h
    Qsci/qsciabstractapis.h
    yaml-cpp/yaml.h
    utils/ProcessManager.cpp
    utils/StringUtilities.h
    utils/CommandBuilder.h
    utils/FileHelper.h
    grid/ChipGridOperations.h
    grid/view.h
    blocks/Blocks.h
    blocks/Sites.h
    blocks/SitesBlock.h
    blocks/SitesSliceL.h
    blocks/Tiles.h
    entity/Cluster.h
    base/Globals.h
    utils/TimeUtilities.h
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
    libyaml-cpp.a
)

# Specify installation paths (if required)
# install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION /opt/${PROJECT_NAME}/bin)

set(app_WINRC appicon.rc)
set_property(SOURCE app_win32.rc APPEND PROPERTY
        OBJECT_DEPENDS ${PROJECT_SOURCE_DIR}/AppIcon.ico
)


if (Qt5PrintSupport_FOUND)
    target_link_libraries(${PROJECT_NAME} Qt5::PrintSupport)
endif()

if (Qt5OpenGL_FOUND)
    target_link_libraries(${PROJECT_NAME} Qt5::OpenGL)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    # 如果是Release模式，添加-mwindows标志
    set(CMAKE_CXX_FLAGS "-mwindows")
endif()


