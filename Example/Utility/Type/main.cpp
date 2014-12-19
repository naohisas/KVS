/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::Type.
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: main.cpp 1221 2012-06-18 10:51:25Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#include <iostream>
#include <iomanip>
#include <typeinfo>
#include <kvs/Type>


/*===========================================================================*/
/**
 *  @brief  Main function.
 */
/*===========================================================================*/
int main( void )
{
    std::cout << std::setiosflags(std::ios::boolalpha);

    // Re-defined primitive types in KVS.
    std::cout << "char == kvs::Int8 ? ";
    std::cout << ( typeid(char) == typeid(kvs::Int8) ) << std::endl;
    std::cout << "unsigned char == kvs::UInt8 ? ";
    std::cout << ( typeid(unsigned char) == typeid(kvs::UInt8) ) << std::endl;
    std::cout << "short == kvs::Int16 ? ";
    std::cout << ( typeid(short) == typeid(kvs::Int16) ) << std::endl;
    std::cout << "unsigned short == kvs::UInt16 ? ";
    std::cout << ( typeid(unsigned short) == typeid(kvs::UInt16) ) << std::endl;
    std::cout << "int == kvs::Int32 ? ";
    std::cout << ( typeid(int) == typeid(kvs::Int32) ) << std::endl;
    std::cout << "unsigned int == kvs::UInt32 ? ";
    std::cout << ( typeid(unsigned int) == typeid(kvs::UInt32) ) << std::endl;
    std::cout << "long == kvs::Int64 ? ";
    std::cout << ( typeid(int) == typeid(kvs::Int64) ) << std::endl;
    std::cout << "unsigned long == kvs::UInt64 ? ";
    std::cout << ( typeid(unsigned long) == typeid(kvs::UInt64) ) << std::endl;
    std::cout << "float == kvs::Real32 ? ";
    std::cout << ( typeid(float) == typeid(kvs::Real32) ) << std::endl;
    std::cout << "double == kvs::Real64 ? ";
    std::cout << ( typeid(double) == typeid(kvs::Real64) ) << std::endl;

    return 0;
}
