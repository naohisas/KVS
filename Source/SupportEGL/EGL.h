#pragma once
#include "Call.h"
#include <kvs/String>
#include <string>


namespace kvs
{

namespace egl
{

inline std::string Description()
{
    const std::string description( "EGL - An interface library between OpenGL and the native windowing system." );
    return description;
}

inline std::string Version()
{
#if defined( EGL_VERSION_1_5 )
    const std::string version( "1.5" );
#else
#if defined( EGL_VERSION_1_4 )
    const std::string version( "1.4" );
#else
#if defined( EGL_VERSION_1_3 )
    const std::string version( "1.3" );
#else
#if defined( EGL_VERSION_1_2 )
    const std::string version( "1.2" );
#else
#if defined( EGL_VERSION_1_1 )
    const std::string version( "1.1" );
#else
#if defined( EGL_VERSION_1_0 )
    const std::string version( "1.0" );
#else
    const std::string version( "Unknown" );
#endif // EGL_VERSION_1_0
#endif // EGL_VERSION_1_1
#endif // EGL_VERSION_1_2
#endif // EGL_VERSION_1_3
#endif // EGL_VERSION_1_4
#endif // EGL_VERSION_1_5
    return version;
}

} // end of namespace osmesa

} // end of namespace kvs
