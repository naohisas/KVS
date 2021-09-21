/*****************************************************************************/
/**
 *  @file   NumPy.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#define NO_IMPORT_ARRAY
#define PY_ARRAY_UNIQUE_SYMBOL KVS_PYTHON_NUMPY_ARRAYOBJECT_H
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

#include <kvs/Compiler>
#if defined( KVS_COMPILER_FCC )
#define __AARCH64EL__ 1
#define __ARM_64BIT_STATE 1 
#endif
