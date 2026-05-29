cmake_minimum_required(VERSION 3.16)

include(GNUInstallDirs)

if(NOT TARGET kvsCore)
    message(FATAL_ERROR "Tool requires kvsCore target.")
endif()

function(kvs_link_if_target tool_name target_name)
    if(TARGET ${target_name})
        target_link_libraries(${tool_name} PRIVATE ${target_name})
    endif()
endfunction()

function(kvs_link_support_if_target tool_name target_name)
    if(TARGET ${target_name})
        target_link_libraries(${tool_name} PRIVATE ${target_name})
        set_property(TARGET ${tool_name} PROPERTY KVS_HAS_SUPPORT_LINK TRUE)
    endif()
endfunction()

function(kvs_link_core_if_no_support tool_name)
    get_property(_has_support TARGET ${tool_name} PROPERTY KVS_HAS_SUPPORT_LINK)
    if(NOT _has_support)
        target_link_libraries(${tool_name} PRIVATE kvsCore)
    endif()
endfunction()

function(kvs_link_cuda_tool_libraries tool_name)
    if(KVS_SUPPORT_CUDA)
        kvs_link_if_target(${tool_name} CUDA::cuda_driver)
        kvs_link_if_target(${tool_name} CUDA::cudart)
    endif()
endfunction()

function(kvs_add_tool tool_name)
    file(GLOB_RECURSE ${tool_name}_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${tool_name}/*.cpp")
    add_executable(${tool_name} ${${tool_name}_SOURCES})
    kvs_apply_makefile_compile_options(${tool_name})
    target_include_directories(${tool_name} PRIVATE "${PROJECT_SOURCE_DIR}/Source")
    install(TARGETS ${tool_name} DESTINATION ${CMAKE_INSTALL_BINDIR})
endfunction()

kvs_add_tool(kvsmake)
target_link_libraries(kvsmake PRIVATE kvsCore)

kvs_add_tool(kvscheck)
foreach(_target
    kvsSupportCUDA
    kvsSupportGLUT
    kvsSupportGLFW
    kvsSupportOpenCV
    kvsSupportPython
    kvsSupportMPI
    kvsSupportEGL
    kvsSupportOSMesa
)
    kvs_link_support_if_target(kvscheck ${_target})
endforeach()
kvs_link_core_if_no_support(kvscheck)
if(KVS_SUPPORT_QT)
    find_package(Qt6 QUIET COMPONENTS Core)
    if(Qt6_FOUND)
        set(_kvscheck_qt_version "${Qt6Core_VERSION}")
        if(NOT _kvscheck_qt_version)
            set(_kvscheck_qt_version "${Qt6_VERSION}")
        endif()
    else()
        find_package(Qt5 QUIET COMPONENTS Core)
        set(_kvscheck_qt_version "${Qt5Core_VERSION_STRING}")
        if(NOT _kvscheck_qt_version)
            set(_kvscheck_qt_version "${Qt5Core_VERSION}")
        endif()
        if(NOT _kvscheck_qt_version)
            find_package(Qt4 QUIET QtCore)
            set(_kvscheck_qt_version "${QT_VERSION}")
        endif()
    endif()
    if(_kvscheck_qt_version)
        target_compile_definitions(kvscheck PRIVATE "KVSCHECK_QT_VERSION=${_kvscheck_qt_version}")
    endif()
endif()

kvs_add_tool(kvsconv)
foreach(_target
    kvsSupportGLUT
    kvsSupportOpenCV
)
    kvs_link_support_if_target(kvsconv ${_target})
endforeach()
kvs_link_cuda_tool_libraries(kvsconv)
kvs_link_core_if_no_support(kvsconv)

if(KVS_SUPPORT_GLUT OR KVS_SUPPORT_GLFW)
    kvs_add_tool(kvsview)
    foreach(_target
        kvsSupportGLUT
        kvsSupportGLFW
        kvsSupportOpenCV
    )
        kvs_link_support_if_target(kvsview ${_target})
    endforeach()
    kvs_link_cuda_tool_libraries(kvsview)
    kvs_link_core_if_no_support(kvsview)
endif()

install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/kvsmake/Makefile.kvs.template"
    "${CMAKE_CURRENT_SOURCE_DIR}/kvsmake/QtProject.template"
    "${CMAKE_CURRENT_SOURCE_DIR}/kvsmake/VCProject.template"
    "${CMAKE_CURRENT_SOURCE_DIR}/kvsmake/VCXProject.template"
    "${CMAKE_CURRENT_SOURCE_DIR}/kvsmake/VCProjectCUDA.template"
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)
