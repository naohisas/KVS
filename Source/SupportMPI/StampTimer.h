/*****************************************************************************/
/**
 *  @file   StampTimer.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/StampTimer>
#include <kvs/mpi/Communicator>
#include <algorithm>


namespace kvs
{

namespace mpi
{

/*===========================================================================*/
/**
 *  @brief  Extended stamp timer class for parallel processing.
 */
/*===========================================================================*/
class StampTimer : public kvs::StampTimer
{
public:
    using BaseClass = kvs::StampTimer;

private:
    kvs::mpi::Communicator m_comm; ///< MPI communicator

public:
    StampTimer( kvs::mpi::Communicator& comm ): m_comm( comm ) {}
    StampTimer( kvs::mpi::Communicator& comm, const kvs::StampTimer& timer ):
        BaseClass( timer ),
        m_comm( comm ) {}
    StampTimer( kvs::mpi::Communicator& comm, const std::string& title ):
        BaseClass( title ),
        m_comm( comm ) {}

    void reduceMin() { this->reduce( MPI_MIN ); }
    void reduceMax() { this->reduce( MPI_MAX ); }

    void reduceAve()
    {
        this->reduce( MPI_SUM );
        const auto& begin = BaseClass::times().begin();
        const auto& end = BaseClass::times().end();
        const auto size = m_comm.size();
        std::for_each( begin, end, [&] ( float& v ) { v /= size; } );
    }

    void reduce( const MPI_Op op )
    {
        Times times( BaseClass::numberOfStamps() );
        m_comm.allReduce( &(BaseClass::times()[0]), &times[0], times.size(), op );
        times.swap( BaseClass::times() );
    }
};

} // end of namespace mpi

} // end of namespace kvs
