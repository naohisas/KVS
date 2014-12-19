/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::Message class.
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
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <kvs/Message>


/*===========================================================================*/
/**
 *  @brief  Main function.
 */
/*===========================================================================*/
int main( void )
{
    // Output error message.
    kvsMessageError( "This is an error message." );

    // Outpu debug message. This message is enable only when KVS_ENABLE_DEBUG
    // is available.
    kvsMessageDebug( "This is a debug message." );

    // Expression for displaying warning and assertion message.
    srand( time( NULL ) );
    bool expression;
    expression = ( rand() % 2 == 0 );

    // Output warning message. This message is enable only when KVS_ENABLE_DEBUG
    // is available.
    kvsMessageWarning( expression, "This is a warning message." );

    // Output assertion message. This message is enable only when KVS_ENABLE_DEBUG
    // is available.
    kvsMessageAssert( expression, "This is an assertion message." );

    return 0;
}
