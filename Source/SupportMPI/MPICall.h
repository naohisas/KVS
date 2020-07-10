#pragma once
#include <kvs/Macro>
#include <kvs/Breakpoint>


namespace kvs
{

namespace mpi
{

namespace internal
{

bool HasError( const char* file, const int line, const char* func, const int err_code, const char* command );

} // end of namespace internal

} // end of namespace mpi

} // end of namespace kvs

/*===========================================================================*/
/**
 *  @def KVS_MPI_CALL( command )
 *  A macro for MPI command safe calling. An error checking will be done
 *  before and after calling the command when the debug option (KVS_ENABLE_DEBUG)
 *  is available.
 */
/*===========================================================================*/
#if defined( KVS_ENABLE_DEBUG )
#define KVS_MPI_CALL( command )                                         \
    if ( kvs::mpi::internal::HasError( KVS_MACRO_FILE, KVS_MACRO_LINE, KVS_MACRO_FUNC, command, #command ) ) { KVS_BREAKPOINT; }
#else
#define KVS_MPI_CALL( command ) ( command )
#endif // KVS_ENABLE_DEBUG
