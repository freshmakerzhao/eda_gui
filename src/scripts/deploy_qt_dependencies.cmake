function(my_deploy_dependencies target_name)
    message(STATUS "📦 Deploying DLLs for target: ${target_name}")

    
    set(DEBUG_SUFFIX "")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(VCPKG_ROOT "${VCPKG_PATH}/installed/x64-windows/debug")
        set(DLLS_MAP
            "bin/Qt5Cored.dll | Qt5Cored.dll"
            "bin/Qt5Guid.dll | Qt5Guid.dll"
            "bin/Qt5Widgetsd.dll | Qt5Widgetsd.dll"
            "bin/Qt5Networkd.dll | Qt5Networkd.dll"
            "bin/Qt5Xmld.dll | Qt5Xmld.dll"
            "bin/Qt5PrintSupportd.dll | Qt5PrintSupportd.dll"
            "bin/Qt5OpenGLd.dll | Qt5OpenGLd.dll"
            "bin/Qt5Svgd.dll | Qt5Svgd.dll"
            "bin/freetyped.dll | freetyped.dll"
            "bin/jpeg62.dll | jpeg62.dll"
            "bin/libpng16d.dll | libpng16d.dll"
            "bin/bz2d.dll | bz2d.dll"
            "bin/libpng16d.dll | libpng16d.dll"
            "bin/pcre2-16d.dll | pcre2-16d.dll"
            "bin/tiffd.dll | tiffd.dll"
            "bin/zstd.dll | zstd.dll"
            "bin/libsharpyuv.dll | libsharpyuv.dll"
            "bin/libssl-3-x64.dll | libssl-3-x64.dll"
            "bin/libwebp.dll | libwebp.dll"
            "bin/libwebpdemux.dll | libwebpdemux.dll"
            "bin/libwebpmux.dll | libwebpmux.dll"
            "bin/double-conversion.dll | double-conversion.dll"
            "bin/jpeg62.dll | jpeg62.dll"
            "bin/libcrypto-3-x64.dll | libcrypto-3-x64.dll"
            "bin/harfbuzz.dll | harfbuzz.dll"
            "bin/brotlicommon.dll | brotlicommon.dll"
            "bin/brotlidec.dll | brotlidec.dll"
            "bin/zlibd1.dll | zlibd1.dll"
            "plugins/platforms/qwindowsd.dll | platforms/qwindowsd.dll"
            "plugins/styles/qwindowsvistastyled.dll | styles/qwindowsvistastyled.dll"
            "plugins/imageformats/qgifd.dll | imageformats/qgifd.dll"
            "plugins/imageformats/qicnsd.dll | imageformats/qicnsd.dll"
            "plugins/imageformats/qicod.dll | imageformats/qicod.dll"
            "plugins/imageformats/qjpegd.dll | imageformats/qjpegd.dll"
            "plugins/imageformats/qsvgd.dll | imageformats/qsvgd.dll"
            "plugins/imageformats/qtgad.dll | imageformats/qtgad.dll"
            "plugins/imageformats/qtiffd.dll | imageformats/qtiffd.dll"
            "plugins/imageformats/qwbmpd.dll | imageformats/qwbmpd.dll"
            "plugins/imageformats/qwebpd.dll | imageformats/qwebpd.dll"
            "plugins/iconengines/qsvgicond.dll | iconengines/qsvgicond.dll"
        )
    else()
        set(VCPKG_ROOT "${VCPKG_PATH}/installed/x64-windows")
        set(DLLS_MAP
            "bin/Qt5Core.dll | Qt5Core.dll"
            "bin/Qt5Gui.dll | Qt5Gui.dll"
            "bin/Qt5Widgets.dll | Qt5Widgets.dll"
            "bin/Qt5Network.dll | Qt5Network.dll"
            "bin/Qt5Xml.dll | Qt5Xml.dll"
            "bin/Qt5PrintSupport.dll | Qt5PrintSupport.dll" 
            "bin/Qt5OpenGL.dll | Qt5OpenGL.dll"
            "bin/Qt5Svg.dll | Qt5Svg.dll"
            "bin/freetype.dll | freetype.dll"
            "bin/jpeg62.dll | jpeg62.dll"
            "bin/libpng16.dll | libpng16.dll"
            "bin/bz2.dll | bz2.dll"
            "bin/libpng16.dll | libpng16.dll"
            "bin/pcre2-16.dll | pcre2-16.dll"
            "bin/tiff.dll | tiff.dll"
            "bin/zst.dll | zst.dll"
            "bin/libsharpyuv.dll | libsharpyuv.dll"
            "bin/libssl-3-x64.dll | libssl-3-x64.dll"
            "bin/libwebp.dll | libwebp.dll"
            "bin/libwebpdemux.dll | libwebpdemux.dll"
            "bin/libwebpmux.dll | libwebpmux.dll"
            "bin/double-conversion.dll | double-conversion.dll"
            "bin/jpeg62.dll | jpeg62.dll"
            "bin/libcrypto-3-x64.dll | libcrypto-3-x64.dll"
            "bin/harfbuzz.dll | harfbuzz.dll"
            "bin/brotlicommon.dll | brotlicommon.dll"
            "bin/brotlidec.dll | brotlidec.dll"
            "bin/zlibd1.dll | zlibd1.dll"
            "plugins/platforms/qwindows.dll | platforms/qwindows.dll"
            "plugins/styles/qwindowsvistastyle.dll | styles/qwindowsvistastyle.dll"
            "plugins/imageformats/qgif.dll | imageformats/qgif.dll"
            "plugins/imageformats/qicns.dll | imageformats/qicns.dll"
            "plugins/imageformats/qico.dll | imageformats/qico.dll"
            "plugins/imageformats/qjpeg.dll | imageformats/qjpeg.dll"
            "plugins/imageformats/qsvg.dll | imageformats/qsvg.dll"
            "plugins/imageformats/qtga.dll | imageformats/qtga.dll"
            "plugins/imageformats/qtiff.dll | imageformats/qtiff.dll"
            "plugins/imageformats/qwbmp.dll | imageformats/qwbmp.dll"
            "plugins/imageformats/qwebp.dll | imageformats/qwebp.dll"
            "plugins/iconengines/qsvgicon.dll | iconengines/qsvgicon.dll"
        )
    endif()

    set(TARGET_DIR "$<TARGET_FILE_DIR:${target_name}>")

    function(copy_file_map entry)
        # 做成 parts 列表
        string(REPLACE " | " ";" parts "${entry}")
        list(GET parts 0 rel_src)
        list(GET parts 1 rel_dst)

        # 拼接src和dst
        set(src "${VCPKG_ROOT}/${rel_src}")
        set(dst "${TARGET_DIR}/${rel_dst}")
        # 从路径中提取出目录部分 imageformats/qjpeg.dll --> imageformats/
        get_filename_component(dst_dir "${dst}" DIRECTORY)

        if(NOT dst_dir STREQUAL "")
            add_custom_command(TARGET ${target_name} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory "${dst_dir}"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${src}" "${dst}"
                COMMENT "📥 Copying ${src} → ${dst}"
                VERBATIM
            )
        else()
            add_custom_command(TARGET ${target_name} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${src}" "${dst}"
                COMMENT "📥 Copying ${src} → ${dst}"
                VERBATIM
            )
        endif()
    
    endfunction()
    
    # === 批量拷贝 ===
    foreach(entry IN LISTS DLLS_MAP)
        copy_file_map("${entry}")
    endforeach()
    
endfunction()