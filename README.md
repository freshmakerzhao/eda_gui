# 一、介绍

这是一个对标vivado的eda工具，能够帮助用户实现综合、布局布线、码流生成等关键步骤。

主要技术栈为c++、qt、tcl，使用了以下几种第三方库：

- ExScintilla
  - 语法解析
- QtADS
  - 拖拽式布局
- VCDVisionLib
  - 波形绘制
- yaml-cpp
  - yaml解析
- compress_tool
  - 基于7z的加解密库
  - https://github.com/freshmakerzhao/compress_tool.git

其中，除了compress_tool之外，其他库均为源码嵌入。

# 二、配置

## 2.1 编译

项目根目录下的cmakelists.txt主要依赖 config_scripts 文件夹下的两个.cmake文件

- config.cmake
  - 用于设置开发人员vcpkg、resource_win等文件的读取路径；
- deploy_qt_dependencies.cmake
  - 用于拷贝所需依赖；

如果仅编译Gridview，编译时添加该选项

```
-DONLY_COMPILE_GRIDVIEW=ON
```

## 2.2 打包

打包发布，编译时添加该选项，主程序只在同级目录下寻找resource_win、fonts等文件，覆盖config.cmake中这几项的配置。

```
-DPACK_DEPLOYED=ON
```

编译成功后，打开config_scripts/pack.bat文件，修改相关路径配置，之后双击执行即可。
