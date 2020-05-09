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
int main()
{
    // Examples for 'int'
    std::cout << "int min: " << kvs::Value<int>::Min() << std::endl;
    std::cout << "int max: " << kvs::Value<int>::Max() << std::endl;
    std::cout << "int epsilon: " << kvs::Value<int>::Epsilon() << std::endl;
    kvs::Value<int>::SetSeed( kvs::UInt32( time(0) ) * 1 );
    std::cout << "int rand in [INT_MIN,INT_MAX]: ";
    for ( int i = 0; i < 5; ++i ) std::cout << kvs::Value<int>::Random() << ", ";
    std::cout << std::endl;
    std::cout << "int rand in [0,9]: ";
    for ( int i = 0; i < 5; ++i ) std::cout << kvs::Value<int>::Random( 0, 9 ) << ", ";
    std::cout << std::endl;
    std::cout << std::endl;

    // Examples for 'float'
    std::cout << "float min: " << kvs::Value<float>::Min() << std::endl;
    std::cout << "float max: " << kvs::Value<float>::Max() << std::endl;
    std::cout << "float epsilon: " << kvs::Value<float>::Epsilon() << std::endl;
    kvs::Value<float>::SetSeed( kvs::UInt32( time(0) ) * 2 );
    std::cout << "float rand in [0,1]: ";
    for ( int i = 0; i < 5; ++i ) std::cout << kvs::Value<float>::Random() << ", ";
    std::cout << std::endl;
    std::cout << "int rand in [-1,1]: ";
    for ( int i = 0; i < 5; ++i ) std::cout << kvs::Value<float>::Random( -1, 1 ) << ", ";
    std::cout << std::endl;

    return 0;
}
