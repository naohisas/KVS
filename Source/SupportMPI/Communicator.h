#pragma once
//#include <mpi.h>
#include <kvs/Type>
#include <kvs/ValueArray>
#include <kvs/Assert>
#include "MPI.h"
#include "MPICall.h"
#include "Operator.h"
#include "DataType.h"
#include "Request.h"


namespace kvs
{

namespace mpi
{

class Communicator
{
private:
    MPI_Comm m_handler; ///< communicator handler

public:
    Communicator( const MPI_Comm handler = MPI_COMM_WORLD );

    const MPI_Comm& handler() const { return m_handler; }
    int size() const;
    int rank() const;

    // Send

    template <typename T>
    void send( const int dst, const int tag, const T value );

    template <typename T>
    void send( const int dst, const int tag, const kvs::ValueArray<T>& values );

    template <typename T>
    void send( const int dst, const int tag, const T* values, const size_t size );

    template <typename T>
    //    MPI_Request immediateSend( const int dst, const int tag, const T value );
    kvs::mpi::Request immediateSend( const int dst, const int tag, const T value );

    template <typename T>
    //    MPI_Request immediateSend( const int dst, const int tag, const kvs::ValueArray<T>& values );
    kvs::mpi::Request immediateSend( const int dst, const int tag, const kvs::ValueArray<T>& values );

    template <typename T>
    //    MPI_Request immediateSend( const int dst, const int tag, const T* values, const size_t size );
    kvs::mpi::Request immediateSend( const int dst, const int tag, const T* values, const size_t size );

    // Receive

    template <typename T>
    MPI_Status receive( const int src, const int tag, T& value );

    template <typename T>
    MPI_Status receive( const int src, const int tag, kvs::ValueArray<T>& values );

    template <typename T>
    MPI_Status receive( const int src, const int tag, T* values, const size_t size );

    template <typename T>
    //    MPI_Request immediateReceive( const int src, const int tag, T& value );
    kvs::mpi::Request immediateReceive( const int src, const int tag, T& value );

    template <typename T>
    //    MPI_Request immediateReceive( const int src, const int tag, kvs::ValueArray<T>& values );
    kvs::mpi::Request immediateReceive( const int src, const int tag, kvs::ValueArray<T>& values );

    template <typename T>
    //    MPI_Request immediateReceive( const int src, const int tag, T* values, const size_t size );
    kvs::mpi::Request immediateReceive( const int src, const int tag, T* values, const size_t size );

    // Broadcast

    template <typename T>
    void broadcast( const int root, T& value );

    template <typename T>
    void broadcast( const int root, kvs::ValueArray<T>& values );

    template <typename T>
    void broadcast( const int root, T* values, const size_t size );

    // Scatter

    template <typename T>
    void scatter( const int root, const kvs::ValueArray<T>& send_values, T& recv_value );

    template <typename T>
    void scatter( const int root, const kvs::ValueArray<T>& send_values, kvs::ValueArray<T>& recv_values );

    template <typename T>
    void scatter( const int root, const T* send_values, const size_t send_size, T* recv_values, const size_t recv_size );

    // Gather

    template <typename T>
    void gather( const int root, const T& send_value, kvs::ValueArray<T>& recv_values );

    template <typename T>
    void gather( const int root, const kvs::ValueArray<T>& send_values, kvs::ValueArray<T>& recv_values );

    template <typename T>
    void gather( const int root, const T* send_values, const size_t send_size, T* recv_values, const size_t recv_size );

    // Reduce

    template <typename T, typename Op>
    void reduce( const int root, const T& send_value, T& recv_value, const Op op );

    template <typename T, typename Op>
    void reduce( const int root, const kvs::ValueArray<T>& send_value, kvs::ValueArray<T>& recv_value, const Op op );

    template <typename T>
    void reduce( const int root, const T& send_value, T& recv_value, const MPI_Op op );

    template <typename T>
    void reduce( const int root, const T* send_values, T* recv_values, const size_t size, const MPI_Op op );

    // All-gather

    template <typename T>
    void allGather( const T& send_value, kvs::ValueArray<T>& recv_values );

    template <typename T>
    void allGather( const kvs::ValueArray<T>& send_values, kvs::ValueArray<T>& recv_values );

    template <typename T>
    void allGather( const T* send_values, const size_t send_size, T* recv_values, const size_t recv_size );

    // All to All

    template <typename T>
    void allToAll( const kvs::ValueArray<T>& send_values, kvs::ValueArray<T>& recv_values );

    template <typename T>
    void allToAll( const T* send_values, const size_t send_size, T* recv_values, const size_t recv_size );

    // All-reduce

    template <typename T, typename Op>
    void allReduce( const T& send_value, T& recv_value, const Op op );

    template <typename T, typename Op>
    void allReduce( const kvs::ValueArray<T>& send_value, kvs::ValueArray<T>& recv_value, const Op op );

    template <typename T>
    void allReduce( const T& send_value, T& recv_value, const MPI_Op op );

    template <typename T>
    void allReduce( const T* send_values, T* recv_values, const size_t size, const MPI_Op op );
};

template <typename T>
inline void Communicator::send( const int dst, const int tag, const T value )
{
    this->send<T>( dst, tag, &value, 1 );
}

template <typename T>
inline void Communicator::send( const int dst, const int tag, const kvs::ValueArray<T>& values )
{
    this->send<T>( dst, tag, values.data(), values.size() );
}

template <typename T>
inline void Communicator::send( const int dst, const int tag, const T* values, const size_t size )
{
    const MPI_Datatype type = kvs::mpi::DataType<T>::Enum();
    KVS_MPI_CALL( MPI_Send( const_cast<T*>(values), static_cast<int>(size), type, dst, tag, m_handler ) );
}

template <typename T>
inline kvs::mpi::Request Communicator::immediateSend( const int dst, const int tag, const T value )
{
    return this->immediateSend<T>( dst, tag, &value, 1 );
}

template <typename T>
inline kvs::mpi::Request Communicator::immediateSend( const int dst, const int tag, const kvs::ValueArray<T>& values )
{
    return this->immediateSend<T>( dst, tag, values.data(), values.size() );
}

template <typename T>
inline kvs::mpi::Request Communicator::immediateSend( const int dst, const int tag, const T* values, const size_t size )
{
    MPI_Request request;
    const MPI_Datatype type = kvs::mpi::DataType<T>::Enum();
    KVS_MPI_CALL( MPI_Isend( const_cast<T*>(values), static_cast<int>(size), type, dst, tag, m_handler, &request ) );
    return kvs::mpi::Request( request );
}

template <typename T>
inline MPI_Status Communicator::receive( const int src, const int tag, T& value )
{
    return this->receive<T>( src, tag, &value, 1 );
}

template <typename T>
inline MPI_Status Communicator::receive( const int src, const int tag, kvs::ValueArray<T>& values )
{
    MPI_Status status;
    KVS_MPI_CALL( MPI_Probe( src, tag, m_handler, &status ) );

    int size = 0;
    const MPI_Datatype type = kvs::mpi::DataType<T>::Enum();
    KVS_MPI_CALL( MPI_Get_count( &status, type, &size ) );

    if ( size > static_cast<int>(values.size()) ) { values.allocate( size ); }
    return this->receive<T>( src, tag, values.data(), values.size() );
}

template <typename T>
inline MPI_Status Communicator::receive( const int src, const int tag, T* values, const size_t size )
{
    MPI_Status status;
    const MPI_Datatype type = kvs::mpi::DataType<T>::Enum();
    KVS_MPI_CALL( MPI_Recv( values, static_cast<int>(size), type, src, tag, m_handler, &status ) );
    return status;
}

template <typename T>
inline kvs::mpi::Request Communicator::immediateReceive( const int src, const int tag, T& value )
{
    return this->immediateReceive<T>( src, tag, &value, 1 );
}

template <typename T>
inline kvs::mpi::Request Communicator::immediateReceive( const int src, const int tag, kvs::ValueArray<T>& values )
{
    int flag;
    MPI_Status status;
    KVS_MPI_CALL( MPI_Iprobe( src, tag, m_handler, &flag, &status ) );

    int size = 0;
    const MPI_Datatype type = kvs::mpi::DataType<T>::Enum();
    KVS_MPI_CALL( MPI_Get_count( &status, type, &size ) );

    if ( size > static_cast<int>(values.size()) ) { values.allocate( size ); }
    return this->immediateReceive<T>( src, tag, values.data(), values.size() );
}

template <typename T>
inline kvs::mpi::Request Communicator::immediateReceive( const int src, const int tag, T* values, const size_t size )
{
    MPI_Request request;
    const MPI_Datatype type = kvs::mpi::DataType<T>::Enum();
    KVS_MPI_CALL( MPI_Irecv( values, static_cast<int>(size), type, src, tag, m_handler, &request ) );
    return kvs::mpi::Request( request );
}

template <typename T>
inline void Communicator::broadcast( const int root, T& value )
{
    this->broadcast<T>( root, &value, 1 );
}

template <typename T>
inline void Communicator::broadcast( const int root, kvs::ValueArray<T>& values )
{
    const int rank = this->rank();
    if ( rank == root )
    {
        // send
        kvs::UInt32 size = static_cast<kvs::UInt32>( values.size() );
        this->broadcast<kvs::UInt32>( root, size );
        this->broadcast<T>( root, values.data(), values.size() );
    }
    else
    {
        // recv
        kvs::UInt32 size = 0;
        this->broadcast<kvs::UInt32>( root, size );

        if ( size > values.size() ) { values.allocate( size ); }
        this->broadcast<T>( root, values.data(), values.size() );
    }
}

template <typename T>
inline void Communicator::broadcast( const int root, T* values, const size_t size )
{
    const MPI_Datatype type = kvs::mpi::DataType<T>::Enum();
    KVS_MPI_CALL( MPI_Bcast( values, static_cast<int>(size), type, root, m_handler ) );
}


template <typename T>
inline void Communicator::scatter( const int root, const kvs::ValueArray<T>& send_values, T& recv_value )
{
    KVS_ASSERT( send_values.size() / this->size() == 1 );

    const int scatter_size = 1;
    this->scatter<T>( root, send_values.data(), scatter_size, &recv_value, scatter_size );
}

template <typename T>
inline void Communicator::scatter( const int root, const kvs::ValueArray<T>& send_values, kvs::ValueArray<T>& recv_values )
{
    const kvs::UInt32 size = send_values.size() / this->size();
    if ( size > recv_values.size() ) { recv_values.allocate( size ); }

    const int rank = this->rank();
    if ( rank == root )
    {
        // send
        this->scatter<T>( root, send_values.data(), size, recv_values.data(), size );
    }
    else
    {
        // recv
        this->scatter<T>( root, NULL, 0, recv_values.data(), size );
    }
}

template <typename T>
inline void Communicator::scatter( const int root, const T* send_values, const size_t send_size, T* recv_values, const size_t recv_size )
{
    const MPI_Datatype type = kvs::mpi::DataType<T>::Enum();
    KVS_MPI_CALL( MPI_Scatter( const_cast<T*>(send_values), static_cast<int>(send_size), type, recv_values, static_cast<int>(recv_size), type, root, m_handler ) );
}

template <typename T>
inline void Communicator::gather( const int root, const T& send_value, kvs::ValueArray<T>& recv_values )
{
    const int rank = this->rank();
    if ( rank == root )
    {
        // recv
        const kvs::UInt32 size = this->size();
        if ( size > recv_values.size() ) { recv_values.allocate( size ); }

        const int gather_size = 1;
        this->gather<T>( root, &send_value, gather_size, recv_values.data(), gather_size );
    }
    else
    {
        // send
        const int gather_size = 1;
        this->gather<T>( root, &send_value, gather_size, NULL, 0 );
    }
}

template <typename T>
inline void Communicator::gather( const int root, const kvs::ValueArray<T>& send_values, kvs::ValueArray<T>& recv_values )
{
    const int rank = this->rank();
    if ( rank == root )
    {
        // recv
        const kvs::UInt32 size = this->size() * send_values.size();
        if ( size > recv_values.size() ) { recv_values.allocate( size ); }

        const int gather_size = send_values.size();
        this->gather<T>( root, send_values.data(), gather_size, recv_values.data(), gather_size );
    }
    else
    {
        // send
        const int gather_size = send_values.size();
        this->gather<T>( root, send_values.data(), gather_size, NULL, 0 );
    }
}

template <typename T>
inline void Communicator::gather( const int root, const T* send_values, const size_t send_size, T* recv_values, const size_t recv_size )
{
    const MPI_Datatype type = kvs::mpi::DataType<T>::Enum();
    KVS_MPI_CALL( MPI_Gather( const_cast<T*>(send_values), static_cast<int>(send_size), type, recv_values, static_cast<int>(recv_size), type, root, m_handler ) );
}

template <typename T, typename Op>
inline void Communicator::reduce( const int root, const T& send_value, T& recv_value, const Op op )
{
    MPI_Op type = OperatorType<Op,T>::Enum();
    this->reduce<T>( root, &send_value, &recv_value, 1, type );
}

template <typename T, typename Op>
inline void Communicator::reduce( const int root, const kvs::ValueArray<T>& send_values, kvs::ValueArray<T>& recv_values, const Op op )
{
    const int rank = this->rank();
    if ( rank == root )
    {
        // recv
        const kvs::UInt32 size = send_values.size();
        if ( size > recv_values.size() ) { recv_values.allocate( size ); }

        const MPI_Op mpi_op = OperatorType<Op,T>::Enum();
        this->reduce<T>( root, send_values.data(), recv_values.data(), send_values.size(), mpi_op );
    }
    else
    {
        // send
        const MPI_Op mpi_op = OperatorType<Op,T>::Enum();
        this->reduce<T>( root, send_values.data(), NULL, send_values.size(), mpi_op );
    }
}

template <typename T>
inline void Communicator::reduce( const int root, const T& send_value, T& recv_value, const MPI_Op op )
{
    this->reduce<T>( root, &send_value, &recv_value, 1, op );
}

template <typename T>
inline void Communicator::reduce( const int root, const T* send_values, T* recv_values, const size_t size, const MPI_Op op )
{
    const MPI_Datatype type = kvs::mpi::DataType<T>::Enum();
    KVS_MPI_CALL( MPI_Reduce( const_cast<T*>(send_values), recv_values, static_cast<int>(size), type, op, root, m_handler ) );
}

template <typename T>
inline void Communicator::allGather( const T& send_value, kvs::ValueArray<T>& recv_values )
{
    const kvs::UInt32 size = this->size();
    if ( size > recv_values.size() ) { recv_values.allocate( size ); }

    const int gather_size = 1;
    this->allGather<T>( &send_value, gather_size, recv_values.data(), gather_size );
}

template <typename T>
inline void Communicator::allGather( const kvs::ValueArray<T>& send_values, kvs::ValueArray<T>& recv_values )
{
    const kvs::UInt32 size = this->size() * send_values.size();
    if ( size > recv_values.size() ) { recv_values.allocate( size ); }

    const int gather_size = send_values.size();
    this->allGather<T>( send_values.data(), gather_size, recv_values.data(), gather_size );
}

template <typename T>
inline void Communicator::allGather( const T* send_values, const size_t send_size, T* recv_values, const size_t recv_size )
{
    const MPI_Datatype type = kvs::mpi::DataType<T>::Enum();
    KVS_MPI_CALL( MPI_Allgather( const_cast<T*>(send_values), static_cast<int>(send_size), type, recv_values, static_cast<int>(recv_size), type, m_handler ) );
}

template <typename T>
inline void Communicator::allToAll( const kvs::ValueArray<T>& send_values, kvs::ValueArray<T>& recv_values )
{
    const int send_size = 1;
    KVS_ASSERT( send_values.size() / send_size == this->size() );

    const kvs::UInt32 size = send_size * this->size();
    if ( size > recv_values.size() ) { recv_values.allocate( size ); }

    const int alltoall_size = send_size;
    this->allToAll<T>( send_values.data(), alltoall_size, recv_values.data(), alltoall_size );
}

template <typename T>
inline void Communicator::allToAll( const T* send_values, const size_t send_size, T* recv_values, const size_t recv_size )
{
    const MPI_Datatype type = kvs::mpi::DataType<T>::Enum();
    KVS_MPI_CALL( MPI_Alltoall( const_cast<T*>(send_values), static_cast<int>(send_size), type, recv_values, static_cast<int>(recv_size), type, m_handler ) );
}

template <typename T, typename Op>
inline void Communicator::allReduce( const T& send_value, T& recv_value, const Op op )
{
    MPI_Op type = OperatorType<Op,T>::Enum();
    this->allReduce<T>( &send_value, &recv_value, 1, type );
}

template <typename T, typename Op>
inline void Communicator::allReduce( const kvs::ValueArray<T>& send_values, kvs::ValueArray<T>& recv_values, const Op op )
{
    const kvs::UInt32 size = send_values.size();
    if ( size > recv_values.size() ) { recv_values.allocate( size ); }

    const MPI_Op mpi_op = OperatorType<Op,T>::Enum();
    this->allReduce<T>( send_values.data(), recv_values.data(), send_values.size(), mpi_op );
}

template <typename T>
inline void Communicator::allReduce( const T& send_value, T& recv_value, const MPI_Op op )
{
    this->allReduce<T>( &send_value, &recv_value, 1, op );
}

template <typename T>
inline void Communicator::allReduce( const T* send_values, T* recv_values, const size_t size, const MPI_Op op )
{
    const MPI_Datatype type = kvs::mpi::DataType<T>::Enum();
    KVS_MPI_CALL( MPI_Allreduce( const_cast<T*>(send_values), recv_values, static_cast<int>(size), type, op, m_handler ) );
}

} // end of namespace mpi

} // end of namespace kvs
