
cmake_minimum_required(VERSION 3.10)

set (library_name "VCDVisionLib")
project(${library_name} CXX)
#lib
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/QCustomPlot)
link_directories(${PROJECT_SOURCE_DIR}/lib/QCustomPlot)


# 设置源文件
set(vcdvisionlib_SRCS
    ${PROJECT_SOURCE_DIR}/src/VCDCustomPlot.cpp
    ${PROJECT_SOURCE_DIR}/src/VCDVision.cpp
    ${PROJECT_SOURCE_DIR}/src/ParserVCDJson.cpp
)

# 设置头文件路径
set(vcdvisionlib_HEADERS
   ${PROJECT_SOURCE_DIR}/include/VCDCustomPlot.h
   ${PROJECT_SOURCE_DIR}/include/VCDVision.h
   ${PROJECT_SOURCE_DIR}/include/DataStruct.h
   ${PROJECT_SOURCE_DIR}/include/ParserVCDJson.h
)

add_library(${library_name}  STATIC ${vcdvisionlib_SRCS}  ${vcdvisionlib_HEADERS}
     )


target_link_libraries(${library_name}    QCustomPlot )

target_include_directories(QCustomPlot PUBLIC
    ${PROJECT_SOURCE_DIR}/lib/QCustomPlot/include
)


# 设置 Python 脚本和 PyInstaller 命令
set(PYTHON_SCRIPT "${PROJECT_SOURCE_DIR}/scripts/parserVCD.py")
set(PYINSTALLER_COMMAND "pyinstaller")

#  自定义输出目录
set(CUSTOM_DIST_DIR "${CMAKE_BINARY_DIR}")

# 添加自定义目标来构建 Python 脚本为可执行文件
add_custom_target(
    build_python_exe
    COMMAND ${PYINSTALLER_COMMAND} --onefile --distpath ${CUSTOM_DIST_DIR} ${PYTHON_SCRIPT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Building Python script to executable using PyInstaller"
    SOURCES ${PYTHON_SCRIPT}
)
# 确保在构建 C++ 项目之前先构建 Python 可执行文件
add_dependencies(${PROJECT_NAME} build_python_exe)
