/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::Endian functions.
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: main.cpp 1070 2012-04-09 12:14:30Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#include <iostream>
#include <kvs/Endian>


/*===========================================================================*/
/**
 *  @brief  Main function.
 */
/*===========================================================================*/
int main( void )
{
    if ( kvs::Endian::IsBig() )
    {
        std::cout << "Byte-order is big-endian." << std::endl;
    }
    else
    {
        std::cout << "Byte-order is little-endian." << std::endl;
    }

    kvs::UInt8 a = 0x12;
    std::cout << "sizeof( kvs::UInt8 ): " << sizeof( kvs::UInt8 ) << std::endl;
    std::cout << "a: 0x" << std::hex << static_cast<unsigned>(a) << std::dec << std::endl;
    kvs::Endian::Swap( &a );
    std::cout << "a: 0x" << std::hex << static_cast<unsigned>(a) << std::dec << std::endl;

    kvs::UInt16 b = 0x1234;
    std::cout << "sizeof( kvs::UInt16 ): " << sizeof( kvs::UInt16 ) << std::endl;
    std::cout << "b: 0x" << std::hex << b << std::dec << std::endl;
    kvs::Endian::Swap( &b );
    std::cout << "b: 0x" << std::hex << b << std::dec << std::endl;

    kvs::UInt32 c = 0x12345678;
    std::cout << "sizeof( kvs::UInt32 ): " << sizeof( kvs::UInt32 ) << std::endl;
    std::cout << "c: 0x" << std::hex << c << std::dec << std::endl;
    kvs::Endian::Swap( &c );
    std::cout << "c: 0x" << std::hex << c << std::dec << std::endl;

    kvs::UInt64 d = 0x123456789abcdef0;
    std::cout << "sizeof( kvs::UInt64 ): " << sizeof( kvs::UInt64 ) << std::endl;
    std::cout << "d: 0x" << std::hex << d << std::dec << std::endl;
    kvs::Endian::Swap( &d );
    std::cout << "d: 0x" << std::hex << d << std::dec << std::endl;

    return 0;
}
