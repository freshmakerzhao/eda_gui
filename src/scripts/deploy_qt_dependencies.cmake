function(my_deploy_dependencies target_name)
    message(STATUS "📦 Deploying Qt DLLs for target: ${target_name}")
    # 方便拼接debug和release版本的路径
    set(VCPKG_ROOT "${VCPKG_PATH}/installed/x64-windows")
    # debug后缀，如果是debug版本，则在dll上增加d
    set(DEBUG_SUFFIX "")

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(BIN_DIR "${VCPKG_ROOT}/debug/bin")
        set(PLUGIN_DIR "${VCPKG_ROOT}/debug/plugins")
        set(DEBUG_SUFFIX "d")
        set(ZLIB_DLL_NAME "zlibd1.dll") 
    else()
        set(BIN_DIR "${VCPKG_ROOT}/bin")
        set(PLUGIN_DIR "${VCPKG_ROOT}/plugins")
        set(ZLIB_DLL_NAME "zlib1.dll")
    endif()


    # 拷贝至 TARGET_DIR 中，target_name 就是 PROJECT_NAME
    set(TARGET_DIR "$<TARGET_FILE_DIR:${target_name}>")

    # 拷贝 Qt DLL 函数
    function(copy_qt_dll libname)
        add_custom_command(TARGET ${target_name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${BIN_DIR}/${libname}${DEBUG_SUFFIX}.dll"
                "${TARGET_DIR}"
        )
    endfunction()

    # 拷贝 Qt 插件（platforms/）
    function(copy_qt_plugin subdir libname)
        add_custom_command(TARGET ${target_name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${TARGET_DIR}/${subdir}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${PLUGIN_DIR}/${subdir}/${libname}${DEBUG_SUFFIX}.dll"
                "${TARGET_DIR}/${subdir}"
        )
    endfunction()

    # === Qt 主 DLL ===
    copy_qt_dll(Qt5Core)
    copy_qt_dll(Qt5Gui)
    copy_qt_dll(Qt5Widgets)
    copy_qt_dll(Qt5Network)
    copy_qt_dll(Qt5Xml)
    copy_qt_dll(Qt5PrintSupport)
    copy_qt_dll(Qt5OpenGL)

    # === 插件 ===
    copy_qt_plugin("platforms" "qwindows")
    
    # === ZLIB ===
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${BIN_DIR}/${ZLIB_DLL_NAME}"
            "$<TARGET_FILE_DIR:${target_name}>"
        COMMENT "Copying ${ZLIB_DLL_NAME} from vcpkg to target directory"
    )
endfunction()
