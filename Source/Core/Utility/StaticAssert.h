#pragma once
#include <kvs/Compiler>


#if defined KVS_COMPILER_GCC
  #if KVS_COMPILER_VERSION_LESS_THAN( 3, 1 )
    #define KVS_STATIC_ASSERT( expr, mes ) typedef char KVS_STATIC_ASSERTION_FAILURE[ ( expr ) ? 1 : -1 ]
  #elif KVS_COMPILER_VERSION_LESS_THAN( 4, 8 )
    #define KVS_STATIC_ASSERT( expr, mes ) typedef char KVS_STATIC_ASSERTION_FAILURE[ ( expr ) ? 1 : -1 ] __attribute__((unused))
  #else
    #define KVS_STATIC_ASSERT( expr, mes ) static_assert( expr, mes )
  #endif
#elif defined KVS_COMPILER_VC
  #if KVS_COMPILER_VERSION_LESS_THAN( 10, 0 )
    #define KVS_STATIC_ASSERT( expr, mes ) typedef char KVS_STATIC_ASSERTION_FAILURE[ ( expr ) ? 1 : -1 ]
  #else
    #define KVS_STATIC_ASSERT( expr, mes ) static_assert( expr, mes )
  #endif
#else
  #error "Not Supported Compiler"
#endif
