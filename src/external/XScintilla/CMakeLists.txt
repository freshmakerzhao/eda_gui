cmake_minimum_required(VERSION 3.10)

project(PowerEditor)

option(BUILD_STATIC_LIB "Build static library" ON)

# set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(BUILD_STATIC_LIB)
    set(LIBRARY_TYPE STATIC)
else()
    set(LIBRARY_TYPE SHARED)
endif()

# 创建库目标
add_library(${PROJECT_NAME} ${LIBRARY_TYPE})

# 设置头文件搜索路径
include_directories(./scintilla/include ./scintilla/lexlib ./scintilla/src ./src)

# 定义预处理宏
# if(BUILD_STATIC_LIB)
#     target_compile_definitions(${PROJECT_NAME} PRIVATE XSCINTILLA_MAKE_DLL)
# endif()

file(GLOB_RECURSE HEADERS
    ./*.h
)

file(GLOB_RECURSE SOURCES
    ./*.cpp
)

set(QSCINTILLA_MOC_HDR
    ./src/Xsci/xsciglobal.h
    ./src/Xsci/xsciscintilla.h
    ./src/Xsci/xsciscintillabase.h
    ./src/Xsci/xsciabstractapis.h
    ./src/Xsci/xsciapis.h
    ./src/Xsci/xscicommand.h
    ./src/Xsci/xscicommandset.h
    ./src/Xsci/xscidocument.h
    ./src/Xsci/xscilexer.h
    ./src/Xsci/xscilexeravs.h
    ./src/Xsci/xscilexerbash.h
    ./src/Xsci/xscilexerbatch.h
    ./src/Xsci/xscilexercmake.h
    ./src/Xsci/xscilexercoffeescript.h
    ./src/Xsci/xscilexercpp.h
    ./src/Xsci/xscilexercsharp.h
    ./src/Xsci/xscilexercss.h
    ./src/Xsci/xscilexercustom.h
    ./src/Xsci/xscilexerd.h
    ./src/Xsci/xscilexerdiff.h
    ./src/Xsci/xscilexeredifact.h
    ./src/Xsci/xscilexerfortran.h
    ./src/Xsci/xscilexerfortran77.h
    ./src/Xsci/xscilexerhtml.h
    ./src/Xsci/xscilexeridl.h
    ./src/Xsci/xscilexerjava.h
    ./src/Xsci/xscilexerjavascript.h
    ./src/Xsci/xscilexerjson.h
    ./src/Xsci/xscilexerlua.h
    ./src/Xsci/xscilexermakefile.h
    ./src/Xsci/xscilexermarkdown.h
    ./src/Xsci/xscilexermatlab.h
    ./src/Xsci/xscilexeroctave.h
    ./src/Xsci/xscilexerpascal.h
    ./src/Xsci/xscilexerperl.h
    ./src/Xsci/xscilexerpostscript.h
    ./src/Xsci/xscilexerpo.h
    ./src/Xsci/xscilexerpov.h
    ./src/Xsci/xscilexerproperties.h
    ./src/Xsci/xscilexerpython.h
    ./src/Xsci/xscilexerruby.h
    ./src/Xsci/xscilexerspice.h
    ./src/Xsci/xscilexersql.h
    ./src/Xsci/xscilexertcl.h
    ./src/Xsci/xscilexertex.h
    ./src/Xsci/xscilexerverilog.h
    ./src/Xsci/xscilexervhdl.h
    ./src/Xsci/xscilexerxml.h
    ./src/Xsci/xscilexeryaml.h
    ./src/Xsci/xscimacro.h
    ./src/Xsci/xsciprinter.h
    ./src/Xsci/xscistyle.h
    ./src/Xsci/xscistyledtext.h
    ./src/SciClasses.h
    ./src/ScintillaQt.h
    ./src/SciAccessibility.h
)
QT5_WRAP_CPP(QSCINTILLA_WRAP_MOC_HDR ${QSCINTILLA_MOC_HDR})

target_compile_definitions(${PROJECT_NAME} PRIVATE
    SCINTILLA_QT
    SCI_LEXER
    INCLUDE_DEPRECATED_FEATURES
)

if(UNIX AND NOT APPLE)
    set(CMAKE_CXX_STANDARD 11)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC external/XScintilla/src)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES} ${HEADERS} ${QSCINTILLA_MOC_HDR} ${QSCINTILLA_WRAP_MOC_HDR})

find_package(Qt5 COMPONENTS Core Widgets PrintSupport REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Gui Qt5::Core Qt5::Widgets Qt5::PrintSupport)

# macOS 特定的 post-link 设置
# if(APPLE AND NOT BUILD_STATIC_LIB)
#     add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
#         COMMAND install_name_tool -id @rpath/$<TARGET_FILE_NAME:${TARGET_NAME}> $<TARGET_FILE:${TARGET_NAME}>
#     )
# endif()

# 安装目标
# install(TARGETS ${PROJECT_NAME}
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
# )

# 安装头文件
# install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Xsci)

# 安装翻译文件
# install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/qscintilla_*.qm DESTINATION ${CMAKE_INSTALL_DATADIR}/translations)

# 安装额外的数据
# install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../qsci DESTINATION ${CMAKE_INSTALL_PREFIX}/data/qsci)

# 安装 mkspecs/features 文件
# if(QT_VERSION_MAJOR GREATER 4)
#     set(FEATURES_INSTALL_PATH "${CMAKE_INSTALL_DATADIR}/mkspecs/features")
# else()
#     set(FEATURES_INSTALL_PATH "${CMAKE_INSTALL_DATADIR}/mkspecs/features")
# endif()

# if(BUILD_STATIC_LIB)
#     install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/features_staticlib/qscintilla2.prf DESTINATION ${FEATURES_INSTALL_PATH})
# else()
#     install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/features/qscintilla2.prf DESTINATION ${FEATURES_INSTALL_PATH})
# endif()
