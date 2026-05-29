include_guard(GLOBAL)

function(kvs_to_cmake_bool out_var value)
    string(STRIP "${value}" _value)
    string(TOUPPER "${_value}" _upper)
    if(_upper STREQUAL "1" OR _upper STREQUAL "ON" OR _upper STREQUAL "YES" OR _upper STREQUAL "TRUE")
        set(${out_var} ON PARENT_SCOPE)
    elseif(_upper STREQUAL "0" OR _upper STREQUAL "OFF" OR _upper STREQUAL "NO" OR _upper STREQUAL "FALSE")
        set(${out_var} OFF PARENT_SCOPE)
    else()
        set(${out_var} "${_value}" PARENT_SCOPE)
    endif()
endfunction()

function(kvs_read_kvs_conf filename)
    if(NOT EXISTS "${filename}")
        message(FATAL_ERROR "kvs.conf was not found: ${filename}")
    endif()

    file(STRINGS "${filename}" _lines)
    foreach(_line IN LISTS _lines)
        string(REGEX REPLACE "#.*$" "" _line "${_line}")
        if(_line MATCHES "^[ \t]*([A-Za-z_][A-Za-z0-9_]*)[ \t]*=[ \t]*(.*)$")
            set(_name "${CMAKE_MATCH_1}")
            set(_value "${CMAKE_MATCH_2}")
            kvs_to_cmake_bool(_cmake_value "${_value}")
            set(KVS_CONF_${_name} "${_cmake_value}" PARENT_SCOPE)
        endif()
    endforeach()
endfunction()

function(kvs_cache_string name help default_value)
    if(DEFINED CACHE{${name}})
        return()
    endif()

    if(DEFINED ENV{${name}} AND NOT "$ENV{${name}}" STREQUAL "")
        set(_value "$ENV{${name}}")
    else()
        set(_value "${default_value}")
    endif()

    set(${name} "${_value}" CACHE STRING "${help}")
endfunction()

function(kvs_cache_path name help default_value)
    if(DEFINED CACHE{${name}})
        return()
    endif()

    if(DEFINED ENV{${name}} AND NOT "$ENV{${name}}" STREQUAL "")
        set(_value "$ENV{${name}}")
    else()
        set(_value "${default_value}")
    endif()

    set(${name} "${_value}" CACHE PATH "${help}")
endfunction()

function(kvs_cache_string_from_conf name help fallback)
    if(DEFINED CACHE{${name}})
        return()
    endif()

    if(DEFINED ENV{${name}} AND NOT "$ENV{${name}}" STREQUAL "")
        set(_value "$ENV{${name}}")
    elseif(DEFINED KVS_CONF_${name})
        set(_value "${KVS_CONF_${name}}")
    else()
        set(_value "${fallback}")
    endif()

    set(${name} "${_value}" CACHE STRING "${help}")
endfunction()

function(kvs_cache_bool_from_conf name help fallback)
    if(DEFINED CACHE{${name}})
        return()
    endif()

    if(DEFINED ENV{${name}} AND NOT "$ENV{${name}}" STREQUAL "")
        kvs_to_cmake_bool(_value "$ENV{${name}}")
    elseif(DEFINED KVS_CONF_${name})
        set(_value "${KVS_CONF_${name}}")
    else()
        set(_value "${fallback}")
    endif()

    set(${name} "${_value}" CACHE BOOL "${help}")
endfunction()

function(kvs_cache_make_environment name)
    if(DEFINED CACHE{${name}})
        return()
    endif()

    if(DEFINED ENV{${name}} AND NOT "$ENV{${name}}" STREQUAL "")
        set(_value "$ENV{${name}}")
    else()
        set(_value "")
    endif()

    set(${name} "${_value}" CACHE STRING "Imported from the ${name} environment variable")
endfunction()

function(kvs_read_makefile_defaults filename)
    if(NOT EXISTS "${filename}")
        message(FATAL_ERROR "Makefile.def was not found: ${filename}")
    endif()

    file(STRINGS "${filename}" _lines)
    set(_ifndef_name "")
    foreach(_line IN LISTS _lines)
        string(REGEX REPLACE "#.*$" "" _line "${_line}")

        if(_line MATCHES "^[ \t]*ifndef[ \t]+([A-Za-z_][A-Za-z0-9_]*)")
            set(_ifndef_name "${CMAKE_MATCH_1}")
        elseif(_ifndef_name AND _line MATCHES "^[ \t]*${_ifndef_name}[ \t]*:?=[ \t]*(.+)$")
            string(STRIP "${CMAKE_MATCH_1}" _default_value)
            kvs_cache_string(${_ifndef_name} "Imported from Makefile.def" "${_default_value}")
            set(_ifndef_name "")
        elseif(_line MATCHES "^[ \t]*endif")
            set(_ifndef_name "")
        endif()
    endforeach()
endfunction()

function(kvs_collect_make_definitions)
    set(_defs)

    foreach(_def
        KVS_ENABLE_OPENGL
        KVS_ENABLE_GLU
        KVS_ENABLE_GLEW
        KVS_ENABLE_OPENMP
        KVS_ENABLE_DEPRECATED
        KVS_SUPPORT_FFMPEG
        KVS_SUPPORT_QT
        KVS_SUPPORT_OPENCV
        KVS_SUPPORT_CUDA
        KVS_SUPPORT_PYTHON
        KVS_SUPPORT_MPI
        KVS_SUPPORT_EGL
        KVS_SUPPORT_OSMESA
    )
        if(${_def})
            list(APPEND _defs ${_def})
        endif()
    endforeach()

    if(KVS_ENABLE_OPENGL AND KVS_SUPPORT_GLUT)
        list(APPEND _defs KVS_SUPPORT_GLUT)
    endif()
    if(KVS_ENABLE_OPENGL AND KVS_SUPPORT_GLFW)
        list(APPEND _defs KVS_SUPPORT_GLFW)
    endif()

    set(KVS_MAKE_DEFINITIONS ${_defs} PARENT_SCOPE)
endfunction()

function(kvs_import_make_environment_variables)
    foreach(_var
        KVS_GL_INCLUDE_PATH
        KVS_GL_LIBRARY_PATH
        KVS_GL_LINK_LIBRARY
        KVS_GLU_LINK_LIBRARY
        KVS_GLEW_INCLUDE_PATH
        KVS_GLEW_LIBRARY_PATH
        KVS_GLEW_LINK_LIBRARY
        KVS_OPENMP_INCLUDE_PATH
        KVS_OPENMP_LIBRARY_PATH
        KVS_OPENMP_LINK_LIBRARY
        KVS_GLUT_INCLUDE_PATH
        KVS_GLUT_LIBRARY_PATH
        KVS_GLUT_LINK_LIBRARY
        KVS_GLFW_INCLUDE_PATH
        KVS_GLFW_LIBRARY_PATH
        KVS_GLFW_LINK_LIBRARY
        KVS_FFMPEG_INCLUDE_PATH
        KVS_FFMPEG_LIBRARY_PATH
        KVS_FFMPEG_LINK_LIBRARY
        KVS_MPI_INCLUDE_PATH
        KVS_MPI_LIBRARY_PATH
        KVS_MPI_LINK_LIBRARY
        KVS_EGL_INCLUDE_PATH
        KVS_EGL_LIBRARY_PATH
        KVS_EGL_LINK_LIBRARY
        KVS_OSMESA_INCLUDE_PATH
        KVS_OSMESA_LIBRARY_PATH
        KVS_OSMESA_LINK_LIBRARY
    )
        kvs_cache_make_environment(${_var})
    endforeach()
endfunction()

function(kvs_parse_make_prefixed_paths out_var prefix value)
    set(_paths)
    if(value)
        separate_arguments(_tokens UNIX_COMMAND "${value}")
        set(_expect_path OFF)
        foreach(_token IN LISTS _tokens)
            if(_expect_path)
                list(APPEND _paths "${_token}")
                set(_expect_path OFF)
            elseif(_token STREQUAL "${prefix}")
                set(_expect_path ON)
            elseif(_token MATCHES "^${prefix}(.+)$")
                list(APPEND _paths "${CMAKE_MATCH_1}")
            elseif(NOT _token MATCHES "^-")
                list(APPEND _paths "${_token}")
            endif()
        endforeach()
    endif()
    set(${out_var} ${_paths} PARENT_SCOPE)
endfunction()

function(kvs_parse_make_link_libraries out_var value)
    set(_libraries)
    if(value)
        separate_arguments(_tokens UNIX_COMMAND "${value}")
        set(_expect_framework OFF)
        foreach(_token IN LISTS _tokens)
            if(_expect_framework)
                list(APPEND _libraries "-framework ${_token}")
                set(_expect_framework OFF)
            elseif(_token STREQUAL "-framework")
                set(_expect_framework ON)
            else()
                list(APPEND _libraries "${_token}")
            endif()
        endforeach()
    endif()
    set(${out_var} ${_libraries} PARENT_SCOPE)
endfunction()

function(kvs_make_include_paths out_var component)
    set(_paths)
    set(_dir_var "KVS_${component}_DIR")
    set(_include_var "KVS_${component}_INCLUDE_PATH")

    if(DEFINED ${_dir_var} AND NOT "${${_dir_var}}" STREQUAL "")
        list(APPEND _paths "${${_dir_var}}/include")
    endif()
    if(DEFINED ${_include_var} AND NOT "${${_include_var}}" STREQUAL "")
        kvs_parse_make_prefixed_paths(_env_paths "-I" "${${_include_var}}")
        list(APPEND _paths ${_env_paths})
    endif()

    set(${out_var} ${_paths} PARENT_SCOPE)
endfunction()

function(kvs_make_library_paths out_var component)
    set(_paths)
    set(_dir_var "KVS_${component}_DIR")
    set(_library_var "KVS_${component}_LIBRARY_PATH")

    if(DEFINED ${_dir_var} AND NOT "${${_dir_var}}" STREQUAL "")
        list(APPEND _paths "${${_dir_var}}/lib")
        list(APPEND _paths "${${_dir_var}}/lib64")
    endif()
    if(DEFINED ${_library_var} AND NOT "${${_library_var}}" STREQUAL "")
        kvs_parse_make_prefixed_paths(_env_paths "-L" "${${_library_var}}")
        list(APPEND _paths ${_env_paths})
    endif()

    set(${out_var} ${_paths} PARENT_SCOPE)
endfunction()

function(kvs_make_link_libraries out_var component)
    set(_link_var "KVS_${component}_LINK_LIBRARY")
    if(DEFINED ${_link_var} AND NOT "${${_link_var}}" STREQUAL "")
        kvs_parse_make_link_libraries(_libraries "${${_link_var}}")
    else()
        set(_libraries)
    endif()
    set(${out_var} ${_libraries} PARENT_SCOPE)
endfunction()

function(kvs_interpret_makefile_def makefile_def kvs_conf)
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${makefile_def}" "${kvs_conf}")

    kvs_read_kvs_conf("${kvs_conf}")

    kvs_cache_string_from_conf(KVS_CPP_STANDARD "C++ standard for KVS" 17)
    kvs_read_makefile_defaults("${makefile_def}")

    set_property(CACHE KVS_CPP_STANDARD PROPERTY STRINGS 11 14 17 20 23)

    kvs_cache_bool_from_conf(KVS_ENABLE_OPENGL "Enable OpenGL support" ON)
    kvs_cache_bool_from_conf(KVS_ENABLE_GLU "Enable GLU support" ON)
    kvs_cache_bool_from_conf(KVS_ENABLE_GLEW "Enable GLEW support" OFF)
    kvs_cache_bool_from_conf(KVS_ENABLE_OPENMP "Enable OpenMP support" OFF)
    kvs_cache_bool_from_conf(KVS_ENABLE_DEPRECATED "Enable deprecated APIs" OFF)

    kvs_cache_bool_from_conf(KVS_SUPPORT_CUDA "Build SupportCUDA" OFF)
    kvs_cache_bool_from_conf(KVS_SUPPORT_GLUT "Build SupportGLUT" ON)
    kvs_cache_bool_from_conf(KVS_SUPPORT_GLFW "Build SupportGLFW" OFF)
    kvs_cache_bool_from_conf(KVS_SUPPORT_FFMPEG "Build SupportFFmpeg" OFF)
    kvs_cache_bool_from_conf(KVS_SUPPORT_OPENCV "Build SupportOpenCV" OFF)
    kvs_cache_bool_from_conf(KVS_SUPPORT_QT "Build SupportQt" OFF)
    kvs_cache_bool_from_conf(KVS_SUPPORT_PYTHON "Build SupportPython" OFF)
    kvs_cache_bool_from_conf(KVS_SUPPORT_MPI "Build SupportMPI" OFF)
    kvs_cache_bool_from_conf(KVS_SUPPORT_EGL "Build SupportEGL" OFF)
    kvs_cache_bool_from_conf(KVS_SUPPORT_OSMESA "Build SupportOSMesa" OFF)

    kvs_cache_bool_from_conf(KVS_TOOL "Build KVS command line tools" ON)
    kvs_cache_bool_from_conf(KVS_INSTALL_CMAKE_PACKAGE "Install CMake find_package support files" ON)
    kvs_import_make_environment_variables()

    if(KVS_SUPPORT_OSMESA)
        set(KVS_ENABLE_OPENGL ON CACHE BOOL "Enable OpenGL support" FORCE)
    endif()

    kvs_collect_make_definitions()
    set(KVS_MAKE_DEFINITIONS ${KVS_MAKE_DEFINITIONS} PARENT_SCOPE)
endfunction()

function(kvs_apply_makefile_compile_options target)
    set_target_properties(${target} PROPERTIES
        CXX_STANDARD ${KVS_CPP_STANDARD}
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )

    target_compile_definitions(${target}
        PRIVATE
            "$<$<CONFIG:Debug>:_DEBUG>"
            "$<$<CONFIG:Debug>:KVS_ENABLE_DEBUG>"
            "$<$<NOT:$<CONFIG:Debug>>:NDEBUG>"
    )

    if(KVS_MAKE_DEFINITIONS)
        target_compile_definitions(${target} PUBLIC ${KVS_MAKE_DEFINITIONS})
    endif()

    if(MSVC)
        target_compile_options(${target} PRIVATE /W3)
    else()
        target_compile_options(${target} PRIVATE -Wall)
        if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
            target_compile_options(${target} PRIVATE -Wno-unused-result)
        endif()
    endif()
endfunction()
