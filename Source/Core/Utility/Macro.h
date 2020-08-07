/****************************************************************************/
/**
 *  @file   Macro.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <kvs/Compiler>

#define KVS_MACRO_FILE __FILE__
#define KVS_MACRO_LINE __LINE__
#define KVS_MACRO_DATE __DATE__
#define KVS_MACRO_TIME __TIME__

// Function name
#if defined ( KVS_COMPILER_VC )
#    if KVS_COMPILER_VERSION_LESS_THAN( 7, 0 )
#        define KVS_MACRO_FUNC "unknown"
#    else
#        define KVS_MACRO_FUNC __FUNCSIG__
#    endif
#elif defined ( KVS_COMPILER_GCC )
#    define KVS_MACRO_FUNC __PRETTY_FUNCTION__
#elif defined ( KVS_COMPILER_FCC )
#    define KVS_MACRO_FUNC "unknown"
#else
#    define KVS_MACRO_FUNC __func__
#endif

// Wrap multistatement macros in a do-while loop.
#define KVS_MACRO_MULTI_STATEMENT_BEGIN do {
#if defined ( KVS_COMPILER_VC )
#    define KVS_MACRO_MULTI_STATEMENT_END           \
        __pragma( warning( push ) )                 \
        __pragma( warning( disable: 4127 ) )        \
        } while ( false )                           \
        __pragma( warning( pop ) )
#else
#    define KVS_MACRO_MULTI_STATEMENT_END } while ( false )
#endif
