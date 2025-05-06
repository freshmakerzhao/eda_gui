@echo off
SETLOCAL ENABLEEXTENSIONS

REM 设置项目的路径和资源路径
SET BUILD_DIR=E:\workspace\eda_gui_msvc\src\out\build\x64-release
SET RESOURCE_DIR=E:\workspace\eda_gui_resource\resource_win
SET FONTS_DIR=E:\workspace\eda_gui_resource\Fonts
SET VCPKG_PATH=E:\workspace\vcpkg

REM 目标文件夹（打包后存放文件的地方）
SET TARGET_DIR=%BUILD_DIR%\release_package

REM 如果目标目录存在，先删除
IF EXIST %TARGET_DIR% (
    echo Deleting existing target directory...
    rmdir /S /Q %TARGET_DIR%
)

REM 创建目标文件夹
mkdir %TARGET_DIR%

REM 拷贝 Qt 的 DLL 文件
echo Copying Qt DLLs...
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\Qt5Core.dll" %TARGET_DIR%\
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\Qt5Gui.dll" %TARGET_DIR%\
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\Qt5Widgets.dll" %TARGET_DIR%\
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\Qt5Network.dll" %TARGET_DIR%\
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\Qt5Xml.dll" %TARGET_DIR%\
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\Qt5PrintSupport.dll" %TARGET_DIR%\
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\Qt5OpenGL.dll" %TARGET_DIR%\
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\Qt5Svg.dll" %TARGET_DIR%\

REM 拷贝项目的其他 DLL 文件
echo Copying other DLLs...
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\freetype.dll" %TARGET_DIR%\
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\jpeg62.dll" %TARGET_DIR%\
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\libpng16.dll" %TARGET_DIR%\
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\bz2.dll" %TARGET_DIR%\
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\libpng16.dll" %TARGET_DIR%\
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\pcre2-16.dll" %TARGET_DIR%\
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\tiff.dll" %TARGET_DIR%\
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\zstd.dll" %TARGET_DIR%\
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\libsharpyuv.dll" %TARGET_DIR%\
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\libssl-3-x64.dll" %TARGET_DIR%\
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\libwebp.dll" %TARGET_DIR%\
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\libwebpdemux.dll" %TARGET_DIR%\
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\libwebpmux.dll" %TARGET_DIR%\
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\double-conversion.dll" %TARGET_DIR%\
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\jpeg62.dll" %TARGET_DIR%\
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\libcrypto-3-x64.dll" %TARGET_DIR%\
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\harfbuzz.dll" %TARGET_DIR%\
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\brotlicommon.dll" %TARGET_DIR%\
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\brotlidec.dll" %TARGET_DIR%\
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\zlib1.dll" %TARGET_DIR%\
COPY /Y "%VCPKG_PATH%\installed\x64-windows\bin\7zip.dll" %TARGET_DIR%\
COPY /Y "%BUILD_DIR%\tcl86.dll" %TARGET_DIR%\
COPY /Y "%BUILD_DIR%\libcrypto-1_1-x64.dll" %TARGET_DIR%\
COPY /Y "%BUILD_DIR%\qt5advanceddocking.dll" %TARGET_DIR%\


REM 拷贝项目的其他 DLL 文件
echo Copying conf file...
COPY /Y "%BUILD_DIR%\qt.conf" %TARGET_DIR%\

REM 拷贝项目的资源文件
echo Copying resource files...
xcopy /E /I /Y "%RESOURCE_DIR%" %TARGET_DIR%\resource_win

REM 拷贝项目的字体文件
echo Copying fonts files...
xcopy /E /I /Y "%FONTS_DIR%" %TARGET_DIR%\Fonts

REM 拷贝 Qt 相关的插件文件夹
echo Copying Qt plugins folders...
xcopy /E /I /Y "%BUILD_DIR%\platforms" %TARGET_DIR%\platforms
xcopy /E /I /Y "%BUILD_DIR%\styles" %TARGET_DIR%\styles
xcopy /E /I /Y "%BUILD_DIR%\imageformats" %TARGET_DIR%\imageformats
xcopy /E /I /Y "%BUILD_DIR%\iconengines" %TARGET_DIR%\iconengines
xcopy /E /I /Y "%BUILD_DIR%\lib" %TARGET_DIR%\lib

REM 拷贝项目的可执行文件
echo Copying executable file...
COPY /Y "%BUILD_DIR%\HybrdLink.exe" %TARGET_DIR%\

REM 完成
echo Packaging completed. All required files are copied to: %TARGET_DIR%

ENDLOCAL
pause
