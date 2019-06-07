#pragma once
#include <string>
#include <kvs/String>
#include <GL/osmesa.h>


namespace kvs
{

namespace osmesa
{

inline std::string Description()
{
    const std::string description( "OSMesa - Off-screen Rendering Framework based on Mesa" );
    return( description );
}

inline std::string Version()
{
    const int major_version = OSMESA_MAJOR_VERSION;
    const int minor_version = OSMESA_MINOR_VERSION;
    const int patch_version = OSMESA_PATCH_VERSION;

    const std::string version(
        kvs::String::ToString( major_version ) + "." +
        kvs::String::ToString( minor_version ) + "." +
        kvs::String::ToString( patch_version ) );
    return version;
}

} // end of namespace osmesa

} // end of namespace kvs
