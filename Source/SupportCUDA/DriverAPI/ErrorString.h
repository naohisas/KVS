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
 *  $Id$
 */
/*****************************************************************************/
#pragma once
#include "../CUDA.h"


namespace kvs
{

namespace cuda
{

namespace DriverAPI
{

const char* ErrorString( const CUresult error );
CUresult LastError();
CUresult PeekAtLastError();
bool HasError();

} // end of namespace DriverAPI

} // end of namespace cuda

} // end of namespace kvs
