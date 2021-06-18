/*****************************************************************************/
/**
 *  @file   LogStream.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <kvs/NullStream>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Log stream class.
 */
/*===========================================================================*/
class LogStream
{
private:
    std::string m_filename; ///< filename
    std::ofstream m_stream; ///< output stream
    kvs::NullStream m_null_stream; ///< null stream

public:
    LogStream() {}
    LogStream( const std::string& filename ):
        m_filename( filename ),
        m_stream( filename.c_str() ) {}

    std::ostream& operator ()() { return m_filename.empty() ? std::cout : m_stream; }
    std::ostream& operator ()( const bool enable ) { return ( enable ) ? (*this)() : m_null_stream; }
};

} // end of namespace kvs
