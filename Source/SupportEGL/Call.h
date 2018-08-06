#pragma once
#include <kvs/Macro>
#include <kvs/Breakpoint>
#include <EGL/egl.h>


namespace kvs
{

namespace egl
{

namespace internal
{

bool HasError( const char* file, const int line, const char* func, const char* command );

} // end of namesapce internal

} // end of namespace egl

} // end of namespace kvs


#if defined( KVS_ENABLE_DEBUG )
#define KVS_EGL_CALL( command )                                          \
    KVS_MACRO_MULTI_STATEMENT_BEGIN                                     \
    if ( kvs::egl::internal::HasError( KVS_MACRO_FILE, KVS_MACRO_LINE, KVS_MACRO_FUNC, "Unknown" ) ) { KVS_BREAKPOINT; } \
    command;                                                            \
    if ( kvs::egl::internal::HasError( KVS_MACRO_FILE, KVS_MACRO_LINE, KVS_MACRO_FUNC, #command ) ) { KVS_BREAKPOINT; } \
    KVS_MACRO_MULTI_STATEMENT_END
#else
#define KVS_EGL_CALL( command ) ( command )
#endif // KVS_ENABLE_DEBUG
