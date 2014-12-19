/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#include <kvs/qt/Application>
#include "MainWindow.h"


/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument counter
 *  @param  argv [i] argument values
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::qt::Application app( argc, argv );

    const int width = 800;
    const int height = 600;
    MainWindow main_window( &app );
    main_window.resize( width, height );
    main_window.show();

    return app.run();
}
