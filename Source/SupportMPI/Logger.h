/*****************************************************************************/
/**
 *  @file   Logger.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Communicator.h"


namespace kvs
{

namespace mpi
{

/*===========================================================================*/
/**
 *  @brief  Logger class.
 */
/*===========================================================================*/
class Logger
{
    class NullStream : public std::ostream
    {
        class Buffer : public std::streambuf
        {
        public:
            int overflow( int c ) { return c; }
        };

    private:
        Buffer m_buffer; ///< buffer;

    public:
        NullStream() : std::ostream( &m_buffer ) {}
    };

private:
    int m_rank; ///< my rank
    std::string m_filename; ///< output filename
    std::ofstream m_stream; ///< output stream
    NullStream m_null_stream; ///< null stream

public:
    Logger( const kvs::mpi::Communicator& comm ):
        m_rank( comm.rank() )
    {
    }

    Logger( const kvs::mpi::Communicator& comm, const std::string& filename ):
        m_rank( comm.rank() ),
        m_filename( filename ),
        m_stream( filename.c_str() )
    {
    }

    std::ostream& operator ()()
    {
        return m_filename.empty() ? std::cout : m_stream;
    }

    std::ostream& operator ()( const int target_rank )
    {
        return ( target_rank == m_rank ) ? (*this)() : m_null_stream;
    }
};

} // end of namespace mpi

} // end of namespace kvs
