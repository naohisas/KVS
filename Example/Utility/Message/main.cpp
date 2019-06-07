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
int main()
{
    // Message with __FILE__, __LINE__, and __FUNC__.
    kvsMessage( "This is default message." );
    kvsMessage() << "This " << "is " << "also " << "default " << "message." << std::endl;

    // Debug message. This message is shown in debugging mode,
    // or no message is shown in release mode.
    kvsMessageDebug( "This is debug message." );
    kvsMessageDebug() << "This " << "is " << "also " << "debug " << "message." << std::endl;

    // Warning message.
    srand( time( NULL ) );
    if ( rand() % 2 == 0 )
    {
        kvsMessageWarning( "This is warning message." );
        kvsMessageWarning() << "This " << "is " << "also " << "warning " << "message." << std::endl;
    }

    // Error message. Breakpoint is set after calling kvsMessageError
    // in debugging mode.
    if ( rand() % 2 == 0 )
    {
        kvsMessageError( "This is error message." );
        kvsMessageError() << "This " << "is " << "also " << "error " << "message." << std::endl;
    }

    // Output assertion message. This message is shown in debugging mode.
    kvsMessageAssert( ( rand() % 2 == 0 ), "This is assertion message." );

    return 0;
}
