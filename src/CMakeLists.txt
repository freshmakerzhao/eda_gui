cmake_minimum_required(VERSION 3.14)

project(HybrdLink)

# Set C++17 as required standard for the project
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# 检测操作系统
if(WIN32)
    message(STATUS "Configuring on/for Windows")
    # 特定于Windows的编译选项
    add_definitions(-DWINDOWS_PLATFORM)
elseif(UNIX AND NOT APPLE)
    message(STATUS "Configuring on/for Linux")
    # 特定于Linux的编译选项
    add_definitions(-DLINUX_PLATFORM)
elseif(APPLE)
    message(STATUS "Configuring on/for macOS")
    # 特定于macOS的编译选项
    add_definitions(-DMACOS_PLATFORM)
endif()

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

# Include the QADVANCEDDOCK library
set(QADVANCEDDOCK_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ads")
set(QADVANCEDDOCK_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")

include_directories(${QADVANCEDDOCK_INCLUDE_DIR})
link_directories(${QADVANCEDDOCK_LIBRARY_DIR})

# Add source files
file(GLOB PROJECT_SOURCES
    wizard/*.cpp
    utils/*.cpp
    widgets/*.cpp
    grid/*.cpp
    blocks/*.cpp
    base/*.cpp
    dialog/*.cpp
    ipmanager/*.cpp
    prjsummary/*.cpp
    ipmanager/BlockMemoryGenerator/*.cpp
    settings/*.cpp
    tinyxml2/*.cpp
    entity/*.cpp
    *.cpp
)

# Add header files
file(GLOB PROJECT_HEADERS
    wizard/*.h
    utils/*.h
    widgets/*.h
    grid/*.h
    blocks/*h
    base/*.h
    entity/*.h
    dialog/*.h
    ipmanager/*.h
    prjsummary/*.h
    ipmanager/BlockMemoryGenerator/*.h
    settings/*.h
    Qsci/*.h
    yaml-cpp/*.h
    tinyxml2/*.h
    *.h
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
    win32.rc
)

# Link the Qt5 Widgets library and the QScintilla library to the executable
target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets
    qscintilla2_qt5
    libyaml-cpp.a
    qtadvanceddocking
)

# Specify installation paths (if required)
# install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION /opt/${PROJECT_NAME}/bin)

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


