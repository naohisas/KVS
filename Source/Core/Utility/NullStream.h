#pragma once
#include <iostream>
#include <sstream>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Null stream class.
 */
/*===========================================================================*/
class NullStream : public std::ostream
{
private:
    class Buffer : public std::streambuf
    {
    public:
        int overflow( int c ) { return c; }
    } m_buffer;

public:
    NullStream() : std::ostream( &m_buffer ) {}
};

} // end of namespace kvs
