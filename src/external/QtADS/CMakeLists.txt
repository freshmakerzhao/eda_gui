cmake_minimum_required(VERSION 3.5)

if (POLICY CMP0091)
  cmake_policy(SET CMP0091 NEW)
endif (POLICY CMP0091)

set(VERSION_SHORT "4.3.1")  # 手动设置版本号
project(QtAdvancedDockingSystem LANGUAGES CXX VERSION ${VERSION_SHORT})

option(BUILD_STATIC "Build the static library" OFF)

include(GNUInstallDirs)
if (${QT_VERSION_MAJOR})
  message(STATUS "Forced to use Qt version ${QT_VERSION_MAJOR}")
  find_package(QT NAMES Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
else()
  find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
endif()
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)
if (UNIX AND NOT APPLE)
    include_directories(${Qt${QT_VERSION_MAJOR}Gui_PRIVATE_INCLUDE_DIRS})
endif()
set(CMAKE_INCLUDE_CURRENT_DIR ON)
if(BUILD_STATIC)
    set(CMAKE_STATIC_LIBRARY_SUFFIX "_static${CMAKE_STATIC_LIBRARY_SUFFIX}")
endif()
set(ads_SRCS
    ads/ads_globals.cpp
    ads/DockAreaTabBar.cpp
    ads/DockAreaTitleBar.cpp
    ads/DockAreaWidget.cpp
    ads/DockContainerWidget.cpp
    ads/DockManager.cpp
    ads/DockOverlay.cpp
    ads/DockSplitter.cpp
    ads/DockWidget.cpp
    ads/DockWidgetTab.cpp
    ads/DockingStateReader.cpp
    ads/DockFocusController.cpp
    ads/ElidingLabel.cpp
    ads/FloatingDockContainer.cpp
    ads/FloatingDragPreview.cpp
    ads/IconProvider.cpp
    ads/DockComponentsFactory.cpp
    ads/AutoHideSideBar.cpp
    ads/AutoHideTab.cpp
    ads/AutoHideDockContainer.cpp
    ads/PushButton.cpp
    ads/ResizeHandle.cpp
    ads/ads.qrc
)
set(ads_HEADERS
    ads/ads_globals.h
    ads/DockAreaTabBar.h
    ads/DockAreaTitleBar.h
    ads/DockAreaTitleBar_p.h
    ads/DockAreaWidget.h
    ads/DockContainerWidget.h
    ads/DockManager.h
    ads/DockOverlay.h
    ads/DockSplitter.h
    ads/DockWidget.h
    ads/DockWidgetTab.h
    ads/DockingStateReader.h
    ads/DockFocusController.h
    ads/ElidingLabel.h
    ads/FloatingDockContainer.h
    ads/FloatingDragPreview.h
    ads/IconProvider.h
    ads/DockComponentsFactory.h
    ads/AutoHideSideBar.h
    ads/AutoHideTab.h
    ads/AutoHideDockContainer.h
    ads/PushButton.h
    ads/ResizeHandle.h
)
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
if (UNIX AND NOT APPLE)
    set(ads_SRCS ads/linux/FloatingWidgetTitleBar.cpp ${ads_SRCS})
    set(ads_HEADERS ads/linux/FloatingWidgetTitleBar.h ${ads_HEADERS})
endif()

include_directories(./ads)

set(library_name "qt${QT_VERSION_MAJOR}advanceddocking")
if(BUILD_STATIC)
    add_library(${library_name} STATIC ${ads_SRCS} ${ads_HEADERS})
    target_compile_definitions( ${library_name} PUBLIC ADS_STATIC)
else()
    add_library( ${library_name} SHARED ${ads_SRCS} ${ads_HEADERS})
    target_compile_definitions( ${library_name} PRIVATE ADS_SHARED_EXPORT)
endif()

add_library(ads::${library_name} ALIAS ${library_name})

target_link_libraries(${library_name} PUBLIC Qt${QT_VERSION_MAJOR}::Core
                                               Qt${QT_VERSION_MAJOR}::Gui
                                               Qt${QT_VERSION_MAJOR}::Widgets)
if (UNIX AND NOT APPLE)
  if (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
      find_package(X11 REQUIRED)
      target_link_libraries(${library_name} PUBLIC X11::xcb)
  else()
    target_link_libraries(${library_name} PUBLIC xcb)
  endif()
endif()
set_target_properties(${library_name} PROPERTIES
    AUTOMOC ON
    AUTORCC ON
    CXX_EXTENSIONS OFF
    VERSION ${VERSION_SHORT}
    EXPORT_NAME ${library_name}
    DEBUG_POSTFIX "d"
    # ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/external/QADS"
    # LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/external/QADS"
    # RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/external/QADS"
)
if(QT_VERSION_MAJOR STREQUAL "5")
    set_target_properties(${library_name} PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON)
elseif(QT_VERSION_MAJOR STREQUAL "6")
    set_target_properties(${library_name} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON)
endif()

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${library_name}ConfigVersion.cmake"
    VERSION ${VERSION_SHORT}
    COMPATIBILITY SameMajorVersion
)
install(FILES ${ads_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${library_name}
    COMPONENT headers
)
install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE"
    "${CMAKE_CURRENT_SOURCE_DIR}/../gnu-lgpl-v2.1.md"
    DESTINATION license/ads
    COMPONENT license
)
install(TARGETS ${library_name}
    EXPORT adsTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${library_name}
)

install(EXPORT adsTargets
    FILE adsTargets.cmake
    NAMESPACE ads::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${library_name}
)
install(FILES qtadvanceddockingConfig.cmake RENAME ${library_name}Config.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${library_name}
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${library_name}ConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${library_name}
)

target_include_directories(${library_name} PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)



