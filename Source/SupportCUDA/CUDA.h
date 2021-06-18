/*****************************************************************************/
/**
 *  @file   CUDA.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once

#include <kvs/String>
#include <kvs/Compiler>
#include <cuda.h>

// Disable warning for CUDA runtime header.
#if defined( KVS_COMPILER_GCC )
#if ( defined(__clang__) && __clang_major__ >= 3 )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunneeded-internal-declaration"
#include <cuda_runtime.h>
#pragma GCC diagnostic pop
#else
#include <cuda_runtime.h>
#endif
#endif


namespace kvs
{

namespace cuda
{

/*===========================================================================*/
/**
 *  @brief  Returns CUDA description.
 *  @return description
 */
/*===========================================================================*/
inline std::string Description()
{
    const std::string description( "CUDA - Compute Unified Device Architecture" );
    return description;
}

/*===========================================================================*/
/**
 *  @brief  Returns CUDA version.
 *  @return CUDA version
 */
/*===========================================================================*/
inline std::string Version()
{
    const int major_version = CUDA_VERSION / 1000;
    const int minor_version = ( CUDA_VERSION - major_version * 1000 ) / 10;

    const std::string version(
        kvs::String::From( major_version ) + "." +
        kvs::String::From( minor_version ) );
    return version;
}

} // end of namespace cuda

} // end of namespace kvs
