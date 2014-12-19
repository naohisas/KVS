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
    std::string message_tag( "KVS CUDA ERROR" );
    std::cerr << KVS_MESSAGE_SET_COLOR( KVS_MESSAGE_RED );
    std::cerr << message_tag;
    std::cerr << KVS_MESSAGE_RESET_COLOR;

    // Output message with an error string.
    std::string error_string;
    const char* c = kvs::cuda::RuntimeAPI::ErrorString( error );
    while ( *c ) error_string += *c++;
    std::cerr << ": " << error_string << std::endl;
    std::cerr << "\t" << "FILE: " << file << " (" << line << ")" << std::endl;
    std::cerr << "\t" << "FUNC: " << func << std::endl;
    std::cerr << "\t" << "CUDA COMMAND: " << command << std::endl;

    return true;
}

} // end of namespace internal

} // end of namespace RuntimeAPI

} // end of namespace cuda

} // end of namespace kvs
