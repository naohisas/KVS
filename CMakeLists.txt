cmake_minimum_required(VERSION 3.16)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(KVSMakefileDef)

set(KVS_CONF_FILE "${CMAKE_CURRENT_SOURCE_DIR}/kvs.conf" CACHE FILEPATH "KVS configuration file")
set(KVS_MAKEFILE_DEF "${CMAKE_CURRENT_SOURCE_DIR}/Makefile.def" CACHE FILEPATH "KVS Makefile.def file")
kvs_cache_makefile_default(
    KVS_MACOSX_DEPLOYMENT_TARGET
    "${KVS_MAKEFILE_DEF}"
    "macOS deployment target imported from KVS_MACOSX_DEPLOYMENT_TARGET or Makefile.def"
)
if(NOT "${KVS_MACOSX_DEPLOYMENT_TARGET}" STREQUAL "" AND
    (NOT DEFINED CMAKE_OSX_DEPLOYMENT_TARGET OR "${CMAKE_OSX_DEPLOYMENT_TARGET}" STREQUAL ""))
    set(CMAKE_OSX_DEPLOYMENT_TARGET "${KVS_MACOSX_DEPLOYMENT_TARGET}" CACHE STRING "macOS deployment target" FORCE)
endif()

if(NOT DEFINED CMAKE_CXX_COMPILER AND DEFINED ENV{KVS_CPP} AND NOT "$ENV{KVS_CPP}" STREQUAL "")
    set(CMAKE_CXX_COMPILER "$ENV{KVS_CPP}" CACHE FILEPATH "C++ compiler imported from KVS_CPP")
endif()

project(kvs VERSION 2.4.0 LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_INSTALL_BINDIR "bin" CACHE PATH "KVS make-compatible executable install directory" FORCE)
set(CMAKE_INSTALL_INCLUDEDIR "include" CACHE PATH "KVS make-compatible header install directory" FORCE)
set(CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "KVS make-compatible library install directory" FORCE)

kvs_interpret_makefile_def("${KVS_MAKEFILE_DEF}" "${KVS_CONF_FILE}")

set(CMAKE_CXX_STANDARD ${KVS_CPP_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

option(DEBUG "Build with KVS debug definitions" OFF)
option(RELEASE "Build with KVS release definitions" OFF)
if(DEBUG)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()
if(RELEASE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

if(DEFINED CACHE{KVS_DIR})
    set(_kvs_install_prefix "${KVS_DIR}")
elseif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND DEFINED ENV{KVS_DIR} AND NOT "$ENV{KVS_DIR}" STREQUAL "")
    set(_kvs_install_prefix "$ENV{KVS_DIR}")
else()
    set(_kvs_install_prefix "${CMAKE_INSTALL_PREFIX}")
endif()
set(KVS_DIR "${_kvs_install_prefix}" CACHE PATH "KVS installation prefix" FORCE)
set(CMAKE_INSTALL_PREFIX "${KVS_DIR}" CACHE PATH "Install prefix used by make-compatible KVS_DIR" FORCE)
set(ENV{KVS_DIR} "${KVS_DIR}")

# Optional dependency roots corresponding to KVS_*_DIR variables in
# Makefile.def. CMake package discovery also honors CMAKE_PREFIX_PATH.
foreach(_dep GL GLU GLEW GLUT GLFW FFMPEG OPENCV QT PYTHON NUMPY MPI EGL OSMESA CUDA CUDA_SDK OPENMP CAVE)
    kvs_cache_path(KVS_${_dep}_DIR "Optional ${_dep} installation prefix" "")
    if(KVS_${_dep}_DIR)
        list(APPEND CMAKE_PREFIX_PATH "${KVS_${_dep}_DIR}")
    endif()
endforeach()

if(NOT DEFINED CACHE{OpenGL_ROOT} AND KVS_GL_DIR)
    set(OpenGL_ROOT "${KVS_GL_DIR}" CACHE PATH "OpenGL root imported from KVS_GL_DIR")
endif()
if(NOT DEFINED CACHE{GLEW_ROOT} AND KVS_GLEW_DIR)
    set(GLEW_ROOT "${KVS_GLEW_DIR}" CACHE PATH "GLEW root imported from KVS_GLEW_DIR")
endif()
if(NOT DEFINED CACHE{GLUT_ROOT} AND KVS_GLUT_DIR)
    set(GLUT_ROOT "${KVS_GLUT_DIR}" CACHE PATH "GLUT root imported from KVS_GLUT_DIR")
endif()

if(NOT DEFINED CACHE{Python3_EXECUTABLE} AND DEFINED ENV{KVS_PY} AND NOT "$ENV{KVS_PY}" STREQUAL "")
    find_program(_kvs_python_executable NAMES "$ENV{KVS_PY}")
    if(_kvs_python_executable)
        set(Python3_EXECUTABLE "${_kvs_python_executable}" CACHE FILEPATH "Python executable imported from KVS_PY")
    else()
        set(Python3_EXECUTABLE "$ENV{KVS_PY}" CACHE FILEPATH "Python executable imported from KVS_PY")
    endif()
endif()

if(NOT DEFINED CACHE{Python3_ROOT_DIR} AND KVS_PYTHON_DIR)
    set(Python3_ROOT_DIR "${KVS_PYTHON_DIR}" CACHE PATH "Python root imported from KVS_PYTHON_DIR")
endif()

if(NOT DEFINED CACHE{MPI_CXX_COMPILER} AND DEFINED ENV{KVS_MPI_CPP} AND NOT "$ENV{KVS_MPI_CPP}" STREQUAL "")
    set(MPI_CXX_COMPILER "$ENV{KVS_MPI_CPP}" CACHE FILEPATH "MPI C++ compiler imported from KVS_MPI_CPP")
endif()

if(NOT DEFINED CACHE{CUDAToolkit_ROOT} AND KVS_CUDA_DIR)
    set(CUDAToolkit_ROOT "${KVS_CUDA_DIR}" CACHE PATH "CUDA root imported from KVS_CUDA_DIR")
endif()

if(DEFINED ENV{KVS_LD} AND NOT "$ENV{KVS_LD}" STREQUAL "")
    set(
        CMAKE_CXX_LINK_EXECUTABLE
        "$ENV{KVS_LD} <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>"
    )
endif()

set(CMAKE_DEBUG_POSTFIX "d")

message(STATUS "CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_COMPILER_ID=${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}")
message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message(STATUS "KVS_CPP_STANDARD=${KVS_CPP_STANDARD}")
message(STATUS "KVS_MACOSX_DEPLOYMENT_TARGET=${KVS_MACOSX_DEPLOYMENT_TARGET}")
message(STATUS "KVS_MAKE_DEFINITIONS=${KVS_MAKE_DEFINITIONS}")
message(STATUS "KVS_ENABLE_OPENGL=${KVS_ENABLE_OPENGL}")
message(STATUS "KVS_ENABLE_GLU=${KVS_ENABLE_GLU}")
message(STATUS "KVS_ENABLE_GLEW=${KVS_ENABLE_GLEW}")
message(STATUS "KVS_ENABLE_OPENMP=${KVS_ENABLE_OPENMP}")
message(STATUS "KVS_ENABLE_DEPRECATED=${KVS_ENABLE_DEPRECATED}")
message(STATUS "KVS_SUPPORT_CUDA=${KVS_SUPPORT_CUDA}")
message(STATUS "KVS_SUPPORT_GLUT=${KVS_SUPPORT_GLUT}")
message(STATUS "KVS_SUPPORT_GLFW=${KVS_SUPPORT_GLFW}")
message(STATUS "KVS_SUPPORT_FFMPEG=${KVS_SUPPORT_FFMPEG}")
message(STATUS "KVS_SUPPORT_OPENCV=${KVS_SUPPORT_OPENCV}")
message(STATUS "KVS_SUPPORT_QT=${KVS_SUPPORT_QT}")
message(STATUS "KVS_SUPPORT_PYTHON=${KVS_SUPPORT_PYTHON}")
message(STATUS "KVS_SUPPORT_MPI=${KVS_SUPPORT_MPI}")
message(STATUS "KVS_SUPPORT_EGL=${KVS_SUPPORT_EGL}")
message(STATUS "KVS_SUPPORT_OSMESA=${KVS_SUPPORT_OSMESA}")
message(STATUS "KVS_TOOL=${KVS_TOOL}")
message(STATUS "KVS_INSTALL_CMAKE_PACKAGE=${KVS_INSTALL_CMAKE_PACKAGE}")
message(STATUS "KVS_DIR=${KVS_DIR}")

add_subdirectory(Source)

if(KVS_TOOL)
    add_subdirectory(Tool)
endif()

install(
    FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/kvs.conf"
        "${CMAKE_CURRENT_SOURCE_DIR}/Makefile.def"
    DESTINATION "."
)
