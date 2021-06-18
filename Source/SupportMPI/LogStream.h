/*****************************************************************************/
/**
 *  @file   LogStream.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <kvs/LogStream>
#include "Communicator.h"


namespace kvs
{

namespace mpi
{

/*===========================================================================*/
/**
 *  @brief  Log stream class.
 */
/*===========================================================================*/

class LogStream : public kvs::LogStream
{
private:
    int m_rank;

public:
    LogStream( const kvs::mpi::Communicator& comm ):
        m_rank( comm.rank() )
    {
    }

    LogStream( const kvs::mpi::Communicator& comm, const std::string& filename ):
        kvs::LogStream( filename ),
        m_rank( comm.rank() )
    {
    }

    std::ostream& operator ()( const int target_rank )
    {
        return kvs::LogStream::operator ()( target_rank == m_rank );
    }
};

} // end of namespace mpi

} // end of namespace kvs
