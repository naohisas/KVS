/*****************************************************************************/
/**
 *  @file   RuntimeAPI.cpp
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
#include "RuntimeAPI.h"
#include "ErrorString.h"
#include <string>
#include <kvs/Message>


namespace kvs
{

namespace cuda
{

namespace RuntimeAPI
{

namespace internal
{

bool HasError( const char* file, const int line, const char* func, const char* command )
{
    cudaError_t error = kvs::cuda::RuntimeAPI::PeekAtLastError();
    return IsError( error, file, line, func, command );
}

bool IsError( const cudaError_t error, const char* file, const int line, const char* func, const char* command )
{
    if ( error == cudaSuccess ) return false;

    // Output message tag.
    std::cerr << kvs::ColorStream::Bold
              << kvs::ColorStream::Red
              << "KVS CUDA ERROR"
              << kvs::ColorStream::Reset
              << ": ";

    // Output message with an error string.
    std::string message;
    const char* c = kvs::cuda::RuntimeAPI::ErrorString( error );
    while ( *c ) message += *c++;
    std::cerr << kvs::ColorStream::Underline( kvs::ColorStream::Bold( message ) ) << std::endl;
    std::cerr << "    " << "Func: " << func << std::endl;
    std::cerr << "    " << "File: " << file << ":" << line << std::endl;
    std::cerr << "    " << "CUDA Command: " << command << std::endl;

    return true;
}

} // end of namespace internal

} // end of namespace RuntimeAPI

} // end of namespace cuda

} // end of namespace kvs
