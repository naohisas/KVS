cmake_minimum_required(VERSION 3.16)

include(GNUInstallDirs)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)

find_package(PkgConfig QUIET)

set(KVS_VERSION ${PROJECT_VERSION})
set(serial ${PROJECT_VERSION})
set(soserial 0)
set(KVS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

function(kvs_filter_generated_sources out_var)
    set(_filtered)
    foreach(_file IN LISTS ARGN)
        if(NOT _file MATCHES "/(Debug|Release|CMakeFiles)/")
            list(APPEND _filtered "${_file}")
        endif()
    endforeach()
    set(${out_var} ${_filtered} PARENT_SCOPE)
endfunction()

function(kvs_collect_sources out_var relative_dir)
    file(GLOB_RECURSE _sources CONFIGURE_DEPENDS "${KVS_SOURCE_DIR}/${relative_dir}/*.cpp")
    kvs_filter_generated_sources(_filtered ${_sources})
    set(${out_var} ${_filtered} PARENT_SCOPE)
endfunction()

function(kvs_collect_headers out_var relative_dir)
    file(GLOB_RECURSE _headers CONFIGURE_DEPENDS "${KVS_SOURCE_DIR}/${relative_dir}/*.h")
    kvs_filter_generated_sources(_filtered ${_headers})
    set(${out_var} ${_filtered} PARENT_SCOPE)
endfunction()

function(kvs_target_link_directories_if_any target visibility)
    set(_directories ${ARGN})
    if(_directories)
        target_link_directories(${target} ${visibility} ${_directories})
    endif()
endfunction()

function(kvs_target_common target)
    set_target_properties(${target} PROPERTIES
        VERSION ${serial}
        SOVERSION ${soserial}
        POSITION_INDEPENDENT_CODE ON
    )

    target_include_directories(${target}
        PUBLIC
            $<BUILD_INTERFACE:${KVS_SOURCE_DIR}>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

    kvs_apply_makefile_compile_options(${target})

    if(KVS_SUPPORT_PYTHON)
        target_compile_definitions(${target} PUBLIC KVS_PYTHON_VERSION=${KVS_PYTHON_VERSION_VALUE})
    endif()
    if(KVS_ENABLE_OPENMP)
        target_include_directories(${target} PUBLIC ${KVS_OPENMP_INCLUDE_DIRS})
        kvs_target_link_directories_if_any(${target} PUBLIC ${KVS_OPENMP_LIBRARY_DIRS})
        if(KVS_OPENMP_LIBRARIES)
            target_link_libraries(${target} PUBLIC ${KVS_OPENMP_LIBRARIES})
        else()
            target_link_libraries(${target} PUBLIC OpenMP::OpenMP_CXX)
        endif()
    endif()

    if(KVS_ENABLE_GLEW)
        target_include_directories(${target} PUBLIC ${KVS_GLEW_INCLUDE_DIRS})
        kvs_target_link_directories_if_any(${target} PUBLIC ${KVS_GLEW_LIBRARY_DIRS})
        target_link_libraries(${target} PUBLIC ${KVS_GLEW_LIBRARIES})
    endif()
endfunction()

function(kvs_install_library target wrapper relative_dir kvs_subdir)
    set(_export_args)
    if(KVS_INSTALL_CMAKE_PACKAGE)
        list(APPEND _export_args EXPORT kvsTargets)
    endif()

    install(
        TARGETS
            ${target}
        ${_export_args}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    if(EXISTS "${KVS_SOURCE_DIR}/${wrapper}")
        install(
            FILES "${KVS_SOURCE_DIR}/${wrapper}"
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )
    endif()

    kvs_install_header_tree("${relative_dir}")

    if(kvs_subdir)
        kvs_install_kvs_wrapper_files("${kvs_subdir}")
    endif()
endfunction()

function(kvs_install_header_tree relative_dir)
    install(
        DIRECTORY "${KVS_SOURCE_DIR}/${relative_dir}"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING
            PATTERN "*.h"
            PATTERN "Debug" EXCLUDE
            PATTERN "Release" EXCLUDE
            PATTERN "CMakeFiles" EXCLUDE
    )
endfunction()

function(kvs_install_files_in_directory source_dir destination)
    file(GLOB _files CONFIGURE_DEPENDS "${source_dir}/*")
    foreach(_file IN LISTS _files)
        if(NOT IS_DIRECTORY "${_file}")
            install(FILES "${_file}" DESTINATION "${destination}")
        endif()
    endforeach()
endfunction()

function(kvs_install_kvs_wrapper_files subdir)
    if(subdir)
        kvs_install_files_in_directory("${KVS_SOURCE_DIR}/kvs/${subdir}" "${CMAKE_INSTALL_INCLUDEDIR}/kvs/${subdir}")
    else()
        kvs_install_files_in_directory("${KVS_SOURCE_DIR}/kvs" "${CMAKE_INSTALL_INCLUDEDIR}/kvs")
    endif()
endfunction()

function(kvs_list_contains_target out_var target_name)
    set(_found FALSE)
    foreach(_item IN LISTS ARGN)
        if(_item STREQUAL "${target_name}")
            set(_found TRUE)
        endif()
    endforeach()
    set(${out_var} ${_found} PARENT_SCOPE)
endfunction()

function(kvs_install_component_package package_name component_name)
    set(KVS_COMPAT_PACKAGE_NAME "${package_name}")
    set(KVS_COMPAT_COMPONENT_NAME "${component_name}")
    set(_package_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${package_name}")

    configure_package_config_file(
        "${KVS_SOURCE_DIR}/cmake/kvsComponentConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${package_name}/${package_name}Config.cmake"
        INSTALL_DESTINATION "${_package_dir}"
    )
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${package_name}/${package_name}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )
    install(
        FILES
            "${CMAKE_CURRENT_BINARY_DIR}/${package_name}/${package_name}Config.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/${package_name}/${package_name}ConfigVersion.cmake"
        DESTINATION "${_package_dir}"
    )
endfunction()

function(kvs_install_cmake_package)
    if(NOT KVS_INSTALL_CMAKE_PACKAGE)
        return()
    endif()

    set(_package_dir "${CMAKE_INSTALL_LIBDIR}/cmake/kvs")

    kvs_list_contains_target(KVS_CONFIG_NEEDS_OPENGL OpenGL::GL ${KVS_GL_LIBRARIES})
    kvs_list_contains_target(KVS_CONFIG_NEEDS_OPENGL_GLU OpenGL::GLU ${KVS_GL_LIBRARIES})
    kvs_list_contains_target(KVS_CONFIG_NEEDS_GLEW GLEW::GLEW ${KVS_GLEW_LIBRARIES})
    kvs_list_contains_target(KVS_CONFIG_NEEDS_GLUT GLUT::GLUT ${KVS_GLUT_LIBRARIES})
    kvs_list_contains_target(KVS_CONFIG_NEEDS_GLFW glfw ${KVS_GLFW_LIBRARIES})
    kvs_list_contains_target(_kvs_needs_glfw3 glfw3 ${KVS_GLFW_LIBRARIES})
    if(_kvs_needs_glfw3)
        set(KVS_CONFIG_NEEDS_GLFW TRUE)
    endif()
    kvs_list_contains_target(KVS_CONFIG_FFMPEG_USES_PKGCONFIG PkgConfig::KVS_FFMPEG ${KVS_FFMPEG_LIBRARIES})
    kvs_list_contains_target(KVS_CONFIG_OPENCV_USES_PKGCONFIG PkgConfig::KVS_OPENCV ${KVS_OPENCV_LIBRARIES})

    set(KVS_CONFIG_OPENMP_USES_TARGET FALSE)
    if(KVS_ENABLE_OPENMP AND NOT KVS_OPENMP_LIBRARIES)
        set(KVS_CONFIG_OPENMP_USES_TARGET TRUE)
    endif()

    configure_package_config_file(
        "${KVS_SOURCE_DIR}/cmake/kvsConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/kvs/kvsConfig.cmake"
        INSTALL_DESTINATION "${_package_dir}"
    )
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/kvs/kvsConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )
    install(
        EXPORT kvsTargets
        FILE kvsTargets.cmake
        NAMESPACE kvs::
        DESTINATION "${_package_dir}"
    )
    install(
        FILES
            "${CMAKE_CURRENT_BINARY_DIR}/kvs/kvsConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/kvs/kvsConfigVersion.cmake"
        DESTINATION "${_package_dir}"
    )

    kvs_install_component_package(kvsCore Core)
    if(TARGET kvsSupportGLUT)
        kvs_install_component_package(kvsSupportGLUT SupportGLUT)
    endif()
    if(TARGET kvsSupportGLFW)
        kvs_install_component_package(kvsSupportGLFW SupportGLFW)
    endif()
    if(TARGET kvsSupportFFmpeg)
        kvs_install_component_package(kvsSupportFFmpeg SupportFFmpeg)
    endif()
    if(TARGET kvsSupportQt)
        kvs_install_component_package(kvsSupportQt SupportQt)
    endif()
    if(TARGET kvsSupportOpenCV)
        kvs_install_component_package(kvsSupportOpenCV SupportOpenCV)
    endif()
    if(TARGET kvsSupportCUDA)
        kvs_install_component_package(kvsSupportCUDA SupportCUDA)
    endif()
    if(TARGET kvsSupportPython)
        kvs_install_component_package(kvsSupportPython SupportPython)
    endif()
    if(TARGET kvsSupportMPI)
        kvs_install_component_package(kvsSupportMPI SupportMPI)
    endif()
    if(TARGET kvsSupportEGL)
        kvs_install_component_package(kvsSupportEGL SupportEGL)
    endif()
    if(TARGET kvsSupportOSMesa)
        kvs_install_component_package(kvsSupportOSMesa SupportOSMesa)
    endif()
endfunction()

set(KVS_OPENMP_INCLUDE_DIRS)
set(KVS_OPENMP_LIBRARY_DIRS)
set(KVS_OPENMP_LIBRARIES)
if(KVS_ENABLE_OPENMP)
    kvs_make_include_paths(KVS_OPENMP_INCLUDE_DIRS OPENMP)
    kvs_make_library_paths(KVS_OPENMP_LIBRARY_DIRS OPENMP)
    kvs_make_link_libraries(KVS_OPENMP_LIBRARIES OPENMP)
endif()

set(KVS_GL_INCLUDE_DIRS)
set(KVS_GL_LIBRARY_DIRS)
set(KVS_GL_LIBRARIES)

if(KVS_ENABLE_OPENGL)
    kvs_make_include_paths(KVS_GL_HINT_INCLUDE_DIRS GL)
    kvs_make_library_paths(KVS_GL_HINT_LIBRARY_DIRS GL)
    kvs_make_link_libraries(KVS_GL_ENV_LIBRARIES GL)
    kvs_make_link_libraries(KVS_GLU_ENV_LIBRARIES GLU)

    if(KVS_SUPPORT_OSMESA)
        kvs_make_include_paths(KVS_OSMESA_HINT_INCLUDE_DIRS OSMESA)
        kvs_make_library_paths(KVS_OSMESA_HINT_LIBRARY_DIRS OSMESA)
        kvs_make_link_libraries(KVS_OSMESA_ENV_LIBRARIES OSMESA)
        find_path(KVS_OSMESA_INCLUDE_DIR NAMES GL/osmesa.h HINTS ${KVS_OSMESA_HINT_INCLUDE_DIRS})
        find_library(KVS_OSMESA_LIBRARY NAMES OSMesa OSMESA HINTS ${KVS_OSMESA_HINT_LIBRARY_DIRS})
        if(NOT KVS_OSMESA_INCLUDE_DIR)
            message(FATAL_ERROR "KVS_SUPPORT_OSMESA=ON requires OSMesa headers and library.")
        endif()
        list(APPEND KVS_GL_INCLUDE_DIRS ${KVS_OSMESA_INCLUDE_DIR})
        list(APPEND KVS_GL_LIBRARY_DIRS ${KVS_OSMESA_HINT_LIBRARY_DIRS})
        if(KVS_OSMESA_ENV_LIBRARIES)
            list(APPEND KVS_GL_LIBRARIES ${KVS_OSMESA_ENV_LIBRARIES})
        elseif(KVS_OSMESA_LIBRARY)
            list(APPEND KVS_GL_LIBRARIES ${KVS_OSMESA_LIBRARY})
        else()
            message(FATAL_ERROR "KVS_SUPPORT_OSMESA=ON requires OSMesa headers and library.")
        endif()
    else()
        find_package(OpenGL REQUIRED)
        list(APPEND KVS_GL_INCLUDE_DIRS ${KVS_GL_HINT_INCLUDE_DIRS})
        list(APPEND KVS_GL_LIBRARY_DIRS ${KVS_GL_HINT_LIBRARY_DIRS})
        set(_kvs_gl_base_libraries)
        if(KVS_GL_ENV_LIBRARIES)
            list(APPEND _kvs_gl_base_libraries ${KVS_GL_ENV_LIBRARIES})
        elseif(TARGET OpenGL::GL)
            list(APPEND _kvs_gl_base_libraries OpenGL::GL)
        else()
            list(APPEND _kvs_gl_base_libraries ${OPENGL_LIBRARIES})
        endif()
        if(OPENGL_INCLUDE_DIR)
            list(APPEND KVS_GL_INCLUDE_DIRS ${OPENGL_INCLUDE_DIR})
        endif()

        if(KVS_ENABLE_GLU)
            if(KVS_GLU_ENV_LIBRARIES)
                list(APPEND KVS_GL_LIBRARIES ${KVS_GLU_ENV_LIBRARIES})
            elseif(TARGET OpenGL::GLU)
                list(APPEND KVS_GL_LIBRARIES OpenGL::GLU)
            elseif(OPENGL_glu_LIBRARY)
                list(APPEND KVS_GL_LIBRARIES ${OPENGL_glu_LIBRARY})
            else()
                find_library(KVS_GLU_LIBRARY NAMES GLU glu)
                if(KVS_GLU_LIBRARY)
                    list(APPEND KVS_GL_LIBRARIES ${KVS_GLU_LIBRARY})
                elseif(NOT APPLE)
                    message(FATAL_ERROR "KVS_ENABLE_GLU=ON but GLU was not found.")
                endif()
            endif()
        endif()
        list(APPEND KVS_GL_LIBRARIES ${_kvs_gl_base_libraries})
    endif()
endif()

set(KVS_GLEW_INCLUDE_DIRS)
set(KVS_GLEW_LIBRARY_DIRS)
set(KVS_GLEW_LIBRARIES)
if(KVS_ENABLE_OPENGL AND KVS_ENABLE_GLEW)
    kvs_make_include_paths(KVS_GLEW_HINT_INCLUDE_DIRS GLEW)
    kvs_make_library_paths(KVS_GLEW_HINT_LIBRARY_DIRS GLEW)
    kvs_make_link_libraries(KVS_GLEW_ENV_LIBRARIES GLEW)
    if(KVS_GLEW_ENV_LIBRARIES)
        set(KVS_GLEW_INCLUDE_DIRS ${KVS_GLEW_HINT_INCLUDE_DIRS})
        set(KVS_GLEW_LIBRARY_DIRS ${KVS_GLEW_HINT_LIBRARY_DIRS})
        set(KVS_GLEW_LIBRARIES ${KVS_GLEW_ENV_LIBRARIES})
    else()
        find_package(GLEW REQUIRED)
    endif()
    if(TARGET GLEW::GLEW)
        set(KVS_GLEW_LIBRARIES GLEW::GLEW)
    elseif(NOT KVS_GLEW_LIBRARIES)
        set(KVS_GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIRS})
        set(KVS_GLEW_LIBRARIES ${GLEW_LIBRARIES})
    endif()
endif()

if(KVS_ENABLE_OPENMP)
    find_package(OpenMP REQUIRED COMPONENTS CXX)
endif()

if(KVS_SUPPORT_PYTHON)
    find_package(Python3 REQUIRED COMPONENTS Interpreter Development NumPy)
    set(KVS_PYTHON_VERSION_VALUE ${Python3_VERSION_MAJOR})
endif()

#
# kvsCore
#
kvs_collect_sources(kvsCore_SOURCES Core)
add_library(kvsCore STATIC ${kvsCore_SOURCES})
generate_export_header(kvsCore)
kvs_target_common(kvsCore)
target_include_directories(kvsCore PUBLIC ${KVS_GL_INCLUDE_DIRS})
kvs_target_link_directories_if_any(kvsCore PUBLIC ${KVS_GL_LIBRARY_DIRS})
target_link_libraries(kvsCore PUBLIC ${KVS_GL_LIBRARIES})
kvs_install_library(kvsCore kvsCore Core "")
kvs_install_kvs_wrapper_files("")

#
# SupportGLUT
#
if(KVS_ENABLE_OPENGL AND KVS_SUPPORT_GLUT)
    kvs_make_include_paths(KVS_GLUT_HINT_INCLUDE_DIRS GLUT)
    kvs_make_library_paths(KVS_GLUT_HINT_LIBRARY_DIRS GLUT)
    kvs_make_link_libraries(KVS_GLUT_ENV_LIBRARIES GLUT)
    find_package(GLUT REQUIRED)
    kvs_collect_sources(kvsSupportGLUT_SOURCES SupportGLUT)
    add_library(kvsSupportGLUT STATIC ${kvsSupportGLUT_SOURCES})
    generate_export_header(kvsSupportGLUT)
    kvs_target_common(kvsSupportGLUT)
    target_include_directories(kvsSupportGLUT PUBLIC ${KVS_GL_INCLUDE_DIRS} ${KVS_GLUT_HINT_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
    kvs_target_link_directories_if_any(kvsSupportGLUT PUBLIC ${KVS_GL_LIBRARY_DIRS} ${KVS_GLUT_HINT_LIBRARY_DIRS})
    if(KVS_GLUT_ENV_LIBRARIES)
        set(KVS_GLUT_LIBRARIES ${KVS_GLUT_ENV_LIBRARIES})
    elseif(TARGET GLUT::GLUT)
        set(KVS_GLUT_LIBRARIES GLUT::GLUT)
    else()
        set(KVS_GLUT_LIBRARIES ${GLUT_LIBRARIES})
    endif()
    target_link_libraries(kvsSupportGLUT PUBLIC kvsCore ${KVS_GLUT_LIBRARIES} ${KVS_GL_LIBRARIES})
    kvs_install_library(kvsSupportGLUT kvsSupportGLUT SupportGLUT glut)
endif()

#
# SupportGLFW
#
if(KVS_ENABLE_OPENGL AND KVS_SUPPORT_GLFW)
    kvs_make_include_paths(KVS_GLFW_HINT_INCLUDE_DIRS GLFW)
    kvs_make_library_paths(KVS_GLFW_HINT_LIBRARY_DIRS GLFW)
    kvs_make_link_libraries(KVS_GLFW_ENV_LIBRARIES GLFW)
    find_package(glfw3 QUIET)
    if(KVS_GLFW_ENV_LIBRARIES)
        find_path(KVS_GLFW_INCLUDE_DIR NAMES GLFW/glfw3.h HINTS ${KVS_GLFW_HINT_INCLUDE_DIRS})
        if(NOT KVS_GLFW_INCLUDE_DIR AND NOT KVS_GLFW_HINT_INCLUDE_DIRS)
            message(FATAL_ERROR "KVS_SUPPORT_GLFW=ON requires GLFW.")
        endif()
        set(KVS_GLFW_INCLUDE_DIRS ${KVS_GLFW_HINT_INCLUDE_DIRS} ${KVS_GLFW_INCLUDE_DIR})
        set(KVS_GLFW_LIBRARIES ${KVS_GLFW_ENV_LIBRARIES})
    elseif(TARGET glfw)
        set(KVS_GLFW_LIBRARIES glfw)
    elseif(TARGET glfw3)
        set(KVS_GLFW_LIBRARIES glfw3)
    else()
        find_path(KVS_GLFW_INCLUDE_DIR NAMES GLFW/glfw3.h HINTS ${KVS_GLFW_HINT_INCLUDE_DIRS})
        find_library(KVS_GLFW_LIBRARY NAMES glfw glfw3 HINTS ${KVS_GLFW_HINT_LIBRARY_DIRS})
        if(NOT KVS_GLFW_INCLUDE_DIR OR (NOT KVS_GLFW_LIBRARY AND NOT KVS_GLFW_ENV_LIBRARIES))
            message(FATAL_ERROR "KVS_SUPPORT_GLFW=ON requires GLFW.")
        endif()
        set(KVS_GLFW_INCLUDE_DIRS ${KVS_GLFW_HINT_INCLUDE_DIRS} ${KVS_GLFW_INCLUDE_DIR})
        if(KVS_GLFW_ENV_LIBRARIES)
            set(KVS_GLFW_LIBRARIES ${KVS_GLFW_ENV_LIBRARIES})
        else()
            set(KVS_GLFW_LIBRARIES ${KVS_GLFW_LIBRARY})
        endif()
    endif()

    kvs_collect_sources(kvsSupportGLFW_SOURCES SupportGLFW)
    add_library(kvsSupportGLFW STATIC ${kvsSupportGLFW_SOURCES})
    generate_export_header(kvsSupportGLFW)
    kvs_target_common(kvsSupportGLFW)
    target_include_directories(kvsSupportGLFW PUBLIC ${KVS_GL_INCLUDE_DIRS} ${KVS_GLFW_HINT_INCLUDE_DIRS} ${KVS_GLFW_INCLUDE_DIRS})
    kvs_target_link_directories_if_any(kvsSupportGLFW PUBLIC ${KVS_GL_LIBRARY_DIRS} ${KVS_GLFW_HINT_LIBRARY_DIRS})
    target_link_libraries(kvsSupportGLFW PUBLIC kvsCore ${KVS_GLFW_LIBRARIES} ${KVS_GL_LIBRARIES})
    kvs_install_library(kvsSupportGLFW kvsSupportGLFW SupportGLFW glfw)
endif()

#
# SupportFFmpeg
#
if(KVS_SUPPORT_FFMPEG)
    set(KVS_FFMPEG_LIBRARIES)
    set(KVS_FFMPEG_INCLUDE_DIRS)
    set(KVS_FFMPEG_LIBRARY_DIRS)
    kvs_make_include_paths(KVS_FFMPEG_HINT_INCLUDE_DIRS FFMPEG)
    kvs_make_library_paths(KVS_FFMPEG_HINT_LIBRARY_DIRS FFMPEG)
    kvs_make_link_libraries(KVS_FFMPEG_ENV_LIBRARIES FFMPEG)
    if(KVS_FFMPEG_ENV_LIBRARIES)
        find_path(KVS_FFMPEG_INCLUDE_DIR NAMES libavcodec/avcodec.h HINTS ${KVS_FFMPEG_HINT_INCLUDE_DIRS})
        if(NOT KVS_FFMPEG_INCLUDE_DIR AND NOT KVS_FFMPEG_HINT_INCLUDE_DIRS)
            message(FATAL_ERROR "KVS_SUPPORT_FFMPEG=ON requires FFmpeg development libraries.")
        endif()
        set(KVS_FFMPEG_INCLUDE_DIRS ${KVS_FFMPEG_HINT_INCLUDE_DIRS} ${KVS_FFMPEG_INCLUDE_DIR})
        set(KVS_FFMPEG_LIBRARY_DIRS ${KVS_FFMPEG_HINT_LIBRARY_DIRS})
        set(KVS_FFMPEG_LIBRARIES ${KVS_FFMPEG_ENV_LIBRARIES})
    elseif(PkgConfig_FOUND)
        pkg_check_modules(KVS_FFMPEG QUIET IMPORTED_TARGET
            libavcodec libavformat libavutil libavdevice libavfilter libswscale libswresample
        )
    endif()
    if(KVS_FFMPEG_LIBRARIES)
        # Libraries were supplied explicitly through KVS_FFMPEG_LINK_LIBRARY.
    elseif(TARGET PkgConfig::KVS_FFMPEG)
        set(KVS_FFMPEG_LIBRARIES PkgConfig::KVS_FFMPEG)
    else()
        find_path(KVS_FFMPEG_INCLUDE_DIR NAMES libavcodec/avcodec.h HINTS ${KVS_FFMPEG_HINT_INCLUDE_DIRS})
        foreach(_lib avcodec avformat avutil avdevice avfilter swscale swresample)
            find_library(KVS_FFMPEG_${_lib}_LIBRARY NAMES ${_lib} HINTS ${KVS_FFMPEG_HINT_LIBRARY_DIRS})
            if(NOT KVS_FFMPEG_${_lib}_LIBRARY)
                message(FATAL_ERROR "KVS_SUPPORT_FFMPEG=ON requires ${_lib}.")
            endif()
            list(APPEND KVS_FFMPEG_LIBRARIES ${KVS_FFMPEG_${_lib}_LIBRARY})
        endforeach()
        if(NOT KVS_FFMPEG_INCLUDE_DIR)
            message(FATAL_ERROR "KVS_SUPPORT_FFMPEG=ON requires FFmpeg development libraries.")
        endif()
        set(KVS_FFMPEG_INCLUDE_DIRS ${KVS_FFMPEG_HINT_INCLUDE_DIRS} ${KVS_FFMPEG_INCLUDE_DIR})
        set(KVS_FFMPEG_LIBRARY_DIRS ${KVS_FFMPEG_HINT_LIBRARY_DIRS})
    endif()

    kvs_collect_sources(kvsSupportFFmpeg_SOURCES SupportFFmpeg)
    add_library(kvsSupportFFmpeg STATIC ${kvsSupportFFmpeg_SOURCES})
    generate_export_header(kvsSupportFFmpeg)
    kvs_target_common(kvsSupportFFmpeg)
    target_include_directories(kvsSupportFFmpeg PUBLIC ${KVS_GL_INCLUDE_DIRS} ${KVS_FFMPEG_INCLUDE_DIRS})
    kvs_target_link_directories_if_any(kvsSupportFFmpeg PUBLIC ${KVS_GL_LIBRARY_DIRS} ${KVS_FFMPEG_LIBRARY_DIRS})
    target_link_libraries(kvsSupportFFmpeg PUBLIC kvsCore ${KVS_GL_LIBRARIES} ${KVS_FFMPEG_LIBRARIES})
    kvs_install_library(kvsSupportFFmpeg kvsSupportFFmpeg SupportFFmpeg ffmpeg)
endif()

#
# SupportQt
#
if(KVS_SUPPORT_QT)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    find_package(Qt6 QUIET COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets)
    if(Qt6_FOUND)
        set(KVS_QT_LIBRARIES Qt6::Core Qt6::Gui Qt6::Widgets Qt6::OpenGL Qt6::OpenGLWidgets)
        set(KVS_CONFIG_QT_PACKAGE Qt6)
        set(KVS_CONFIG_QT_COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets)
    else()
        find_package(Qt5 QUIET COMPONENTS Core Gui Widgets OpenGL)
        if(Qt5_FOUND)
            set(KVS_QT_LIBRARIES Qt5::Core Qt5::Gui Qt5::Widgets Qt5::OpenGL)
            set(KVS_CONFIG_QT_PACKAGE Qt5)
            set(KVS_CONFIG_QT_COMPONENTS Core Gui Widgets OpenGL)
        else()
            find_package(Qt4 REQUIRED QtCore QtGui QtOpenGL)
            include(${QT_USE_FILE})
            set(KVS_QT_LIBRARIES ${QT_LIBRARIES})
            set(KVS_QT_DEFINITIONS ${QT_DEFINITIONS})
            set(KVS_CONFIG_QT_PACKAGE Qt4)
            set(KVS_CONFIG_QT_COMPONENTS QtCore QtGui QtOpenGL)
        endif()
    endif()

    kvs_collect_sources(kvsSupportQt_SOURCES SupportQt)
    kvs_collect_headers(kvsSupportQt_HEADERS SupportQt)
    add_library(kvsSupportQt STATIC ${kvsSupportQt_SOURCES} ${kvsSupportQt_HEADERS})
    generate_export_header(kvsSupportQt)
    kvs_target_common(kvsSupportQt)
    target_compile_definitions(kvsSupportQt PRIVATE ${KVS_QT_DEFINITIONS})
    target_include_directories(kvsSupportQt PUBLIC ${KVS_GL_INCLUDE_DIRS})
    target_link_libraries(kvsSupportQt PUBLIC kvsCore ${KVS_GL_LIBRARIES} ${KVS_QT_LIBRARIES})
    kvs_install_library(kvsSupportQt kvsSupportQt SupportQt qt)
endif()

#
# SupportOpenCV
#
if(KVS_SUPPORT_OPENCV)
    set(KVS_OPENCV_LIBRARIES)
    set(KVS_OPENCV_INCLUDE_DIRS)
    set(KVS_OPENCV_DEFINITIONS)
    find_package(OpenCV QUIET)
    if(OpenCV_FOUND)
        set(KVS_OPENCV_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS})
        set(KVS_OPENCV_LIBRARIES ${OpenCV_LIBS})
        if(OpenCV_VERSION VERSION_GREATER_EQUAL 4)
            list(APPEND KVS_OPENCV_DEFINITIONS KVS_OPENCV4)
        endif()
    elseif(PkgConfig_FOUND)
        pkg_check_modules(KVS_OPENCV QUIET IMPORTED_TARGET opencv4)
        if(KVS_OPENCV_FOUND)
            list(APPEND KVS_OPENCV_DEFINITIONS KVS_OPENCV4)
        else()
            pkg_check_modules(KVS_OPENCV REQUIRED IMPORTED_TARGET opencv)
        endif()
        set(KVS_OPENCV_INCLUDE_DIRS ${KVS_OPENCV_INCLUDE_DIRS})
        set(KVS_OPENCV_LIBRARIES PkgConfig::KVS_OPENCV)
    else()
        message(FATAL_ERROR "KVS_SUPPORT_OPENCV=ON requires OpenCV.")
    endif()

    kvs_collect_sources(kvsSupportOpenCV_SOURCES SupportOpenCV)
    add_library(kvsSupportOpenCV STATIC ${kvsSupportOpenCV_SOURCES})
    generate_export_header(kvsSupportOpenCV)
    kvs_target_common(kvsSupportOpenCV)
    target_compile_definitions(kvsSupportOpenCV PUBLIC ${KVS_OPENCV_DEFINITIONS})
    target_include_directories(kvsSupportOpenCV PUBLIC ${KVS_GL_INCLUDE_DIRS} ${KVS_OPENCV_INCLUDE_DIRS})
    target_link_libraries(kvsSupportOpenCV PUBLIC kvsCore ${KVS_GL_LIBRARIES} ${KVS_OPENCV_LIBRARIES})
    kvs_install_library(kvsSupportOpenCV kvsSupportOpenCV SupportOpenCV opencv)
endif()

#
# SupportCUDA
#
if(KVS_SUPPORT_CUDA)
    find_package(CUDAToolkit REQUIRED)
    kvs_collect_sources(kvsSupportCUDA_SOURCES SupportCUDA)
    add_library(kvsSupportCUDA STATIC ${kvsSupportCUDA_SOURCES})
    generate_export_header(kvsSupportCUDA)
    kvs_target_common(kvsSupportCUDA)
    target_link_libraries(kvsSupportCUDA PUBLIC kvsCore CUDA::cuda_driver CUDA::cudart)
    kvs_install_library(kvsSupportCUDA kvsSupportCUDA SupportCUDA cuda)
endif()

#
# SupportPython
#
if(KVS_SUPPORT_PYTHON)
    kvs_collect_sources(kvsSupportPython_SOURCES SupportPython)
    add_library(kvsSupportPython STATIC ${kvsSupportPython_SOURCES})
    generate_export_header(kvsSupportPython)
    kvs_target_common(kvsSupportPython)
    target_include_directories(kvsSupportPython PUBLIC ${Python3_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIRS})
    target_link_libraries(kvsSupportPython PUBLIC kvsCore Python3::Python Python3::NumPy)
    kvs_install_library(kvsSupportPython kvsSupportPython SupportPython python)
endif()

#
# SupportMPI
#
if(KVS_SUPPORT_MPI)
    kvs_make_include_paths(KVS_MPI_HINT_INCLUDE_DIRS MPI)
    kvs_make_library_paths(KVS_MPI_HINT_LIBRARY_DIRS MPI)
    kvs_make_link_libraries(KVS_MPI_ENV_LIBRARIES MPI)
    find_package(MPI REQUIRED COMPONENTS CXX)
    kvs_collect_sources(kvsSupportMPI_SOURCES SupportMPI)
    add_library(kvsSupportMPI STATIC ${kvsSupportMPI_SOURCES})
    generate_export_header(kvsSupportMPI)
    kvs_target_common(kvsSupportMPI)
    target_include_directories(kvsSupportMPI PUBLIC ${KVS_MPI_HINT_INCLUDE_DIRS})
    kvs_target_link_directories_if_any(kvsSupportMPI PUBLIC ${KVS_MPI_HINT_LIBRARY_DIRS})
    target_link_libraries(kvsSupportMPI PUBLIC kvsCore MPI::MPI_CXX ${KVS_MPI_ENV_LIBRARIES})
    kvs_install_library(kvsSupportMPI kvsSupportMPI SupportMPI mpi)
endif()

#
# SupportEGL
#
if(KVS_SUPPORT_EGL)
    kvs_make_include_paths(KVS_EGL_HINT_INCLUDE_DIRS EGL)
    kvs_make_library_paths(KVS_EGL_HINT_LIBRARY_DIRS EGL)
    kvs_make_link_libraries(KVS_EGL_ENV_LIBRARIES EGL)
    find_path(KVS_EGL_INCLUDE_DIR NAMES EGL/egl.h HINTS ${KVS_EGL_HINT_INCLUDE_DIRS})
    find_library(KVS_EGL_LIBRARY NAMES EGL egl HINTS ${KVS_EGL_HINT_LIBRARY_DIRS})
    if(NOT KVS_EGL_INCLUDE_DIR OR (NOT KVS_EGL_LIBRARY AND NOT KVS_EGL_ENV_LIBRARIES))
        message(FATAL_ERROR "KVS_SUPPORT_EGL=ON requires EGL.")
    endif()

    kvs_collect_sources(kvsSupportEGL_SOURCES SupportEGL)
    add_library(kvsSupportEGL STATIC ${kvsSupportEGL_SOURCES})
    generate_export_header(kvsSupportEGL)
    kvs_target_common(kvsSupportEGL)
    target_include_directories(kvsSupportEGL PUBLIC ${KVS_GL_INCLUDE_DIRS} ${KVS_EGL_HINT_INCLUDE_DIRS} ${KVS_EGL_INCLUDE_DIR})
    kvs_target_link_directories_if_any(kvsSupportEGL PUBLIC ${KVS_GL_LIBRARY_DIRS} ${KVS_EGL_HINT_LIBRARY_DIRS})
    if(KVS_EGL_ENV_LIBRARIES)
        target_link_libraries(kvsSupportEGL PUBLIC kvsCore ${KVS_GL_LIBRARIES} ${KVS_EGL_ENV_LIBRARIES})
    else()
        target_link_libraries(kvsSupportEGL PUBLIC kvsCore ${KVS_GL_LIBRARIES} ${KVS_EGL_LIBRARY})
    endif()
    kvs_install_library(kvsSupportEGL kvsSupportEGL SupportEGL egl)
endif()

#
# SupportOSMesa
#
if(KVS_ENABLE_OPENGL AND KVS_SUPPORT_OSMESA)
    kvs_collect_sources(kvsSupportOSMesa_SOURCES SupportOSMesa)
    add_library(kvsSupportOSMesa STATIC ${kvsSupportOSMesa_SOURCES})
    generate_export_header(kvsSupportOSMesa)
    kvs_target_common(kvsSupportOSMesa)
    target_include_directories(kvsSupportOSMesa PUBLIC ${KVS_GL_INCLUDE_DIRS})
    kvs_target_link_directories_if_any(kvsSupportOSMesa PUBLIC ${KVS_GL_LIBRARY_DIRS})
    target_link_libraries(kvsSupportOSMesa PUBLIC kvsCore ${KVS_GL_LIBRARIES})
    kvs_install_library(kvsSupportOSMesa kvsSupportOSMesa SupportOSMesa osmesa)
endif()

kvs_install_cmake_package()
