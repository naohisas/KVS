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
#include "DriverAPI.h"


namespace kvs
{

namespace cuda
{

namespace DriverAPI
{

/*===========================================================================*/
/**
 *  @brief  Returns error string.
 *  @param  error [in] CUDA error code
 *  @return error string
 */
/*===========================================================================*/
const char* ErrorString( const CUresult error )
{
    switch ( error )
    {
    case CUDA_SUCCESS:                 return "Success";
    case CUDA_ERROR_INVALID_VALUE:     return "Invalid value";
    case CUDA_ERROR_OUT_OF_MEMORY:     return "Out of memory";
    case CUDA_ERROR_NOT_INITIALIZED:   return "Not initialized";
#if CUDA_VERSION >= 2000
    case CUDA_ERROR_DEINITIALIZED:     return "Deinitialized";
#endif
    case CUDA_ERROR_NO_DEVICE:         return "No device";
    case CUDA_ERROR_INVALID_DEVICE:    return "Invalid device";
    case CUDA_ERROR_INVALID_IMAGE:     return "Invalid image";
    case CUDA_ERROR_INVALID_CONTEXT:   return "Invalid context";
    case CUDA_ERROR_CONTEXT_ALREADY_CURRENT: return "Context already current";
    case CUDA_ERROR_MAP_FAILED:        return "Map failed";
    case CUDA_ERROR_UNMAP_FAILED:      return "Unmap failed";
    case CUDA_ERROR_ARRAY_IS_MAPPED:   return "Array is mapped";
    case CUDA_ERROR_ALREADY_MAPPED:    return "Already mapped";
    case CUDA_ERROR_NO_BINARY_FOR_GPU: return "No binary for gpu";
    case CUDA_ERROR_ALREADY_ACQUIRED:  return "Already acquired";
    case CUDA_ERROR_NOT_MAPPED:        return "Not mapped";
    case CUDA_ERROR_INVALID_SOURCE:    return "Invalid source";
    case CUDA_ERROR_FILE_NOT_FOUND:    return "File not found";
    case CUDA_ERROR_INVALID_HANDLE:    return "Invalid handle";
    case CUDA_ERROR_NOT_FOUND:         return "Not found";
    case CUDA_ERROR_NOT_READY:         return "Not ready";
    case CUDA_ERROR_LAUNCH_FAILED:     return "Launch failed";
    case CUDA_ERROR_LAUNCH_OUT_OF_RESOURCES: return "Launch out of resources";
    case CUDA_ERROR_LAUNCH_TIMEOUT:    return "Launch timeout";
    case CUDA_ERROR_LAUNCH_INCOMPATIBLE_TEXTURING: return "Launch incompatible texturing";
    case CUDA_ERROR_UNKNOWN:           return "Unknown";
    default: return "Invalid error code";
    }
}

CUresult LastError()
{
    return internal::GetLastError();
}

CUresult PeekAtLastError()
{
    return internal::PeekAtLastError();
}

bool HasError()
{
    return PeekAtLastError() != CUDA_SUCCESS;
}

} // end of namespace DriverAPI

} // end of namespace cuda

} // end of namespace kvs
