
# 写在前面

**该项目中，以下文件禁止上传：**

* `eda_gui/src/config_scripts/config.cmake`
* `eda_gui/src/CMakePresets.json`

# 一、介绍

**这是一个对标vivado的eda工具，能够帮助用户实现综合、布局布线、码流生成等关键步骤。**

**主要技术栈为c++、qt、tcl，使用了以下几种第三方库：**

* **ExScintilla**

  * **语法解析**
* **QtADS**

  * **拖拽式布局**
* **VCDVisionLib**

  * **波形绘制**
* **yaml-cpp**

  * **yaml解析**
* **compress_tool**

  * **基于7z的加解密库**
  * [http://192.168.11.164:30000/eda/compress_tool.git](http://192.168.11.164:30000/eda/compress_tool.git)

**其中，除了compress_tool之外，其他库均为源码嵌入。**

# 二、项目配置

## 2.1 打开项目

* **拉取项目**
  * git clone [git@github.com](mailto:git@github.com):freshmakerzhao/eda_gui.git
* **初始化子模块**
  * **cd eda_gui**
  * **git submodule update --init --recursive**
* **追踪子仓库最新提交**
  * **git submodule update --remote --merge**
* **启动visual studio 2022**
* **如果在欢迎页，点击Continue without code，进入空项目页**
* **File -> open.. -> CMake..**
* **选择eda_gui/src/CMakeLists.txt**

## 2.2 配置

**项目编译前，需要安装vcpkg，参考教程：**

[https://www.cnblogs.com/quantoublog/articles/18820438](https://www.cnblogs.com/quantoublog/articles/18820438)

**之后安装必要的库：**

* **boost**
  * **vcpkg install boost**
* **qt5**
  * **vcpkg install qt5**
* **7zip**
  * **vcpkg install 7zip**
* **bit7z**
  * **vcpkg install bit7z**

**完成后，修改** `eda_gui/src/config_scripts/config.cmake`

* **VCPKG_PATH**
  * **vcpkg安装目录**
* **RESOURCE_WIN_PATH**
  * **resource文件路径**
* **FONTS_PATH**
  * **字体路径**

## 2.3 编译

**编译命令已在** `eda_gui/src/CMakePresets.json`中留有模板

* **x64 Debug**
  * **debug模式**
  * **不开启mac地址校验**
  * **不开启日期校验**
  * **不进行打包配置**
* **x64 Release**
  * **release模式**
  * **不开启mac地址校验**
  * **不开启日期校验**
  * **不进行打包配置**
* **package**
  * **release模式**
  * **开启mac地址校验**
  * **开启日期校验**
  * **进行打包配置**

**项目自定义编译选项如下：**

* **ENABLE_MAC_CHECK**
  * **是否启用MAC地址校验**
  * **可选值：**
    * **"OFF"，关闭**
    * **"ON"，开启**
  * **默认 : "OFF"**
* **ENABLE_EXPIRATION_CHECK**
  * **是否启用日期校验**
  * **可选值：**
    * **"OFF"，关闭**
    * **"ON"，开启**
  * **默认 : "OFF"**
* **PACK_DEPLOYED**
  * **是否启用打包模式**
  * **可选值：**
    * **"OFF"，关闭**
    * **"ON"，开启**
  * **默认 : "OFF"**

**如果仅编译Gridview，编译时添加该选项**

```
-DONLY_COMPILE_GRIDVIEW=ON
```

## 2.4 打包

**打包发布，编译时添加该选项（参考CMakePresets中的package），主程序只在同级目录下寻找resource_win、fonts等文件，并会覆盖config.cmake中这几项的配置（config.cmake不生效）。**

```
-DPACK_DEPLOYED=ON
```

**编译成功后，打开** `eda_gui/src/config_scripts/pack.bat`文件，修改相关路径配置，之后双击执行即可。

```
SET BUILD_DIR=E:\workspace\eda_gui_msvc\src\out\build\x64-release
SET RESOURCE_DIR=E:\workspace\eda_gui_resource\resource_win
SET FONTS_DIR=E:\workspace\eda_gui_resource\Fonts
SET VCPKG_PATH=E:\workspace\vcpkg
```
