/*****************************************************************************/
/**
 *  @file   RuntimeAPI.h
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
#pragma once
#include "../CUDA.h"
#include <kvs/Macro>
#include <kvs/Breakpoint>
#include "Device.h"
#include "ErrorString.h"


namespace kvs
{

namespace cuda
{

namespace RuntimeAPI
{

namespace internal
{

bool HasError( const char* file, const int line, const char* func, const char* command );
bool IsError( const cudaError_t error, const char* file, const int line, const char* func, const char* command );

} // end of namespace internal

} // end of namespace RuntimeAPI

} // end of namespace cuda

} // end of namespace kvs

#if defined KVS_ENABLE_DEBUG
#define KVS_CUDA_CALL( command )                                        \
    KVS_MACRO_MULTI_STATEMENT_BEGIN                                     \
    if ( kvs::cuda::RuntimeAPI::internal::HasError( KVS_MACRO_FILE, KVS_MACRO_LINE, KVS_MACRO_FUNC, "Unknown" ) ) { KVS_BREAKPOINT; } \
    cudaError_t error = command;                                        \
    if ( kvs::cuda::RuntimeAPI::internal::IsError( error, KVS_MACRO_FILE, KVS_MACRO_LINE, KVS_MACRO_FUNC, #command ) ) { KVS_BREAKPOINT; } \
    KVS_MACRO_MULTI_STATEMENT_END
#else
#define KVS_CUDA_CALL( command ) ( command )
#endif
