/*****************************************************************************/
/**
 *  @file   CUDA.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: CUDA.h 1698 2014-01-16 10:49:03Z naohisa.sakamoto@gmail.com $
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
        kvs::String::ToString( major_version ) + "." +
        kvs::String::ToString( minor_version ) );
    return version;
}

} // end of namespace cuda

} // end of namespace kvs
