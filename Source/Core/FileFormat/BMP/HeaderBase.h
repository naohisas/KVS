/****************************************************************************/
/**
 *  @file   HeaderBase.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#ifndef KVS__BMP__HEADER_BASE_H_INCLUDE
#define KVS__BMP__HEADER_BASE_H_INCLUDE

#include <fstream>
#include <kvs/Platform>
#include <kvs/Type>


#ifdef KVS_PLATFORM_BIG_ENDIAN
#define BMP_HEADER_SWAP_BYTES (swapBytes())
#else
#define BMP_HEADER_SWAP_BYTES ((void)0)
#endif


namespace kvs
{

namespace bmp
{

struct HeaderBase
{
    template <typename T>
    static void ReadValue( std::ifstream& ifs, T* value )
    {
        ifs.read( reinterpret_cast<char*>( value ), sizeof( T ) );
    }

    template <typename T>
    static void WriteValue( std::ofstream& ofs, T value )
    {
        ofs.write( reinterpret_cast<char*>( &value ), sizeof( T ) );
    }

    virtual void swapBytes() = 0;
};

} // end of namespace bmp

} // end of namespace kvs

#endif // KVS_CORE_HEADER_BASE_H_INCLUDE
