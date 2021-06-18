/*****************************************************************************/
/**
 *  @file   ErrorString.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/Deprecated>
#include "RuntimeAPI/ErrorString.h"


namespace kvs
{

namespace cuda
{

// KVS_DEPRECATED
inline const char* ErrorString( const CUresult error ) { return kvs::cuda::RuntimeAPI::ErrorString( error ); }

} // end of namespace cuda

} // end of namespace kvs
