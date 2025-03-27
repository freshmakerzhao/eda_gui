cmake_minimum_required(VERSION 3.5)

# 项目名称
set (library_name "QCustomPlot")
project(${library_name} CXX)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS PrintSupport REQUIRED )

# 设置源文件
set(qcustomplot_SRCS
    ${PROJECT_SOURCE_DIR}/src/qcustomplot.cpp
)
# 设置头文件路径
set(qcustomplot_HEADERS
   ${PROJECT_SOURCE_DIR}/include/qcustomplot.h )

# 创建库
add_library(${library_name} STATIC ${qcustomplot_SRCS} ${qcustomplot_HEADERS} )

# 链接 Qt的头文件
target_link_libraries(${library_name}
    Qt${QT_VERSION_MAJOR}::PrintSupport
)
