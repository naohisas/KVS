/*****************************************************************************/
/**
 *  @file   Environment.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "Environment.h"
#include "MPI.h"
#include "MPICall.h"


namespace kvs
{

namespace mpi
{

Environment::Environment( int argc, char** argv )
{
    KVS_MPI_CALL( MPI_Init( &argc, &argv ) );
}

Environment::~Environment()
{
    KVS_MPI_CALL( MPI_Finalize() );
}

} // end of namespace mpi

} // end of namespace kvs
