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
#include "ErrorString.h"


namespace kvs
{

namespace cuda
{

namespace RuntimeAPI
{

/*===========================================================================*/
/**
 *  @brief  Returns the error message from an error code.
 *  @param  error [in] CUDA error code
 *  @return error message
 */
/*===========================================================================*/
const char* ErrorString( const cudaError_t error )
{
    return cudaGetErrorString( error );
}

/*===========================================================================*/
/**
 *  @brief  Returns the last error code from a runtime call.
 *  @return error code
 */
/*===========================================================================*/
cudaError_t LastError()
{
    return cudaGetLastError();
}

/*===========================================================================*/
/**
 *  @brief  Returns the last error code from a runtime call.
 *  @return error code
 */
/*===========================================================================*/
cudaError_t PeekAtLastError()
{
    return cudaPeekAtLastError();
}

bool HasError()
{
    return PeekAtLastError() != cudaSuccess;
}

} // end of namespace DriverAPI

} // end of namespace cuda

} // end of namespace kvs
