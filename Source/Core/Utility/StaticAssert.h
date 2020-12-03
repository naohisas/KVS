/*****************************************************************************/
/**
 *  @file   StaticAssert.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/Compiler>

// Workaround for static_assert
#define KVS__INTERNAL__STATIC_ASSERT( expr ) typedef char KVS_STATIC_ASSERTION_FAILURE[ ( expr ) ? 1 : -1 ]

// GCC
#if defined KVS_COMPILER_GCC
  // Supported
  #if (__cplusplus >= 201103)
    #define KVS_STATIC_ASSERT( expr, mes ) static_assert( expr, mes )
  #else
    // Not supported
    #if !defined(__cpp_static_assert) && !defined(static_assert)
      #if KVS_COMPILER_VERSION_LESS_THAN( 3, 1 )
        #define KVS_STATIC_ASSERT( expr, mes ) KVS__INTERANL__STATIC_ASSERTION( expr )
      #else
        #define KVS_STATIC_ASSERT( expr, mes ) KVS__INTERANL__STATIC_ASSERTION( expr ) __attribute__((unused))
      #endif
    // Supported
    #else
      // GCC
      #if defined(__GNUC__)
        #if (__GNUC__ >= 6)
          #define KVS_STATIC_ASSERT( expr, mes ) static_assert( expr, mes )
        #elif (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
          #define KVS_STATIC_ASSERT( expr, mes ) _Static_assert( expr, mes )
        #else
          #define KVS_STATIC_ASSERT( expr, mes ) KVS__INTERANL__STATIC_ASSERTION( expr )
        #endif
      // Clang (Apple GCC)
      #elif defined(__clang__)
        #if (__clang_major__ >= 9)
          #define KVS_STATIC_ASSERT( expr, mes ) static_assert( expr, mes )
        #else
          #define KVS_STATIC_ASSERT( expr, mes ) static_assert( expr, mes ) __attribute__((unused))
        #endif
      #else
        #define KVS_STATIC_ASSERT( expr, mes ) static_assert( expr, mes )
      #endif
    #endif
  #endif

// VC
#elif defined KVS_COMPILER_VC
  #if KVS_COMPILER_VERSION_LESS_THAN( 10, 0 )
    #define KVS_STATIC_ASSERT( expr, mes ) KVS__INTERANL__STATIC_ASSERTION( expr )
  #else
    #define KVS_STATIC_ASSERT( expr, mes ) static_assert( expr, mes )
  #endif

// FCC
#elif defined KVS_COMPILER_FCC
  #define KVS_STATIC_ASSERT( expr, mes ) KVS__INTERANL__STATIC_ASSERTION( expr )

#else
  #define KVS_STATIC_ASSERT( expr, mes ) KVS__INTERANL__STATIC_ASSERTION( expr )
#endif
