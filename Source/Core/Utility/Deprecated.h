/*****************************************************************************/
/**
 *  @file   Deprecated.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#ifndef KVS__DEPRECATED_H_INCLUDE
#define KVS__DEPRECATED_H_INCLUDE

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

#endif // KVS__DEPRECATED_H_INCLUDE
