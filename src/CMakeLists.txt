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
# 打包部署
option(PACK_DEPLOYED "Pack and deployed" OFF)

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

if (PACK_DEPLOYED)
    message(STATUS "PACK_DEPLOYED ON.")
    add_definitions(-DPACK_DEPLOYED)
else ()
    message(STATUS "PACK_DEPLOYED OFF.")
endif()

# 检测操作系统
if(WIN32)
    message(STATUS "Configuring on/for Windows")
    # 特定于Windows的编译选项
    add_definitions(-DWINDOWS_PLATFORM)
    # Copy qt.conf
    configure_file(${CMAKE_SOURCE_DIR}/qt.conf ${CMAKE_BINARY_DIR}/qt.conf COPYONLY)
elseif(UNIX AND NOT APPLE)
    message(STATUS "Configuring on/for Linux")
    # 特定于Linux的编译选项
    add_definitions(-DLINUX_PLATFORM)
    set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
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
find_package(Qt5 COMPONENTS Core Xml REQUIRED)

# Include the PowerEditor library
add_subdirectory(external/XScintilla EXCLUDE_FROM_ALL)
include_directories(${CMAKE_SOURCE_DIR}/external/XScintilla/src)

# Include the YAML library
add_subdirectory(external/yaml-cpp)

# Include the QADVANCEDDOCK library
add_subdirectory(external/QtADS)
set(QADVANCEDDOCK_LIBRARY_DIR "${CMAKE_BINARY_DIR}")
link_directories(${QADVANCEDDOCK_LIBRARY_DIR}/external/QtADS)
include_directories(${CMAKE_SOURCE_DIR}/external/QtADS)

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

# Disable external-library warnings
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Use -w with GCC or Clang to disable all warnings
    target_compile_options(PowerEditor PRIVATE -w)
    target_compile_options(yaml-cpp PRIVATE -w)
    target_compile_options(qt5advanceddocking PRIVATE -w)
# elseif(MSVC)
    # Use /W0 with MSVC to disable all warnings
#     target_compile_options(PowerEditor PRIVATE /W0)
#     target_compile_options(yaml-cpp PRIVATE /W0)
#     target_compile_options(qt5advanceddocking PRIVATE /W0)
endif()

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
    ipmanager/common/*.cpp
    ipmanager/clk_wiz/*.cpp
    ipmanager/blk_mem_gen/*.cpp
    ipmanager/fifo_generator/*.cpp
    component/tclconsole/*.cpp
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
    ipmanager/common/*.h
    ipmanager/clk_wiz/*.h
    ipmanager/blk_mem_gen/*.h
    ipmanager/fifo_generator/*.h
    component/tclconsole/*.h
    settings/*.h
    include/Qsci/*.h
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
    Qt5::Xml
    PowerEditor
    yaml-cpp
    libqt5advanceddocking${CMAKE_SHARED_LIBRARY_SUFFIX}
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


