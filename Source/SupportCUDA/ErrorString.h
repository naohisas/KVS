/*****************************************************************************/
/**
 *  @file   ErrorString.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ErrorString.h 1666 2013-12-02 17:21:40Z naohisa.sakamoto@gmail.com $
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
