# config.cmake

# 设置 vcpkg 路径
set(VCPKG_PATH "E:/workspace/vcpkg" CACHE STRING "Path to vcpkg installation")
# 设置 resource_win 路径
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(RESOURCE_WIN_PATH "E:/workspace/eda_gui_resource/resource_win" CACHE STRING "Path to resource_win directory (Release)")
else()
    set(RESOURCE_WIN_PATH "E:/workspace/eda_gui_resource/resource_win" CACHE STRING "Path to resource_win directory (Debug)")
endif()
# 设置 项目字体 路径（除编辑器界面）
set(FONTS_PATH "E:/workspace/eda_gui_resource/Fonts" CACHE STRING "Path to Fonts directory")
add_definitions(-DRESOURCE_WIN_PATH="${RESOURCE_WIN_PATH}")
add_definitions(-DFONTS_PATH="${FONTS_PATH}")