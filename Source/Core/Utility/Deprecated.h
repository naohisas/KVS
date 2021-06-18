/*****************************************************************************/
/**
 *  @file   Deprecated.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include "Compiler.h"

// MS VC compiler.
#if defined ( KVS_COMPILER_VC )
#    if KVS_COMPILER_VERSION_LESS_THAN( 7, 0 )
#        define KVS_DEPRECATED( function ) function
#    else
#        define KVS_DEPRECATED( function ) __declspec(deprecated) function
#    endif

// GCC compiler.
#elif defined ( KVS_COMPILER_GCC )
#    if KVS_COMPILER_VERSION_LESS_THAN( 3, 1 )
#        define KVS_DEPRECATED( function ) function
#    else
#        define KVS_DEPRECATED( function ) function __attribute__((deprecated))
#    endif

// Other compiler.
#else
#    define KVS_DEPRECATED( function ) function
#endif
