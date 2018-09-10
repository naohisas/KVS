/*****************************************************************************/
/**
 *  @file   DriverAPI.cpp
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
#include "DriverAPI.h"
#include "ErrorString.h"
#include <string>
#include <kvs/Message>


namespace { CUresult LastError = CUDA_SUCCESS; }

namespace kvs
{

namespace cuda
{

namespace DriverAPI
{

namespace internal
{

void SetLastError( const CUresult error )
{
    ::LastError = error;
}

CUresult GetLastError()
{
    CUresult error = ::LastError;
    ::LastError = CUDA_SUCCESS;
    return error;
}

CUresult PeekAtLastError()
{
    return ::LastError;
}

bool HasError( const char* file, const int line, const char* func, const char* command )
{
    CUresult error = PeekAtLastError();
    return IsError( error, file, line, func, command );
}

bool IsError( const CUresult error, const char* file, const int line, const char* func, const char* command )
{
    if ( error == CUDA_SUCCESS ) return false;

    // Output message tag.
    std::cerr << kvs::ColorStream::Bold
              << kvs::ColorStream::Red
              << "KVS CUDA ERROR"
              << kvs::ColorStream::Reset
              << ": ";

    // Output message with an error string.
    std::string message;
    const char* c = kvs::cuda::DriverAPI::ErrorString( error );
    while ( *c ) message += *c++;
    std::cerr << kvs::ColorStream::Underline( kvs::ColorStream::Bold( message ) ) << std::endl;
    std::cerr << "    " << "Func: " << func << std::endl;
    std::cerr << "    " << "File: " << file << ":" << line << std::endl;
    std::cerr << "    " << "CUDA Command: " << command << std::endl;

    return true;
}

} // end of namespace internal

} // end of namespace DriverAPI

} // end of namespace cuda

} // end of namespace kvs
