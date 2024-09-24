cmake_minimum_required(VERSION 3.14)

project(HybrdLink)

# Set C++17 as required standard for the project
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 是否需要检查 MAC 地址
option(ENABLE_MAC_CHECK "Enable MAC address check" OFF)
# 是否需要检查 软件有效期
option(ENABLE_EXPIRATION_CHECK "Enable Software Expiration Check" ON)
# 仅编译GridView
option(ONLY_COMPILE_GRIDVIEW "Only compile the GridView" OFF)

if (ENABLE_MAC_CHECK)
    message(STATUS "ENABLE_MAC_CHECK ON.")
    add_definitions(-DENABLE_MAC_CHECK)
else ()
    message(STATUS "ENABLE_MAC_CHECK OFF.")
endif()

if (ENABLE_EXPIRATION_CHECK)
    message(STATUS "ENABLE_EXPIRATION_CHECK ON.")
    add_definitions(-DENABLE_EXPIRATION_CHECK)
else ()
    message(STATUS "ENABLE_EXPIRATION_CHECK OFF.")
endif()

if (ONLY_COMPILE_GRIDVIEW)
    message(STATUS "ONLY_COMPILE_GRIDVIEW ON.")
    add_definitions(-DONLY_COMPILE_GRIDVIEW)
else ()
    message(STATUS "ONLY_COMPILE_GRIDVIEW OFF.")
endif()

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
find_package(Qt5 COMPONENTS Core Network REQUIRED)

# Include the QScintilla library
set(QSCINTILLA_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/Qsci")
set(QSCINTILLA_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")

include_directories(${QSCINTILLA_INCLUDE_DIR})
link_directories(${QSCINTILLA_LIBRARY_DIR})

# Include the YAML library
set(YAML_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/yaml-cpp")
set(YAML_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")

include_directories(${YAML_INCLUDE_DIR})
link_directories(${YAML_LIBRARY_DIR})

# Include the QADVANCEDDOCK library
set(QADVANCEDDOCK_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/ads")
set(QADVANCEDDOCK_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")

include_directories(${QADVANCEDDOCK_INCLUDE_DIR})
link_directories(${QADVANCEDDOCK_LIBRARY_DIR})

# Include the OpenSSL library
set(OPENSSL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/openssl")
set(OPENSSL_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")

include_directories(${OPENSSL_INCLUDE_DIR})
link_directories(${OPENSSL_LIBRARY_DIR})

# Include the TCL library
set(TCL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/tcl")
set(TCL_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/tcl/bin")

include_directories(${TCL_INCLUDE_DIR})
link_directories(${TCL_LIBRARY_DIR})

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
    ipmanager/BlockMemoryGenerator/*.cpp
    component/tclconsole/TclConsole.cpp
    component/tclconsole/LineEditor.cpp
    component/tclconsole/OutputEditor.cpp
    settings/*.cpp
    include/tinyxml2/*.cpp
    entity/*.cpp
    *.cpp
)

# Add header files
file(GLOB PROJECT_HEADERS
    wizard/*.h
    utils/*.h
    utils/json.hpp
    widgets/*.h
    grid/*.h
    blocks/*h
    base/*.h
    entity/*.h
    dialog/*.h
    ipmanager/*.h
    ipmanager/BlockMemoryGenerator/*.h
    component/tclconsole/TclConsole.h
    component/tclconsole/LineEditor.h
    component/tclconsole/OutputEditor.h
    settings/*.h
    include/Qsci/*.h
    include/yaml-cpp/*.h
    include/tinyxml2/*.h
    *.h
)

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

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

# Link the Qt5 Widgets library and Third-Party library to the executable
target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets
    Qt5::Core
    Qt5::Network
    # HybrdLinkPlugin # qscintilla2_qt5
    qscintilla2_qt5
    libyaml-cpp.a
    qtadvanceddocking
    libcrypto-1_1-x64
    libssl-1_1-x64
    tcl86
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


