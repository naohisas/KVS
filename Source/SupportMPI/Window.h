#pragma once
#include "MPI.h"
#include "MPICall.h"
#include "Communicator.h"
#include "DataType.h"
#include <kvs/ValueArray>
#include <kvs/Message>


namespace kvs
{

namespace mpi
{

template <typename T>
class Window
{
private:
    MPI_Win m_handler; ///< window handler
    kvs::ValueArray<T> m_buffer; ///< buffer data to expose

public:
    Window( const kvs::mpi::Communicator& comm, const size_t size );
    Window( const kvs::mpi::Communicator& comm, const kvs::ValueArray<T>& buffer );
    Window(): m_handler( MPI_WIN_NULL ) {}
    ~Window() { this->free(); }

    const MPI_Win& handler() const { return m_handler; }
    const kvs::ValueArray<T>& buffer() const { return m_buffer; }

    void create( const kvs::mpi::Communicator& comm, const kvs::ValueArray<T>& buffer );
    void allocate( const kvs::mpi::Communicator& comm, const size_t size );
    void free();

    void get( T& value, const int rank, const size_t offset = 0 );
    void get( kvs::ValueArray<T>& values, const int rank, const size_t offset = 0 );
    void put( const T value, const int rank, const size_t offset = 0 );
    void put( const kvs::ValueArray<T>& values, const int rank, const size_t offset = 0 );
    void accumulate( const T value, const MPI_Op op, const int rank, const size_t offset = 0 );
    void accumulate( const kvs::ValueArray<T>& values, const MPI_Op op, const int rank, const size_t offset = 0 );

    void fence( const int assert = 0 );
    void lock( const int lock_type, const int rank, const int assert = 0 );
    void lockAll( const int assert = 0 );
    void unlock( const int rank );
    void unlockAll();

    void sync();
    void flush( const int rank );
    void flushAll();
    void flushLocal( const int rank );
    void flushLocalAll();
};

template <typename T>
inline Window<T>::Window( const kvs::mpi::Communicator& comm, const size_t size ):
    m_handler( MPI_WIN_NULL )
{
    this->allocate( comm, size );
}

template <typename T>
inline Window<T>::Window( const kvs::mpi::Communicator& comm, const kvs::ValueArray<T>& buffer ):
    m_handler( MPI_WIN_NULL )
{
    this->create( comm, buffer );
}

template <typename T>
inline void Window<T>::create( const kvs::mpi::Communicator& comm, const kvs::ValueArray<T>& buffer )
{
    m_buffer = buffer;
    KVS_MPI_CALL( MPI_Win_create( m_buffer.data(), buffer.byteSize(), sizeof(T), MPI_INFO_NULL, comm.handler(), &this->m_handler ) );
}

template <typename T>
inline void Window<T>::allocate( const kvs::mpi::Communicator& comm, const size_t size )
{
    T* values = NULL;
#if MPI_VERSION >= 3
    KVS_MPI_CALL( MPI_Win_allocate( sizeof(T) * size, sizeof(T), MPI_INFO_NULL, comm.handler(), values, &this->m_handler ) );
#else
    KVS_MPI_CALL( MPI_Alloc_mem( sizeof(T) * size, MPI_INFO_NULL, values ) );
    KVS_MPI_CALL( MPI_Win_create( values, sizeof(T) * size, sizeof(T), MPI_INFO_NULL, comm.handler(), &this->m_handler ) );
#endif
    m_buffer = kvs::ValueArray<T>( kvs::SharedPointer<T>( values ), size );
}

template <typename T>
inline void Window<T>::free()
{
    if ( m_handler != MPI_WIN_NULL )
    {
#if MPI_VERSION >= 3
        KVS_MPI_CALL( MPI_Win_free( &this->m_handler ) );
#else
        KVS_MPI_CALL( MPI_Win_free( &this->m_handler ) );
        KVS_MPI_CALL( MPI_Free_mem( this->m_buffer.data() ) );
#endif
    }
}

template <typename T>
inline void Window<T>::get( T& value, const int rank, const size_t offset )
{
    const MPI_Datatype data_type = kvs::mpi::DataType<T>::Enum();
    KVS_MPI_CALL( MPI_Get( &value, 1, data_type, rank, offset, 1, data_type, this->m_handler ) );
}

template <typename T>
inline void Window<T>::get( kvs::ValueArray<T>& values, const int rank, const size_t offset )
{
    const MPI_Datatype data_type = kvs::mpi::DataType<T>::Enum();
    const int data_size = static_cast<int>( values.size() );
    KVS_MPI_CALL( MPI_Get( values.data(), data_size, data_type, rank, offset, data_size, data_type, this->m_handler ) );
}

template <typename T>
inline void Window<T>::put( const T value, const int rank, const size_t offset )
{
    const MPI_Datatype data_type = kvs::mpi::DataType<T>::Enum();
    KVS_MPI_CALL( MPI_Put( (void*)&value, 1, data_type, rank, offset, 1, data_type, this->m_handler ) );
}

template <typename T>
inline void Window<T>::put( const kvs::ValueArray<T>& values, const int rank, const size_t offset )
{
    const MPI_Datatype data_type = kvs::mpi::DataType<T>::Enum();
    const int data_size = static_cast<int>( values.size() );
    KVS_MPI_CALL( MPI_Put( (void*)values.data(), data_size, data_type, rank, offset, data_size, data_type, this->m_handler ) );
}

template <typename T>
inline void Window<T>::accumulate( const T value, const MPI_Op op, const int rank, const size_t offset )
{
    const MPI_Datatype data_type = kvs::mpi::DataType<T>::Enum();
    KVS_MPI_CALL( MPI_Accumulate( (void*)&value, 1, data_type, rank, offset, 1, data_type, op, this->m_handler ) );
}

template <typename T>
inline void Window<T>::accumulate( const kvs::ValueArray<T>& values, const MPI_Op op, const int rank, const size_t offset )
{
    const MPI_Datatype data_type = kvs::mpi::DataType<T>::Enum();
    const int data_size = static_cast<int>( values.size() );
    KVS_MPI_CALL( MPI_Accumulate( (void*)values.data(), data_size, data_type, rank, offset, data_size, data_type, op, this->m_handler ) );
}

template <typename T>
inline void Window<T>::fence( const int assert )
{
    KVS_MPI_CALL( MPI_Win_fence( assert, this->m_handler ) );
}

template <typename T>
inline void Window<T>::lock( const int lock_type, const int rank, const int assert )
{
    KVS_MPI_CALL( MPI_Win_lock( lock_type, rank, assert, this->m_handler ) );
}

template <typename T>
inline void Window<T>::lockAll( const int assert )
{
#if MPI_VERSION >= 3
    KVS_MPI_CALL( MPI_Win_lock_all( assert, this->m_handler ) );
#else
    kvsMessageWarning( "MPI_Win_lock_all is not supported." );
#endif
}

template <typename T>
inline void Window<T>::unlock( const int rank )
{
    KVS_MPI_CALL( MPI_Win_unlock( rank, this->m_handler ) );
}

template <typename T>
inline void Window<T>::unlockAll()
{
#if MPI_VERSION >= 3
    KVS_MPI_CALL( MPI_Win_unlock_all( this->m_handler ) );
#else
    kvsMessageWarning( "MPI_Win_unlock_all is not supported." );
#endif
}

template <typename T>
inline void Window<T>::sync()
{
#if MPI_VERSION >= 3
    KVS_MPI_CALL( MPI_Win_sync( this->m_handler ) );
#else
    kvsMessageWarning( "MPI_Win_sync is not supported." );
#endif
}

template <typename T>
inline void Window<T>::flush( const int rank )
{
#if MPI_VERSION >= 3
    KVS_MPI_CALL( MPI_Win_flush( rank, this->m_handler ) );
#else
    KVS_MPI_CALL( MPI_Win_unlock( rank, this->m_handler ) );
#endif
}

template <typename T>
inline void Window<T>::flushAll()
{
#if MPI_VERSION >= 3
    KVS_MPI_CALL( MPI_Win_flush_all( this->m_handler ) );
#else
    kvsMessageWarning( "MPI_Win_flush_all is not supported." );
#endif
}

template <typename T>
inline void Window<T>::flushLocal( const int rank )
{
#if MPI_VERSION >= 3
    KVS_MPI_CALL( MPI_Win_flush_local( rank, this->m_handler ) );
#else
    kvsMessageWarning( "MPI_Win_flush_local is not supported." );
#endif
}

template <typename T>
inline void Window<T>::flushLocalAll()
{
#if MPI_VERSION >= 3
    KVS_MPI_CALL( MPI_Win_flush_local_all( this->m_handler ) );
#else
    kvsMessageWarning( "MPI_Win_flush_local_all is not supported." );
#endif
}

} // end of namespace mpi

} // end of namespace kvs
