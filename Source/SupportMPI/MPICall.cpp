/*****************************************************************************/
/**
 *  @file   MPICall.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "MPICall.h"
#include "MPI.h"
#include <string>
#include <kvs/Message>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Returns a string for a given error code
 *  @param  err_code [in] error code
 *  @return text that corresponds to the error code
 */
/*===========================================================================*/
inline std::string GetErrorString( int err_code )
{
    char err_msg[ MPI_MAX_ERROR_STRING ] = {0};
    int err_msg_len = 0;
    if ( MPI_Error_string( err_code, err_msg, &err_msg_len ) != MPI_SUCCESS )
    {
        return std::string( "Invalid error code" );
    }

    return std::string( err_msg, err_msg_len );
}

}

namespace kvs
{

namespace mpi
{

namespace internal
{

/*===========================================================================*/
/**
 *  @brief  Returns true if an error was occurred.
 *  @param  file [in] file name
 *  @param  line [in] line number
 *  @param  func [in] function name
 *  @param  err_code [in] MPI error code
 *  @param  command [in] MPI command
 *  @return true if the specified command couldn't be successfully executed.
 */
/*===========================================================================*/
bool HasError( const char* file, const int line, const char* func, const int err_code, const char* command )
{
    if ( err_code == MPI_SUCCESS ) { return false; }

    // Output message tag.
    std::cerr << kvs::ColorStream::Bold
              << kvs::ColorStream::Red
              << "KVS MPI ERROR"
              << kvs::ColorStream::Reset
              << ": ";

    // Output message with an error string.
    const std::string message = ::GetErrorString( err_code );
    std::cerr << kvs::ColorStream::Underline( kvs::ColorStream::Bold( message ) ) << std::endl;
    std::cerr << "    " << "Func: " << func << std::endl;
    std::cerr << "    " << "File: " << file << ":" << line << std::endl;
    std::cerr << "    " << "MPI Command: " << command << std::endl;

    return true;
}

} // end of namespace internal

} // end of namespace mpi

} // end of namespace kvs
