/*****************************************************************************/
/**
 *  @file   DriverAPI.h
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
#include "Context.h"
#include "Device.h"
#include "ErrorString.h"
#include "Initialization.h"

namespace kvs
{

namespace cuda
{

namespace DriverAPI
{

namespace internal
{

void SetLastError( const CUresult error );
CUresult GetLastError();
CUresult PeekAtLastError();
bool HasError( const char* file, const int line, const char* func, const char* command );
bool IsError( const CUresult error, const char* file, const int line, const char* func, const char* command );

} // end of namespace internal

} // end of namespace DriverAPI

} // end of namespace cuda

} // end of namespace kvs

#if defined KVS_ENABLE_DEBUG
#define KVS_CU_CALL( command )                                          \
    KVS_MACRO_MULTI_STATEMENT_BEGIN                                     \
    if ( kvs::cuda::DriverAPI::internal::HasError( KVS_MACRO_FILE, KVS_MACRO_LINE, KVS_MACRO_FUNC, "Unknown" ) ) { KVS_BREAKPOINT; } \
    CUresult error = command; kvs::cuda::DriverAPI::internal::SetLastError( error ); \
    if ( kvs::cuda::DriverAPI::internal::IsError( error, KVS_MACRO_FILE, KVS_MACRO_LINE, KVS_MACRO_FUNC, #command ) ) { KVS_BREAKPOINT; } \
    KVS_MACRO_MULTI_STATEMENT_END
#else
#define KVS_CU_CALL( command ) ( kvs::cuda::DriverAPI::internal::SetLastError( command ) )
#endif
