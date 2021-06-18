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

namespace RuntimeAPI
{

const char* ErrorString( const cudaError_t error );
cudaError_t LastError();
cudaError_t PeekAtLastError();
bool HasError();

} // end of namespace DriverAPI

} // end of namespace cuda

} // end of namespace kvs
