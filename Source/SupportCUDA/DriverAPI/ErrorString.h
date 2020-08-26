/*****************************************************************************/
/**
 *  @file   ErrorString.h
 *  @author Naohisa Sakamoto
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
