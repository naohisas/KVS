#include "Communicator.h"
#include "MPICall.h"
#include <kvs/Type>


namespace kvs
{

namespace mpi
{

Communicator::Communicator( const MPI_Comm handler, const int root ):
    m_handler( handler ),
    m_root( root )
{
}

int Communicator::size() const
{
    int size;
    KVS_MPI_CALL( MPI_Comm_size( m_handler, &size ) );
    return size;
}

int Communicator::rank() const
{
    int rank;
    KVS_MPI_CALL( MPI_Comm_rank( m_handler, &rank ) );
    return rank;
}

} // end of namespace mpi

} // end of namespace kvs
