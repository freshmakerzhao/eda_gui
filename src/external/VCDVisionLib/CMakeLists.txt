
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


