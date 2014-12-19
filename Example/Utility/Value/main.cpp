/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Test program for kvs::Value class.
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: main.cpp 1139 2012-05-10 04:43:22Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#include <iostream>
#include <kvs/Value>


/*===========================================================================*/
/**
 *  @brief  Main function.
 */
/*===========================================================================*/
int main( void )
{
    std::cout << "char min: " << (int)kvs::Value<char>::Min() << std::endl;
    std::cout << "char max: " << (int)kvs::Value<char>::Max() << std::endl;
    std::cout << "char epsilon: " << (int)kvs::Value<char>::Epsilon() << std::endl;
    std::cout << std::endl;

    std::cout << "uchar min: " << (int)kvs::Value<unsigned char>::Min() << std::endl;
    std::cout << "uchar max: " << (int)kvs::Value<unsigned char>::Max() << std::endl;
    std::cout << "uchar epsilon: " << (int)kvs::Value<unsigned char>::Epsilon() << std::endl;
    std::cout << std::endl;

    std::cout << "short min: " << kvs::Value<short>::Min() << std::endl;
    std::cout << "short max: " << kvs::Value<short>::Max() << std::endl;
    std::cout << "short epsilon: " << kvs::Value<short>::Epsilon() << std::endl;
    std::cout << std::endl;

    std::cout << "ushort min: " << kvs::Value<unsigned short>::Min() << std::endl;
    std::cout << "ushort max: " << kvs::Value<unsigned short>::Max() << std::endl;
    std::cout << "ushort epsilon: " << kvs::Value<unsigned short>::Epsilon() << std::endl;
    std::cout << std::endl;

    std::cout << "int min: " << kvs::Value<int>::Min() << std::endl;
    std::cout << "int max: " << kvs::Value<int>::Max() << std::endl;
    std::cout << "int epsilon: " << kvs::Value<int>::Epsilon() << std::endl;
    std::cout << std::endl;

    std::cout << "uint min: " << kvs::Value<unsigned int>::Min() << std::endl;
    std::cout << "uint max: " << kvs::Value<unsigned int>::Max() << std::endl;
    std::cout << "uint epsilon: " << kvs::Value<unsigned int>::Epsilon() << std::endl;
    std::cout << std::endl;

    std::cout << "long min: " << kvs::Value<long>::Min() << std::endl;
    std::cout << "long max: " << kvs::Value<long>::Max() << std::endl;
    std::cout << "long epsilon: " << kvs::Value<long>::Epsilon() << std::endl;
    std::cout << std::endl;

    std::cout << "ulong min: " << kvs::Value<unsigned long>::Min() << std::endl;
    std::cout << "ulong max: " << kvs::Value<unsigned long>::Max() << std::endl;
    std::cout << "ulong epsilon: " << kvs::Value<unsigned long>::Epsilon() << std::endl;
    std::cout << std::endl;

    std::cout << "float min: " << kvs::Value<float>::Min() << std::endl;
    std::cout << "float max: " << kvs::Value<float>::Max() << std::endl;
    std::cout << "float epsilon: " << kvs::Value<float>::Epsilon() << std::endl;
    std::cout << std::endl;

    std::cout << "double min: " << kvs::Value<double>::Min() << std::endl;
    std::cout << "double max: " << kvs::Value<double>::Max() << std::endl;
    std::cout << "double epsilon: " << kvs::Value<double>::Epsilon() << std::endl;
    std::cout << std::endl;

    return 0;
}
